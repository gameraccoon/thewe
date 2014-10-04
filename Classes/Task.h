#ifndef TASK_H
#define TASK_H

#include <memory>
#include <string>

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
		float duration;
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
		float startTime;
		float endTime;
		Status status;
	};

public:
	Task(const Task::Info* info, float startTime);
	
	static Ptr Create(const Info* info, float startTime);

	/**
	 * Check task completeness
	 *
	 * @return true if task is completed
	 */
	bool CheckCompleteness(float worldTime);

	const Info* GetInfo() const;

	float GetStartTime() const;
	float GetEndTime() const;
	float CalculateProgress(float worldTime) const;
	bool IsAborted() const;
	bool IsFastFinished() const;
	/** Abort task */
	void Abort();
	/** Finish the task now (cheat) */
	void FastFinish();

private:
	/** Start time of the task */
	float _startTime;
	/** Time when task will be completed */
	float _endTime;

	bool _isAborted;
	bool _isFastFinished;

	const Task::Info* _info;
};

#endif // TASK_H
