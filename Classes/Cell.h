#ifndef CELL_H
#define CELL_H

#include <memory>

#include "Town.h"
#include "Vector2.h"
#include "Task.h"

class Cell
{
public:
	typedef std::shared_ptr<Cell> Ptr;
	typedef std::weak_ptr<Cell> WeakPtr;

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
		Cell::WeakPtr parent;
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
	void AddChild(Cell::WeakPtr cell);

	/**
	 * Removes child cell
	 * Set child's parent to null
	 */
	void RemoveChild(Cell::WeakPtr cell);
	void RemoveAllChildren(void);

	void BeginDestruction(void);
	void BeginAutonomy(void);
	void ReturnToNormalState(void);

	/** Returns all child cells */
	const std::vector<Cell::WeakPtr>& GetChildren() const;

	WeakPtr GetParent() const;

	/**
	* Returns information about the cell
	*/
	Info& GetInfo(void);

	void AddCurrentTask(Task::WeakPtr currentTask);
	Task::WeakPtr getCurrentTask() const;

	bool IsCurrentTaskExists(void) const;
	bool IsState(State state) const;

	/** Add completed task */
	void AddCompletedTask(const Task::CompletedTaskInfo& completedTask);

	unsigned int GetUid(void) const;

	void UpdateToTime(Utils::GameTime time);

	float GetStateProgress(Utils::GameTime time) const;

	float CalcConnectivity() const;
	int CalcDistanceToTheRootCell() const;

	bool IsInTemporaryState() const;

	bool IsReadyToCreateSpinoff() const;

private:
	Cell(const Info &info, unsigned int uid);
	/** Set parent for the cell */
	void SetParent(Cell::WeakPtr cell);

	/** Check all values and write warnings to the log */
	void _CheckValues() const;

private:
	/** All usual info about the cell */
	Info _info;

	unsigned int _uid;

	/** Child cells */
	std::vector<Cell::WeakPtr> _childCells;

	/** Task that runned now */
	Task::WeakPtr _currentTask;

	/** List of recently completed task */
	std::vector<Task::CompletedTaskInfo> _completedTasks;

	/** Geographic position on the map */
	Vector2 _worldLocation;

	friend class GameSavesManager;
};

#endif // CELL_H
