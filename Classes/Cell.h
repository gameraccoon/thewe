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

	enum State
	{
		CONSTRUCTION,
		READY,
	};

	typedef std::shared_ptr<Cell> Ptr;
	typedef std::weak_ptr<Cell> WeakPtr;

public:
	Cell(const Info &info);

	/**
	 * Create Cell and return a smart ptr
	 */
	static Ptr Create(const Info &info);
	
	/**
	 * Adds new child to the cell
	 * This cell automatically adds as parent
	 */
	void AddChild(Cell::Ptr cell);

	/**
	 * Removes child cell
	 * Set child's parent to null
	 */
	void RemoveChild(Cell::Ptr cell);

	/** Returns all child cells */
	const std::vector<Cell::Ptr>& GetChildrens() const;

	/**
	* Returns information about the cell
	*/
	Info& GetInfo(void);

	State& GetState(void);

	void SetConstructionTime(float seconds);

	float GetConstructionTime(void) const;
	
	/** Updated cell's state */
	void Update(float deltatime);

	void AddCurrentTask(Task::WeakPtr currentTask);
	Task::WeakPtr getCurrentTask() const;

	void SwitchState(State state);

	bool IsCurrentTaskPresented(void) const;

	/** Add completed task */
	void AddCompletedTask(const Task::CompletedTaskInfo& completedTask);

	float GetCash();
	unsigned int GetUid(void) const;

private:

	/** Set parent for the cell */
	void _SetParent(Cell* cell);

	/** Recalculate cash */
	void _UpdateCash(float deltatime);
	/** Recalculate morale value */
	void _UpdateMorale(float deltatime);
	/** Recalculate contentement level */
	void _UpdateContentment(float deltatime);

	/** Check all values and write warnings to the log */
	void _CheckValues() const;

	/** All usual info about the cell */
	Info _info;

	State _state;

	float _constructionTime;

	/**  уникальный идентификатор */
	unsigned int _uid;

	/** Child cells */
	std::vector<Cell::Ptr> _childCells;

	/** Task that runned now */
	Task::WeakPtr _currentTask;

	/** List of recently completed task */
	std::vector<Task::CompletedTaskInfo> _completedTasks;

	/** Geographic position on the map */
	Vector2 _worldLocation;
};

#endif // CELL_H
