#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <vector>
#include "Cell.h"
#include "Task.h"

class TaskManager
{
public:
	typedef std::vector<const Task::Info*> TasksList;

public:
	/**
	 * Возвращает экземпляр класса
	 */
	static TaskManager& Instance();
	
public:
	/**
	 * Запустить новое задание для ячейки
	 * 
	 * @param cell ячейка для которой запускается задание
	 * @param info информация о задании
	 * @param startTime игровое время, которое нужно считать
	 * временем начала выполнения задачи
	 */
	void RunTask(Cell::WeakPtr &cell, const Task::Info* info, float startTime);
	
	/**
	 * Возвращает список задач, которые достпны для данной ячейки
	 */
	TasksList GetAvailableTasks(Cell::WeakPtr cell) const;

	/** Обновить информацию к текущему игровому времени */
	void UpdateToTime(float worldTime);

	void FillTasks(const std::vector<Task::Info>& tasks);
private:
	/** Информация о запущенной задаче */
	struct RunnedTaskInfo
	{
		/** Задание */
		Task::Ptr task;
		/** Ячейка, которая выполняет задание */
		Cell::WeakPtr cell;
	};

private:
	/** Информация о всех возможных заданиях (заполняется один раз) */
	std::map<const std::string, const Task::Info> _allTasks;
	/** Флаг, показывающий заполнен ли список всех возможных задач */
	bool isTasksFilled;

	/** Задания, которые сейчас выполняются */
	std::vector<RunnedTaskInfo> _runnedTasks;

private:
	/** Проверить информацию и вывести все предупреждения в журнал */
	void _CheckTask(const Task::Info& taskInfo) const;

	/*
	 * Отключаем ненужные операции
	 */
	TaskManager();
	~TaskManager();
	TaskManager(const TaskManager&);
	void operator=(const TaskManager&);
};

#endif // TASK_MANAGER_H
