#include "TaskManager.h"

#include "Log.h"
#include "World.h"
#include "LuaInstance.h"
#include "WorldLoader.h"
#include "MessageManager.h"

#include <luabind/luabind.hpp>

TaskManager::TaskManager()
	: _isTasksFilled(false)
{
}

void TaskManager::RunTask(Cell::WeakPtr cell, const Task::Info* info, Utils::GameTime startTime)
{
	RunnedTaskInfo runnedTaskInfo;
	runnedTaskInfo.cell = cell;
	runnedTaskInfo.task = Task::Create(info, startTime);
	cell.lock()->AddCurrentTask(runnedTaskInfo.task);

	_runnedTasks.push_back(runnedTaskInfo);

	MessageManager::Instance().PutMessage(Message("SaveGame"));
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

void TaskManager::CallCuccessfulCompletition(Cell::WeakPtr cell, const Task::Info *info)
{
	if (!info || cell.expired()) {
		Log::Instance().writeWarning("TaskManager::CallCuccessfulCompletition wrong params");
		return;
	}

	luabind::call_function<bool>(World::Instance().GetLuaInst()->GetLuaState()
								, info->successFn.c_str()
								, cell.lock().get()
								, info
								, 0);
}

Task::Info TaskManager::FindTaskById(const std::string &id) const
{
	Task::Info def;

	std::map<const std::string, const Task::Info>::const_iterator it;
	it = _allTasks.find(id);
	return it != _allTasks.end() ? (*it).second : def;
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
					bool isSuccess = luabind::call_function<bool>(World::Instance().GetLuaInst()->GetLuaState()
																  , "CheckTaskStatus"
																  , cellInfo
																  , taskInfo
																  , 0);

					if (isSuccess)
					{
						//funcName = taskInfo->successFn;
						//info.status = Task::Status::Successed;

						Message message("PushTaskRewardOnMap");
						message.variables.SetInt("CELL_UID", cell->GetUid());
						message.variables.SetString("TASK_ID", taskInfo->id);
						MessageManager::Instance().PutMessage(message);
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

				if (!funcName.empty())
				{
					// call task end function (success, fail, abort)
					luabind::call_function<bool>(World::Instance().GetLuaInst()->GetLuaState()
												 , funcName.c_str()
												 , cell.get()
												 , taskInfo
												 , 0);
				}

				// adds information of the completed task to the cell
				cell->AddCompletedTask(info);
			}

			// release smart ptr
			iterator = _runnedTasks.erase(iterator);

			MessageManager::Instance().PutMessage(Message("SaveGame"));
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

float TaskManager::CalcTaskSuccessChance(const Cell::Info& cellInfo, const Task::Info* taskInfo)
{
	return luabind::call_function<float>(World::Instance().GetLuaInst()->GetLuaState()
												  , "CalcTaskSuccessChance"
												  , cellInfo
												  , taskInfo
												  , 0);
}

TaskManager::Tasks TaskManager::GetAvailableTasks(Cell::WeakPtr cell) const
{
	if (!_isTasksFilled)
	{
		Log::Instance().writeError("Trying to acces to not initialized TaskManager");
	}

	std::vector<const Task::Info*> availableTasks;

	for (const auto& pair : _allTasks)
	{
		bool add = luabind::call_function<bool>(World::Instance().GetLuaInst()->GetLuaState()
													  , "IsShowTaskInList"
													  , cell.lock().get()
													  , pair.second
													  , 0);
		if (add)
		{
			availableTasks.push_back(&pair.second);
		}
	}

	return availableTasks;
}

void TaskManager::_CheckTask(const Task::Info& taskInfo) const
{
	if (taskInfo.id == "")
	{
		Log::Instance().writeWarning("Empty task id");
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
		Log::Instance().writeWarning("Wrong task moral level");
	}

	if (taskInfo.fameImpact < 0.0f || 1.0f < taskInfo.fameImpact)
	{
		Log::Instance().writeWarning("Wrong fameImpact level");
	}

	if (taskInfo.chanceToLooseMembers < 0.0f || 1.0f < taskInfo.chanceToLooseMembers)
	{
		Log::Instance().writeWarning("Wrong chanceToLooseMembers value");
	}

	if (taskInfo.heartPoundingLevel < 0.0f || 1.0f < taskInfo.heartPoundingLevel)
	{
		Log::Instance().writeWarning("Wrong heartPoundingLevel value");
	}

	if (taskInfo.duration < 0.0f)
	{
		Log::Instance().writeWarning("Negative task duration");
	}

	if (taskInfo.level < 0.0f)
	{
		Log::Instance().writeWarning("Negative task level");
	}

	if (taskInfo.needCash < 0)
	{
		Log::Instance().writeWarning("Negative needCash value");
	}

	if (taskInfo.needMembers < 0)
	{
		Log::Instance().writeWarning("Negative needMembers value");
	}

	if (taskInfo.needTech < 0)
	{
		Log::Instance().writeWarning("Negative needTech value");
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
