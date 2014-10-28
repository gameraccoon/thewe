#include "Cell.h"

#include "World.h"
#include "Log.h"
#include "GameInfo.h"
#include "MessageManager.h"

Cell::Cell(const Info &info, bool root)
	: _info(info)
	, _currentTask()
	, _isRoot(root)
	, _uid(World::Instance().GetNewUid())
{
	info.town.lock()->SetCellPresented(true);

	_CheckValues();
}

Cell::~Cell(void)
{
}

Cell::Ptr Cell::Create(const Info &info, bool root)
{
	return std::make_shared<Cell>(info, root);
}

void Cell::AddChild(Cell::Ptr cell)
{
	_childCells.push_back(cell);
	cell->SetParent(this);
}

void Cell::RemoveChild(Cell::Ptr cell)
{
	auto iterator = _childCells.begin(), iEnd = _childCells.end();
	while (iterator != iEnd)
	{
		const Cell::Ptr currentPart = (*iterator);	

		if (currentPart == cell)
		{
			cell->SetParent(nullptr);
			_childCells.erase(iterator);
			return;
		}

		iterator++;
	}
}

void Cell::RemoveAllChildren(void)
{
	for (Cell::Ptr child : _childCells) {
		child->SetParent(nullptr);
	}
	_childCells.clear();
}

void Cell::BeginDestruction(void)
{
	// ToDo: make some checks
	_info.state = State::DESTRUCTION;
	_info.stateBegin = Utils::GetGameTime();
	_info.stateDuration = GameInfo::Instance().GetFloat("CELL_DESTRUCTION_TIME");
}

void Cell::BeginAutonomy(void)
{
	// ToDo: make some checks
	_info.state = State::AUTONOMY;
	_info.stateBegin = Utils::GetGameTime();
	_info.stateDuration = GameInfo::Instance().GetFloat("CELL_AUTONOMY_LIFE_TIME");
}

void Cell::SetParent(Cell* cell)
{
	_info.parent = cell;
}

const std::vector<Cell::Ptr>& Cell::GetChildren() const
{
	return _childCells;
}

const Cell* Cell::GetParent() const
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
		Cell::Ptr ptr = World::Instance().GetCellsNetwork().GetCellByInfo(_info);
		World::Instance().RemoveCellFromInvestigation(ptr);
		World::Instance().GetCellsNetwork().RemoveCell(ptr);
		MessageManager::Instance().PutMessage(Message("DeleteCellWidget", _uid));
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

bool Cell::IsRoot(void) const
{
	return _isRoot;
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
	if (_info.cash < 0.0f)
	{
		Log::Instance().writeWarning("Negative cash value");
	}

	if (_info.membersCount <= 0)
	{
		Log::Instance().writeWarning("Wrong members count");
	}

	if (_info.techUnitsCount < 0)
	{
		Log::Instance().writeWarning("Wrong techUnitsCount value");
	}

	if (_info.ratsCount < 0)
	{
		Log::Instance().writeWarning("Wrong ratsCount value");
	}

	if (_info.experience < 0)
	{
		Log::Instance().writeWarning("Wrong experience value");
	}

	if (_info.morale < 0.0f || 1.0f < _info.morale)
	{
		Log::Instance().writeWarning("Wrong morale value");
	}
	
	if (_info.devotion < 0.0f || 1.0f < _info.devotion)
	{
		Log::Instance().writeWarning("Wrong devotion value");
	}

	if (_info.fame < 0.0f || 1.0f < _info.fame)
	{
		Log::Instance().writeWarning("Wrong fame value");
	}

	if (_info.townHeartPounding < 0.0f || 1.0f < _info.townHeartPounding)
	{
		Log::Instance().writeWarning("Wrong townHeartPounding value");
	}

	if (_info.townInfluence < 0.0f || 1.0f < _info.townInfluence)
	{
		Log::Instance().writeWarning("Wrong townInfluence value");
	}

	if (_info.townWelfare < 0.0f || 1.0f < _info.townWelfare)
	{
		Log::Instance().writeWarning("Wrong townWelfare value");
	}

	if (_info.town.expired())
	{
		Log::Instance().writeWarning("Dead reference to town");
	}

	if (IsInTemporaryState() && _info.stateDuration <= 0)
	{
		Log::Instance().writeError("State duration less or equals than zero");
	}
}

float Cell::GetStateProgress(Utils::GameTime time) const
{
	return 1.0f - ((float)((_info.stateBegin + _info.stateDuration) - time)) / ((float)_info.stateDuration);
}

float Cell::CalcConnectivity() const
{
	if (_info.parent != nullptr)
	{
		return _info.parent->CalcConnectivity() / 2.0f + _childCells.size() + 1.0f;
	}
	else
	{
		return _childCells.size();
	}
}

int Cell::CalcDistanceToTheRootCell() const
{
	if (_info.parent != nullptr)
	{
		return _info.parent->CalcDistanceToTheRootCell() + 1;
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
