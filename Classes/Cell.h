#ifndef CELL_H
#define CELL_H

#include <memory>

#include "Town.h"
#include "Vector2.h"
#include "Task.h"

class Cell
{
public:
	struct Info
	{
		Cell *parent;
		Town::WeakPtr town;
		Vector2 location;

		float cash;
		float morale;
		float contentment;

		int membersCount;
	};

	typedef std::shared_ptr<Cell> Ptr;
	typedef std::weak_ptr<Cell> WeakPtr;

public:
	Cell(const Info &info);

	/**
	 * Создать Cell и вернуть умный указатель
	 */
	static Ptr Create(const Info &info);
	
	/**
	 * Добавляет ячейке новую дочернюю ячейку
	 * Ячейка сразу прописывается как родительская к дочерней ячейке
	 */
	void AddChild(Cell::Ptr cell);

	/**
	 * Удаляет дочернюю ячейку
	 * Ставит в nullptr значение родительской ячейки у дочерней
	 */
	void RemoveChild(Cell::Ptr cell);

	/** Возвращает все дочерние ячейки */
	const std::vector<Cell::Ptr>& GetChildrens() const;

	/**
	* Возвращает информацию о ячейке
	*/
	const Info& GetInfo(void) const;
	
	/** Обновляем состояние ячейки */
	void Update(float deltatime);

	void AddCurrentTask(Task::WeakPtr currentTask);
	Task::WeakPtr getCurrentTask() const;

	/** Добавляем задание как выполненное */
	void AddCompletedTask(const Task::CompletedTaskInfo& completedTask);
private:

	/** Установить ячейке родителя */
	void _SetParent(Cell* cell);

	/** Пересчитать наличные со временем */
	void _UpdateCash(float deltatime);
	/** Пересчитать удовлетворённость со временем */
	void _UpdateMorale(float deltatime);
	/** Пересчитать удовлетворённость со временем */
	void _UpdateContentment(float deltatime);

	/** Проверить значения и вывести в журнал все предупреждения */
	void _CheckValues() const;

	/** Вся важная информация о ячейке */
	Info _info;

	/** Дочерние ячейки */
	std::vector<Cell::Ptr> _childCells;

	/** Задание, которое сейчас выполняется */
	Task::WeakPtr _currentTask;

	/** Список выполненных заданий */
	std::vector<Task::CompletedTaskInfo> _completedTasks;

	/** Географическое расположение ячейки на карте */
	Vector2 _worldLocation;
};

#endif // CELL_H