#include "TaskManager.h"

#include "Log.h"

TaskManager::TaskManager()
{
	isTasksFilled = false;
}

TaskManager::~TaskManager()
{
}

TaskManager& TaskManager::Instance()
{
	static TaskManager singleInstance;
	return singleInstance;
}

void TaskManager::RunTask(Cell::Ptr &cell, const Task::Info* info, float startTime)
{
	RunnedTaskInfo runnedTaskInfo;
	runnedTaskInfo.cell = cell;
	runnedTaskInfo.task = std::make_shared<Task>(Task(info, startTime));

	_runnedTasks.push_back(runnedTaskInfo);
}

void TaskManager::UpdateToTime(float worldTime)
{
	auto iterator = _runnedTasks.begin(), iEnd = _runnedTasks.end();
	while (iterator < iEnd)
	{
		Task* task = iterator->task.get();

		bool isComplete = iterator->task->CheckComleteness(worldTime);

		if (isComplete)
		{
			Task::CompletedTaskInfo info;
			info.taskInfo = task->GetInfo();
			info.startTime = task->GetStartTime();
			info.endTime = task->GetEndTime();
			iterator->cell->AddCompletedTask(info);

			// release smart ptr and delete task
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
		Log::Instance().writeError("Trying to double filling tasks table");
		assert(false);
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