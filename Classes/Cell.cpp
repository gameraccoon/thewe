#include "Cell.h"

#include "World.h"
#include "Log.h"
#include "GameInfo.h"
#include "MessageManager.h"

Cell::Cell(const Info &info)
	: _info(info)
	, _currentTask()
	, _uid(World::Instance().GetNewUid())
{
	info.town.lock()->SetCellPresented(true);

	_CheckValues();
}

Cell::Cell(const Info &info, unsigned int uid)
	: _info(info)
	, _currentTask()
	, _uid(uid)
{
	info.town.lock()->SetCellPresented(true);

	_CheckValues();
}

Cell::~Cell(void)
{
}

Cell::Ptr Cell::Create(const Info &info)
{
	return std::make_shared<Cell>(info);
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
	_info.state = State::DESTRUCTION;
	_info.stateBegin = Utils::GetGameTime();
	_info.stateDuration = GameInfo::Instance().GetTime("CELL_DESTRUCTION_TIME");
}

void Cell::BeginAutonomy(void)
{
	// ToDo: make some checks
	_info.state = State::AUTONOMY;
	_info.stateBegin = Utils::GetGameTime();
	_info.stateDuration = GameInfo::Instance().GetTime("CELL_AUTONOMY_LIFE_TIME");
}

void Cell::ReturnToNormalState(void)
{
	_info.state = State::READY;
}

void Cell::SetParent(Cell::WeakPtr cell)
{
	_info.parent = cell;
}

const std::vector<Cell::WeakPtr>& Cell::GetChildren() const
{
	return _childCells;
}

Cell::WeakPtr Cell::GetParent() const
{
	return _info.parent;
}

Cell::Info& Cell::GetInfo(void)
{
	return _info;
}

void Cell::UpdateToTime(Utils::GameTime time)
{
	if (_info.state == State::CONSTRUCTION && time > _info.stateBegin + _info.stateDuration)
	{
		_info.state = State::READY;
	}
	else if (_info.state == State::DESTRUCTION && time > _info.stateBegin + _info.stateDuration)
	{
		Cell::Ptr ptr = World::Instance().GetCellsNetwork().GetCellByUid(_uid);
		World::Instance().GetCellsNetwork().RemoveCell(ptr);

		Message message("DeleteCellWidget");
		message.variables.SetInt("UID", _uid);
		MessageManager::Instance().PutMessage(message);
	}
	else if (_info.state == State::AUTONOMY && time > _info.stateBegin + _info.stateDuration)
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
	return _info.state == state;
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
	WARN_IF(_info.cash < 0.0f, "Negative cash value");
	WARN_IF(_info.membersCount <= 0, "Wrong members count");
	WARN_IF(_info.techUnitsCount < 0, "Wrong techUnitsCount value");
	WARN_IF(_info.ratsCount < 0, "Wrong ratsCount value");
	WARN_IF(_info.experience < 0.f, "Wrong experience value");
	WARN_IF(_info.morale < 0.0f || 1.0f < _info.morale, "Wrong morale value");
	WARN_IF(_info.devotion < 0.0f || 1.0f < _info.devotion, "Wrong devotion value");
	WARN_IF(_info.fame < 0.0f || 1.0f < _info.fame, "Wrong fame value");
	WARN_IF(_info.townHeartPounding < 0.0f || 1.0f < _info.townHeartPounding, "Wrong townHeartPounding value");
	WARN_IF(_info.townInfluence < 0.0f || 1.0f < _info.townInfluence, "Wrong townInfluence value");
	WARN_IF(_info.townWelfare < 0.0f || 1.0f < _info.townWelfare, "Wrong townWelfare value");
	WARN_IF(_info.town.expired(), "Dead reference to town");
	WARN_IF(IsInTemporaryState() && _info.stateDuration <= 0, "State duration less or equals than zero");
}

float Cell::GetStateProgress(Utils::GameTime time) const
{
	return 1.0f - ((float)((_info.stateBegin + _info.stateDuration) - time)) / ((float)_info.stateDuration);
}

float Cell::CalcConnectivity() const
{
	Cell::Ptr parent = _info.parent.lock();

	if (parent)
	{
		return parent->CalcConnectivity() / 2.0f + _childCells.size() + 1.0f;
	}
	else
	{
		return _childCells.size();
	}
}

int Cell::CalcDistanceToTheRootCell() const
{
	Cell::Ptr parent = _info.parent.lock();
	if (parent)
	{
		return parent->CalcDistanceToTheRootCell() + 1;
	}
	else
	{
		return 0;
	}
}

bool Cell::IsInTemporaryState() const
{
	return (_info.state != State::READY && _info.state != State::ARRESTED);
}

bool Cell::IsReadyToCreateSpinoff() const
{
	bool isMembersEnough = _info.membersCount >= GameInfo::Instance().GetInt("CELL_SPINOFF_MEMBERS_PRICE") * 2;
	bool isCashEnough = _info.cash >= GameInfo::Instance().GetInt("CELL_SPINOFF_CASH_PRICE");
	return isMembersEnough && isCashEnough;
}

void Cell::AddResource(const Resource& resource)
{
	if (_info.resources.find(resource.name) != _info.resources.end())
	{
		_info.resources.at(resource.name).count += resource.count;
	}
	else
	{
		_info.resources.insert(std::pair<const std::string, Resource>(resource.name, resource));
	}
}

void Cell::AddReward(const Resource::Vector& reward)
{
	for (const auto& resource : reward)
	{
		if (resource.name == "money")
		{
			_info.cash += resource.count;
		}
		else if (resource.name == "people")
		{
			_info.membersCount += resource.count;
		}
		else
		{
			AddResource(resource);
		}
	}
}

int Cell::GetExp(void) const
{
	return _info.experience;
}

void Cell::SetExp(int newExp)
{
	World &world = World::Instance();
	int levelBefore = world.GetLevelFromExperience(_info.experience);
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

	_info.experience = newExp;
}
