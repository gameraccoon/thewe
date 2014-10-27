#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <vector>
#include <map>
#include "Cell.h"
#include "Task.h"

class TaskManager
{
public:
	typedef std::vector<const Task::Info*> TasksList;

public:
	TaskManager();

	/**
	 * Run new task for the cell
	 * 
	 * @param cell cell that work on this task
	 * @param info information about task
	 * @param startTime start time of this task
	 */
	void RunTask(Cell::WeakPtr cell, const Task::Info* info, Utils::GameTime startTime);
	void RunTask(Cell::WeakPtr cell, const std::string& info, Utils::GameTime startTime);

	/**
	 * Returns tasks that available for current cell for now
	 */
	TasksList GetAvailableTasks(Cell::WeakPtr cell) const;

	/** Update information to the time */
	void UpdateToTime(Utils::GameTime worldTime);

	void FillTasks(const std::vector<Task::Info>& tasks);

	float CalcTaskSuccessChance(const Cell::Info &cellInfo, const Task::Info* taskInfo);
private:
	/** Information about runned task */
	struct RunnedTaskInfo
	{
		/** Task */
		Task::Ptr task;
		/** Cell that work on the task */
		Cell::WeakPtr cell;
	};

private:
	/** Information about all tasks available in the game (updates only once) */
	std::map<const std::string, const Task::Info> _allTasks;
	/** Flag that shows is tasks already filled */
	bool _isTasksFilled;

	/** Tasks that now in process */
	std::vector<RunnedTaskInfo> _runnedTasks;

private:
	/** Check all values and write all warnings in the log */
	void _CheckTask(const Task::Info& taskInfo) const;

	const Task::Info* GetTaskInfoById(const std::string& id);
};

#endif // TASK_MANAGER_H
