#ifndef TASK_H
#define TASK_H

#include <memory>
#include <string>
#include "MiscUtils.h"

class Task
{
public:
	typedef std::shared_ptr<Task> Ptr;
	typedef std::weak_ptr<Task> WeakPtr;
	
	/** Status of the task */
	enum class Status
	{
		Runned
		, Successed
		, Failed
		, Aborted
	};

	/** Static information about the task */
	struct Info
	{
		/** ID */
		std::string id;
		/** Title of the task */
		std::string title;
		/** Comlexity */
		float severity;
		/** Time needless to complete the task */
		Utils::GameTime duration;
		float moralLevel;

		/** The name of the Lua-function, that will be run on successful end of the task */
		std::string successFn;
		/** The name of the Lua-function, that will be run on fail of the task */
		std::string failFn;
		/** The name of the Lua-function, that will be run on abort of the task */
		std::string abortFn;
	};

	struct CompletedTaskInfo
	{
		const Info* taskInfo;
		Utils::GameTime startTime;
		Utils::GameTime endTime;
		Status status;
	};

public:
	Task(const Task::Info* info, Utils::GameTime startTime);
	
	static Ptr Create(const Info* info, Utils::GameTime startTime);

	/**
	 * Check task completeness
	 *
	 * @return true if task is completed
	 */
	bool CheckCompleteness(Utils::GameTime worldTime);

	const Info* GetInfo() const;

	Utils::GameTime GetStartTime() const;
	Utils::GameTime GetEndTime() const;
	float CalculateProgress(Utils::GameTime worldTime) const;
	bool IsAborted() const;
	bool IsFastFinished() const;
	/** Abort task */
	void Abort();
	/** Finish the task now (cheat) */
	void FastFinish();

private:
	/** Start time of the task */
	Utils::GameTime _startTime;
	/** Time when task will be completed */
	Utils::GameTime _endTime;

	bool _isAborted;
	bool _isFastFinished;

	const Task::Info* _info;
};

#endif // TASK_H
