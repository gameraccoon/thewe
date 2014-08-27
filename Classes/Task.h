#ifndef TASK_H
#define TASK_H

#include <memory>
#include <string>

struct Task
{
public:
	typedef std::shared_ptr<Task> Ptr;
	typedef std::weak_ptr<Task> WeakPtr;
	
	/** Статус задачи */
	enum class Status
	{
		Runned
		, Successed
		, Failed
		, Aborted
	};

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
		Status status;
	};

public:
	Task(const Task::Info* info, float startTime);
	
	static Ptr Create(const Info* info, float startTime);

	/**
	 * Проверяет, не выполнилось ли задание
	 *
	 * @return true если задание уже выполнено
	 */
	bool CheckCompleteness(float worldTime);

	const Info* GetInfo() const;

	float GetStartTime() const;
	float GetEndTime() const;
	float CalculateProgress(float worldTime) const;
	bool IsAborted() const;
	bool IsFastFinished() const;
	/** Прервать выполнение задания */
	void Abort();
	/** Завершить задание сейчас (ускорить выполнение) */
	void FastFinish();

private:
	/** Время начала выполнения задания */
	float _startTime;
	/** Время окончания выполнения задания */
	float _endTime;

	bool _isAborted;
	bool _isFastFinished;

	const Task::Info* _info;
};

#endif // TASK_H