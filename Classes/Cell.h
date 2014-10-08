#ifndef CELL_H
#define CELL_H

#include <memory>

#include "Town.h"
#include "Vector2.h"
#include "Task.h"

class Cell
{
public:
	enum State
	{
		CONSTRUCTION,
		READY,
	};

	struct Info
	{
		Cell *parent;
		Town::WeakPtr town;
		Vector2 location;
		State state;

		float morale;
		float contentment;
		float constructionProgress;
		
		int cash;
		int membersCount;

		Info(void) : constructionProgress(0)
		{}
	};

	typedef std::shared_ptr<Cell> Ptr;
	typedef std::weak_ptr<Cell> WeakPtr;

public:
	Cell(const Info &info, Utils::GameTime constructionTime);

	/**
	 * Create Cell and return a smart ptr
	 */
	static Ptr Create(const Info &info, Utils::GameTime constructionTime);
	
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
	const std::vector<Cell::Ptr>& GetChildren() const;

	const Cell* GetParent() const;

	/**
	* Returns information about the cell
	*/
	Info& GetInfo(void);

	Utils::GameTime GetConstructionTime(void) const;
	
	/** Updated cell's state */
	void Update(float deltatime);

	void AddCurrentTask(Task::WeakPtr currentTask);
	Task::WeakPtr getCurrentTask() const;

	bool IsCurrentTaskPresented(void) const;

	/** Add completed task */
	void AddCompletedTask(const Task::CompletedTaskInfo& completedTask);

	unsigned int GetUid(void) const;

private:

	/** Set parent for the cell */
	void _SetParent(Cell *cell);

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

	Utils::GameTime _constructionTime;

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
