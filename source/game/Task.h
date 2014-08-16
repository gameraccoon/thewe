#ifndef TASK_H
#define TASK_H

#include <memory>
#include <string>

struct Task
{
public:
	typedef std::shared_ptr<Task> Ptr;
	
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
		
		void (*successFn)();
		void (*failFn)();
		void (*abortFn)();
	private:
		/** Закрытый конструктор */
		Info();
		/** Разрешаем конструировать Task::Info только классу TaskManager */
		friend class TaskManager;
	};

	struct CompletedTaskInfo
	{
		const Info* taskInfo;
		float startTime;
		float endTime;
	};

public:
	Task(const Task::Info* info, float startTime);

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