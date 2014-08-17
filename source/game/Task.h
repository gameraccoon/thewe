#ifndef TASK_H
#define TASK_H

#include <memory>
#include <string>

struct Task
{
public:
	typedef std::shared_ptr<Task> Ptr;
	typedef std::weak_ptr<Task> WeakPtr;
	
	/** Статическая информация о задаче */
	struct Info
	{
		/** Идентификатор */
		std::string id;
		/** Сложность */
		float severity;
		/** Продолжительность */
		float duration;
		float moralLevel;

		/** Имя Lua-функции, которая будет выполнена при успешном финале */
		std::string successFn;
		/** Имя Lua-функции, которая будет выполнена при провале задания */
		std::string failFn;
		/** Имя Lua-функции, которая будет выполнена при прерывании задания */
		std::string abortFn;
	};

	struct CompletedTaskInfo
	{
		const Info* taskInfo;
		float startTime;
		float endTime;
	};

public:
	Task(const Task::Info* info, float startTime);
	
	static Task::Ptr Create(const Task::Info* info, float startTime);

	/**
	 * Проверяет, не выполнилось ли задание
	 *
	 * @return true если задание уже выполнено
	 */
	bool CheckComleteness(float worldTime);

	const Info* GetInfo() const;

	float GetStartTime() const;
	float GetEndTime() const;

private:
	/** Время начала выполнения задания */
	float _startTime;
	/** Время окончания выполнения задания */
	float _endTime;

	const Task::Info* _info;
};

#endif // TASK_H