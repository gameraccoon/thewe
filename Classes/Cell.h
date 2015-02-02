#ifndef CELL_H
#define CELL_H

#include <memory>

#include "Town.h"
#include "Vector2.h"
#include "Task.h"
#include "Resources.h"
#include "Member.h"

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

public:
	Cell(Town::WeakPtr town);
	~Cell(void);

	/**
	 * Create Cell and return a smart ptr
	 */
	static Ptr Create(Town::WeakPtr town);

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

	void AddResource(const Resource& resource);
	void AddReward(const Resource::Vector& reward);
	const Resource::Map& GetResources() const;

	int GetExperience(void) const;
	void SetExperience(int newExp);

	Vector2 GetLocation() const;
	void SetLocation(const Vector2& newLocation);

	State GetState() const;
	Utils::GameTime GetStateBegin() const;
	Utils::GameTime GetStateDuration() const;
	void SetState(State newState, Utils::GameTime beginTime = 0, Utils::GameTime duration = 0);

	int GetMembersCount() const;
	int GetCash() const;
	void SetCash(int newCashValue);

	Town::WeakPtr GetTown() const;

private:
	Cell(Town::WeakPtr town, unsigned int uid);
	/** Set parent for the cell */
	void SetParent(Cell::WeakPtr cell);

	/** Check all values and write warnings to the log */
	void _CheckValues() const;

private:
	unsigned int _uid;

	/** Child cells */
	std::vector<Cell::WeakPtr> _childCells;

	/** Task that runned now */
	Task::WeakPtr _currentTask;

	/** List of recently completed task */
	std::vector<Task::CompletedTaskInfo> _completedTasks;

	Cell::WeakPtr parent;
	Town::WeakPtr town;
	Vector2 location;

	int cash;
	int experience;

	Resource::Map resources;
	std::vector<Member> members;

	// current state (construction, destruction, autonomy)
	State state;
	Utils::GameTime stateBegin;
	Utils::GameTime stateDuration;

	friend class GameSavesManager;
};

#endif // CELL_H
