#include "TaskManager.h"

#include <fstream>
#include <iostream>

#include "Log.h"
#include "LuaType.h"
#include "LuaFunction.h"

#include <cocos2d.h>

TaskManager::TaskManager()
{
	isTasksFilled = false;

	_luaScript = LuaInstance::Create();

	LuaType::registerConstant(_luaScript.get(), "Log", &Log::Instance());

	std::string fileName = "../_gamedata/scripts/tasks.lua";
	std::string script;
    char temp;
	std::ifstream fileStream(fileName);
    
    if (!fileStream)
	{
		Log::Instance().writeError(std::string("File ").append(fileName).append(" not found error"));
    }

    fileStream.unsetf(std::ios::skipws);
    while(fileStream >> temp)
	{
        script += temp;
	}

	_luaScript->execScript(script.c_str());
}

TaskManager::~TaskManager()
{
}

TaskManager& TaskManager::Instance()
{
	static TaskManager singleInstance;
	return singleInstance;
}

void TaskManager::RunTask(Cell::WeakPtr &cell, const Task::Info* info, float startTime)
{
	RunnedTaskInfo runnedTaskInfo;
	runnedTaskInfo.cell = cell;
	runnedTaskInfo.task = Task::Create(info, startTime);
	cell._Get()->AddCurrentTask(runnedTaskInfo.task);

	_runnedTasks.push_back(runnedTaskInfo);
}

void TaskManager::UpdateToTime(float worldTime)
{
	std::vector<RunnedTaskInfo>::iterator iterator = _runnedTasks.begin();
	while (iterator != _runnedTasks.end())
	{
		Task* task = iterator->task.get();

		bool isEnded = iterator->task->CheckCompleteness(worldTime);

		if (isEnded)
		{
			Cell::Ptr cell = iterator->cell.lock();
			// если Cell ещё не удалена
			if (cell)
			{
				const Cell::Info& cellInfo = cell->GetInfo();
				const Task::Info* taskInfo = task->GetInfo();

				Task::CompletedTaskInfo info;
				info.taskInfo = task->GetInfo();
				info.startTime = task->GetStartTime();
				info.endTime = (task->IsAborted() || task->IsFastFinished()) ? worldTime : task->GetEndTime();
				
				std::string funcName;

				if (!task->IsAborted())
				{
					// Вызываем луа функцию определения статуса задания
					LuaFunction luaFunction;
					luaFunction.readyToRunFunction(_luaScript.get(), "CheckStatus");
						_luaScript->sendToLua<int>(cellInfo.membersCount);
						_luaScript->sendToLua<float>(cellInfo.morale);
						_luaScript->sendToLua<float>(cellInfo.contentment);
						_luaScript->sendToLua<float>(taskInfo->moralLevel);
						_luaScript->sendToLua<float>(taskInfo->severity);
					luaFunction.runFunction(5, 1);
						bool isSuccess = _luaScript->getFromLua<bool>(1);
					luaFunction.clearAfterFunction();

					if (isSuccess)
					{
						funcName = taskInfo->successFn;
						info.status = Task::Status::Successed;
					}
					else
					{
						funcName = taskInfo->failFn;
						info.status = Task::Status::Failed;
					}
				}
				else
				{
					funcName = taskInfo->abortFn;
					info.status = Task::Status::Aborted;
				}

				// вызываем из луа нужную функцию
				LuaType::registerConstant(_luaScript.get(), "cell", cell.get());
				LuaFunction luaFunction;
				luaFunction.readyToRunFunction(_luaScript.get(), funcName.c_str());
				luaFunction.runFunction(0, 0);
				luaFunction.clearAfterFunction();
				_luaScript->removeSymbol("cell");
				
				// добавляем информацию о законченном задании в ячейку
				cell->AddCompletedTask(info);
			}

			// освобождаем умный указатель и удаляем Task
			iterator = _runnedTasks.erase(iterator);
		}
		else
		{
			++iterator;
		}
	}
}

void TaskManager::FillTasks(const std::vector<Task::Info>& tasks)
{
	if (isTasksFilled)
	{
		Log::Instance().writeWarning("Trying to fill tasks info twice");
	}

	for (const Task::Info& info : tasks)
	{
		_allTasks.insert(std::pair<const std::string, const Task::Info>(info.id, info));
	}

	isTasksFilled = true;
}

const std::vector<const Task::Info*> TaskManager::GetAvailableTasks(Cell* cell) const
{
	
	if (!isTasksFilled)
	{
		Log::Instance().writeError("Trying to acces to not initialized TaskManager");
		assert(false);
	}

	std::vector<const Task::Info*> availableTasks;

	for (const auto& pair : _allTasks)
	{
		// ToDo: тут надо добавить необходимые проверки
		availableTasks.push_back(&pair.second);
	}

	return availableTasks;
}