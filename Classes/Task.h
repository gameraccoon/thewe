#ifndef TASK_H
#define TASK_H

#include <memory>
#include <string>
#include <vector>

#include "MiscUtils.h"
#include "Resources.h"
#include "Member.h"

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
	};

	struct Executant
	{
		std::string special;
		int count;
		int exp;
	};

	/** Static information about the task */
	struct Info
	{
		/** ID */
		std::string id;
		/** Time need to complete the task */
		Utils::GameTime duration;
		int level;

		Resource::Vector reward;
		std::vector<Executant> members;

		/** The name of the Lua-function, that will be run on successful end of the task */
		std::string successFn;
		/** The name of the Lua-function, that will be run on fail of the task */
		std::string failFn;
	};

	struct CompletedTaskInfo
	{
		const Info* taskInfo;
		Utils::GameTime startTime;
		Utils::GameTime endTime;
		Status status;
	};

public:
	Task(const Task::Info &info);

	static Ptr Create(const Info &info);

	/**
	 * Check task completeness
	 *
	 * @return true if task is completed
	 */
	bool CheckCompleteness(Utils::GameTime worldTime);

	void Start(Utils::GameTime startTime);

	const Info& GetInfo() const;

	Utils::GameTime GetStartTime() const;
	Utils::GameTime GetEndTime() const;
	float CalculateProgress(Utils::GameTime worldTime) const;

	void AddExecutant(Member::Ptr executant);
	void RemoveExecutant(Member::Ptr executant);
	void ReleaseExecutants(void);

private:
	/** Start time of the task */
	Utils::GameTime _startTime;
	/** Time when task will be completed */
	Utils::GameTime _endTime;
	Member::Vector _executants;

	const Task::Info _info;
};

#endif // TASK_H
