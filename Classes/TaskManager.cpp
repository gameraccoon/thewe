#include "TaskManager.h"

#include "MessageManager.h"

#include "Log.h"
#include "LuaInstance.h"
#include "GameInfo.h"

#include <cocos2d.h>

#include <luabind/luabind.hpp>

TaskManager::TaskManager()
	: _isTasksFilled(false)
{
	std::string fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename("tasks.lua");
	_luaScript = new LuaInstance();

	_luaScript->BindClass<Log>();
	_luaScript->BindClass<MessageManager>();
	_luaScript->BindClass<GameInfo>();
	_luaScript->BindClass<Cell::Info>();
	_luaScript->BindClass<const Task::Info>();
	_luaScript->BindClass<Vector2>();

	_luaScript->RegisterVariable("Log", &(Log::Instance()));
	_luaScript->RegisterVariable("MessageManager", &(MessageManager::Instance()));
	_luaScript->RegisterVariable("GameInfo", &(GameInfo::Instance()));
	std::string script = cocos2d::FileUtils::getInstance()->getStringFromFile(fullPath);

	_luaScript->ExecScript(script.c_str());
}

TaskManager::~TaskManager()
{
	delete _luaScript;
}

TaskManager& TaskManager::Instance()
{
	static TaskManager singleInstance;
	return singleInstance;
}

void TaskManager::RunTask(Cell::WeakPtr cell, const Task::Info* info, Utils::GameTime startTime)
{
	RunnedTaskInfo runnedTaskInfo;
	runnedTaskInfo.cell = cell;
	runnedTaskInfo.task = Task::Create(info, startTime);
	cell.lock()->AddCurrentTask(runnedTaskInfo.task);

	_runnedTasks.push_back(runnedTaskInfo);
}

void TaskManager::RunTask(Cell::WeakPtr cell, const std::string& id, Utils::GameTime startTime)
{
	const Task::Info* taskInfo = GetTaskInfoById(id);
	if (taskInfo)
	{
		RunTask(cell, taskInfo, startTime);
	}
	else
	{
		Log::Instance().writeWarning(std::string("Wrong task id ").append(id));
	}
}

void TaskManager::UpdateToTime(Utils::GameTime worldTime)
{
	std::vector<RunnedTaskInfo>::iterator iterator = _runnedTasks.begin();
	while (iterator != _runnedTasks.end())
	{
		Task* task = iterator->task.get();

		bool isEnded = iterator->task->CheckCompleteness(worldTime);

		if (isEnded)
		{
			Cell::Ptr cell = iterator->cell.lock();
			// if Cell wasn't removed yet
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
					// call lua function that calculate status of the task
					bool isSuccess = luabind::call_function<bool>(_luaScript->GetLuaState()
																  , "CheckStatus"
																  , cellInfo
																  , taskInfo
																  , 0);

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

				// call task end function (success, fail, abort)
				luabind::call_function<bool>(_luaScript->GetLuaState()
											 , funcName.c_str()
											 , &cell->GetInfo()
											 , taskInfo
											 , 0);

				// adds information of the completed task to the cell
				cell->AddCompletedTask(info);
			}

			// release smart ptr
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
	if (_isTasksFilled)
	{
		Log::Instance().writeWarning("Trying to fill tasks info twice");
	}

	for (const Task::Info& info : tasks)
	{
		_CheckTask(info);
		_allTasks.insert(std::pair<const std::string, const Task::Info>(info.id, info));
	}

	_isTasksFilled = true;
}

TaskManager::TasksList TaskManager::GetAvailableTasks(Cell::WeakPtr cell) const
{
	if (!_isTasksFilled)
	{
		Log::Instance().writeError("Trying to acces to not initialized TaskManager");
	}

	std::vector<const Task::Info*> availableTasks;

	for (const auto& pair : _allTasks)
	{
		// ToDo: add some checks
		availableTasks.push_back(&pair.second);
	}

	return availableTasks;
}

void TaskManager::_CheckTask(const Task::Info& taskInfo) const
{
	if (taskInfo.id == "")
	{
		Log::Instance().writeWarning("Empty task Id");
	}

	if (taskInfo.successFn == "")
	{
		Log::Instance().writeWarning("Empty success function name");
	}

	if (taskInfo.failFn == "")
	{
		Log::Instance().writeWarning("Empty fail function name");
	}

	if (taskInfo.abortFn == "")
	{
		Log::Instance().writeWarning("Empty abort function name");
	}

	if (taskInfo.moralLevel < 0.0f || 1.0f < taskInfo.moralLevel)
	{
		Log::Instance().writeWarning("Wrong moral level");
	}

	if (taskInfo.severity < 0.0f || 1.0f < taskInfo.severity)
	{
		Log::Instance().writeWarning("Wrong severity level");
	}

	if (taskInfo.duration < 0.0f)
	{
		Log::Instance().writeWarning("Negative duration");
	}
}

const Task::Info* TaskManager::GetTaskInfoById(const std::string& id)
{
	auto taskInfoIterator = _allTasks.find(id);
	if (taskInfoIterator != _allTasks.end())
	{
		return &(taskInfoIterator->second);
	}
	else
	{
		return nullptr;
	}
}
