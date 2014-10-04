#include "Cell.h"

#include "World.h"
#include "Log.h"

Cell::Cell(const Info &info)
	: _info(info)
	, _state(READY)
	, _currentTask()
	, _constructionTime(0.0f)
	, _uid(World::Instance().GetNewUid())
{
	_CheckValues();
}

Cell::Ptr Cell::Create(const Info &info)
{
	return std::make_shared<Cell>(info);
}

void Cell::AddChild(Cell::Ptr cell)
{
	_childCells.push_back(cell);
	cell->_SetParent(this);
}

void Cell::RemoveChild(Cell::Ptr cell)
{
	auto iterator = _childCells.begin(), iEnd = _childCells.end();
	while (iterator != iEnd)
	{
		const Cell::Ptr currentPart = (*iterator);	

		if (currentPart == cell)
		{
			cell->_SetParent(nullptr);
			_childCells.erase(iterator);
			return;
		}

		iterator++;
	}
}

void Cell::_SetParent(Cell* cell)
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

Cell::State& Cell::GetState(void)
{
	return _state;
}

void Cell::SetConstructionTime(float seconds)
{
	_constructionTime = seconds;
}

float Cell::GetConstructionTime(void) const
{
	return _constructionTime;
}

void Cell::SwitchState(Cell::State state)
{
	_state = state;
}

void Cell::Update(float deltatime)
{
	/*
	_UpdateCash(deltatime);
	_UpdateMorale(deltatime);
	_UpdateContentment(deltatime);
	*/

	_CheckValues();
}

void Cell::_UpdateCash(float deltalime)
{
	float cashDelta = deltalime * _info.membersCount;
	_info.cash = _info.cash > cashDelta ? _info.cash - cashDelta : 0.0f;
}

void Cell::_UpdateMorale(float deltatime)
{
	//_info.morale = 
}

void Cell::_UpdateContentment(float deltatime)
{
	const float halfDelta = 0.3f;
	_info.contentment = std::min(halfDelta + (_info.cash / _info.membersCount) * 30.0f, 1 - halfDelta) + sinf(World::Instance().GetWorldTime()) * halfDelta;
}

void Cell::AddCurrentTask(Task::WeakPtr currentTask)
{
	_currentTask = currentTask;
}

Task::WeakPtr Cell::getCurrentTask() const
{
	return _currentTask;
}

bool Cell::IsCurrentTaskPresented(void) const
{
	return _currentTask.use_count() > 0;
}

void Cell::AddCompletedTask(const Task::CompletedTaskInfo& completedTask)
{
	_completedTasks.push_back(completedTask);
}

unsigned int Cell::GetUid(void) const
{
	return _uid;
}

float Cell::GetCash()
{
	return _info.cash;
}

void Cell::_CheckValues() const
{
	if (_info.cash < 0.0f)
	{
		Log::Instance().writeWarning("Negative cash value");
	}
	
	if (_info.contentment < 0.0f || 1.0f < _info.contentment)
	{
		Log::Instance().writeWarning("Wrong value of the containment");
	}

	if (_info.membersCount <= 0)
	{
		Log::Instance().writeWarning("Wrong members count");
	}

	if (_info.morale < 0.0f || 1.0f < _info.morale)
	{
		Log::Instance().writeWarning("Wrong morale value");
	}

	if (_info.town.expired())
	{
		Log::Instance().writeWarning("Dead reference to town");
	}
}
