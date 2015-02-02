#include "Cell.h"

#include "World.h"
#include "Log.h"
#include "GameInfo.h"
#include "MessageManager.h"

Cell::Cell(Town::WeakPtr town)
	: town(town)
	, _currentTask()
	, _uid(World::Instance().GetNewUid())
	, cash(0)
	, location(Vector2(0.0f, 0.0f))
{
	town.lock()->SetCellPresented(true);

	_CheckValues();
}

Cell::Cell(Town::WeakPtr town, unsigned int uid)
	: town(town)
	, _currentTask()
	, _uid(uid)
	, cash(0)
	, location(Vector2(0.0f, 0.0f))
{
	town.lock()->SetCellPresented(true);

	_CheckValues();
}

Cell::~Cell(void)
{
}

Cell::Ptr Cell::Create(Town::WeakPtr town)
{
	return std::make_shared<Cell>(town);
}

void Cell::AddChild(Cell::WeakPtr cell)
{
	Cell::Ptr lockedCell = cell.lock();
	if (lockedCell)
	{
		_childCells.push_back(cell);
		lockedCell->SetParent(World::Instance().GetCellsNetwork().GetCellByUid(_uid));
	}
}

void Cell::RemoveChild(Cell::WeakPtr cell)
{
	Cell::Ptr cellPtr = cell.lock();

	auto iterator = _childCells.begin(), iEnd = _childCells.end();
	while (iterator != iEnd)
	{
		const Cell::Ptr currentPart = (*iterator).lock();

		if (currentPart == cellPtr)
		{
			cellPtr->SetParent(Cell::Ptr());
			_childCells.erase(iterator);
			return;
		}

		iterator++;
	}
}

void Cell::RemoveAllChildren(void)
{
	for (Cell::WeakPtr child : _childCells)
	{
		Cell::Ptr childPtr = child.lock();
		if (childPtr)
		{
			childPtr->SetParent(Cell::Ptr());
		}
		else
		{
			WRITE_ERR("Dead link to cell child");
		}
	}
	_childCells.clear();
}

void Cell::BeginDestruction(void)
{
	// ToDo: make some checks
	state = State::DESTRUCTION;
	stateBegin = Utils::GetGameTime();
	stateDuration = GameInfo::Instance().GetTime("CELL_DESTRUCTION_TIME");
}

void Cell::BeginAutonomy(void)
{
	// ToDo: make some checks
	state = State::AUTONOMY;
	stateBegin = Utils::GetGameTime();
	stateDuration = GameInfo::Instance().GetTime("CELL_AUTONOMY_LIFE_TIME");
}

void Cell::ReturnToNormalState(void)
{
	state = State::READY;
}

void Cell::SetParent(Cell::WeakPtr cell)
{
	parent = cell;
}

const std::vector<Cell::WeakPtr>& Cell::GetChildren() const
{
	return _childCells;
}

Cell::WeakPtr Cell::GetParent() const
{
	return parent;
}

void Cell::UpdateToTime(Utils::GameTime time)
{
	if (state == State::CONSTRUCTION && time > stateBegin + stateDuration)
	{
		state = State::READY;
	}
	else if (state == State::DESTRUCTION && time > stateBegin + stateDuration)
	{
		Cell::Ptr ptr = World::Instance().GetCellsNetwork().GetCellByUid(_uid);
		World::Instance().GetCellsNetwork().RemoveCell(ptr);

		Message message("DeleteCellWidget");
		message.variables.SetInt("UID", _uid);
		MessageManager::Instance().PutMessage(message);
	}
	else if (state == State::AUTONOMY && time > stateBegin + stateDuration)
	{
		BeginDestruction();
	}

	_CheckValues();
}

void Cell::AddCurrentTask(Task::WeakPtr currentTask)
{
	_currentTask = currentTask;
}

Task::WeakPtr Cell::getCurrentTask() const
{
	return _currentTask;
}

bool Cell::IsCurrentTaskExists(void) const
{
	return _currentTask.use_count() > 0;
}

bool Cell::IsState(State state) const
{
	return state == state;
}

void Cell::AddCompletedTask(const Task::CompletedTaskInfo& completedTask)
{
	_completedTasks.push_back(completedTask);
}

unsigned int Cell::GetUid(void) const
{
	return _uid;
}

void Cell::_CheckValues() const
{
	WARN_IF(cash < 0.0f, "Negative cash value");
	WARN_IF(experience < 0.f, "Wrong experience value");
	WARN_IF(town.expired(), "Dead reference to town");
}

float Cell::GetStateProgress(Utils::GameTime time) const
{
	return 1.0f - ((float)((stateBegin + stateDuration) - time)) / ((float)stateDuration);
}

float Cell::CalcConnectivity() const
{
	Cell::Ptr par = parent.lock();

	if (par)
	{
		return par->CalcConnectivity() / 2.0f + _childCells.size() + 1.0f;
	}
	else
	{
		return _childCells.size();
	}
}

int Cell::CalcDistanceToTheRootCell() const
{
	Cell::Ptr par = parent.lock();
	if (par)
	{
		return par->CalcDistanceToTheRootCell() + 1;
	}
	else
	{
		return 0;
	}
}

bool Cell::IsInTemporaryState() const
{
	return (state != State::READY && state != State::ARRESTED);
}

bool Cell::IsReadyToCreateSpinoff() const
{
	bool isCashEnough = cash >= GameInfo::Instance().GetInt("CELL_SPINOFF_CASH_PRICE");
	return isCashEnough;
}

void Cell::AddResource(const Resource& resource)
{
	if (resources.find(resource.name) != resources.end())
	{
		resources.at(resource.name).count += resource.count;
	}
	else
	{
		resources.insert(std::pair<const std::string, Resource>(resource.name, resource));
	}
}

void Cell::AddReward(const Resource::Vector& reward)
{
	for (const auto& resource : reward)
	{
		if (resource.name == "money")
		{
			cash += resource.count;
		}
		else
		{
			AddResource(resource);
		}
	}
}

const Resource::Map&Cell::GetResources() const
{
	return resources;
}

int Cell::GetExperience(void) const
{
	return experience;
}

void Cell::SetExperience(int newExp)
{
	World &world = World::Instance();
	int levelBefore = world.GetLevelFromExperience(experience);
	int levelAfter = world.GetLevelFromExperience(newExp);
	if (levelBefore < levelAfter)
	{
		Message message("CellLevelUp");
		message.variables.SetInt("levelBefore", levelBefore);
		message.variables.SetInt("levelAfter", levelAfter);
		message.variables.SetInt("cellUid", (int)GetUid());
		MessageManager::Instance().PutMessage(message);
	}

	if (levelAfter < levelBefore)
	{
		WRITE_WARN("Decrease cell level");
	}

	experience = newExp;
}

Vector2 Cell::GetLocation() const
{
	return location;
}

void Cell::SetLocation(const Vector2& newLocation)
{
	location = newLocation;
}

Cell::State Cell::GetState() const
{
	return state;
}

Utils::GameTime Cell::GetStateBegin() const
{
	return stateBegin;
}

Utils::GameTime Cell::GetStateDuration() const
{
	return stateDuration;
}

void Cell::SetState(Cell::State newState, Utils::GameTime beginTime, Utils::GameTime duration)
{
	state = newState;
	stateBegin = beginTime;
	stateDuration = duration;
}

int Cell::GetMembersCount() const
{
	return (int)members.size();
}

int Cell::GetCash() const
{
	return cash;
}

void Cell::SetCash(int newCashValue)
{
	cash = newCashValue;
}

Town::WeakPtr Cell::GetTown() const
{
	return town;
}
