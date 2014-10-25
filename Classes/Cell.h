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
		CONSTRUCTION = 0,
		ARRESTED,
		READY,
		DESTRUCTION
	};

	enum Specialization
	{
		NORMAL
	};

	struct Info
	{
		Cell *parent;
		Town::WeakPtr town;
		Vector2 location;
		State state;

		int cash;
		int membersCount;
		int ratsCount;
		int techUnitsCount;
		int experience;
		Specialization specialization;
		float morale;
		float devotion;
		float fame;
		float townInfluence;
		float townHeartPounding;
		float townWelfare;

		Utils::GameTime constructionBegin;
		Utils::GameTime constructionDuration;
		Utils::GameTime destructionBegin;
		Utils::GameTime destructionDuration;
	};

	typedef std::shared_ptr<Cell> Ptr;
	typedef std::weak_ptr<Cell> WeakPtr;

public:
	Cell(const Info &info);
	~Cell(void);

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

	void Kill(void);

	/** Returns all child cells */
	const std::vector<Cell::Ptr>& GetChildren() const;

	const Cell* GetParent() const;

	/**
	* Returns information about the cell
	*/
	Info& GetInfo(void);

	void AddCurrentTask(Task::WeakPtr currentTask);
	Task::WeakPtr getCurrentTask() const;

	bool IsCurrentTaskExists(void) const;

	/** Add completed task */
	void AddCompletedTask(const Task::CompletedTaskInfo& completedTask);

	unsigned int GetUid(void) const;

	void UpdateToTime(Utils::GameTime time);

	float GetConstructionProgress(Utils::GameTime time) const;
	float GetDestructionProgress(Utils::GameTime time) const;

	float CalcConnectivity() const;
	int CalcDistanceToTheRootCell() const;

	/** Set parent for the cell */
	void SetParent(Cell *cell);

private:

	/** Check all values and write warnings to the log */
	void _CheckValues() const;

	/** All usual info about the cell */
	Info _info;

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
