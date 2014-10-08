#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <vector>
#include "Cell.h"
#include "Task.h"
#include "LuaInstance.h"

class TaskManager
{
public:
	typedef std::vector<const Task::Info*> TasksList;

public:
	/**
	 * Return single instance of the class
	 */
	static TaskManager& Instance();
	
public:
	/**
	 * Run new task for the cell
	 * 
	 * @param cell cell that work on this task
	 * @param info information about task
	 * @param startTime start time of this task
	 */
	void RunTask(Cell::WeakPtr &cell, const Task::Info* info, Utils::GameTime startTime);
	void RunTask(Cell::WeakPtr &cell, const std::string& info, Utils::GameTime startTime);

	/**
	 * Returns tasks that available for current cell for now
	 */
	TasksList GetAvailableTasks(Cell::WeakPtr cell) const;

	/** Update information to the time */
	void UpdateToTime(Utils::GameTime worldTime);

	void FillTasks(const std::vector<Task::Info>& tasks);
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
	/** Context for running lua-scripts */
	LuaInstance* _luaScript;
private:
	/** Check all values and write all warnings in the log */
	void _CheckTask(const Task::Info& taskInfo) const;

	const Task::Info* GetTaskInfoById(const std::string& id);

	/*
	 * Turn off unusual operations
	 */
	TaskManager();
	~TaskManager();
	TaskManager(const TaskManager&);
	void operator=(const TaskManager&);
};

#endif // TASK_MANAGER_H
