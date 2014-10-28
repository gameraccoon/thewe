#ifndef CELL_H
#define CELL_H

#include <memory>

#include "Town.h"
#include "Vector2.h"
#include "Task.h"

class Cell
{
public:
	enum class State
	{
		READY = 0,
		CONSTRUCTION,
		ARRESTED,
		AUTONOMY,
		DESTRUCTION
	};

	enum class Specialization
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

		// current process (construction, destruction, autonomy)
		Utils::GameTime stateBegin;
		Utils::GameTime stateDuration;
	};

	typedef std::shared_ptr<Cell> Ptr;
	typedef std::weak_ptr<Cell> WeakPtr;

public:
	Cell(const Info &info, bool root);
	~Cell(void);

	/**
	 * Create Cell and return a smart ptr
	 */
	static Ptr Create(const Info &info, bool root = false);
	
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

	void BeginDestruction(void);
	void BeginAutonomy(void);

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
	bool IsState(State state) const;
	bool IsRoot(void) const;

	/** Add completed task */
	void AddCompletedTask(const Task::CompletedTaskInfo& completedTask);

	unsigned int GetUid(void) const;

	void UpdateToTime(Utils::GameTime time);

	float GetStateProgress(Utils::GameTime time) const;

	float CalcConnectivity() const;
	int CalcDistanceToTheRootCell() const;

	bool IsInTemporaryState() const;

private:
	/** Set parent for the cell */
	void SetParent(Cell *cell);

	/** Check all values and write warnings to the log */
	void _CheckValues() const;

	/** All usual info about the cell */
	Info _info;

	bool _isRoot;

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
