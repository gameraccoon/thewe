#include "Cell.h"

#include "World.h"

Cell::Cell(const Info &info)
	: _info(info)
	, _currentTask()
{
}

Cell::Ptr Cell::Create(const Info &info)
{
	return std::make_shared<Cell>(Cell(info));
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

const std::vector<Cell::Ptr>& Cell::GetChildrens() const
{
	return _childCells;
}

const Cell::Info& Cell::GetInfo(void) const
{
	return _info;
}

void Cell::Update(float deltatime)
{
	// пересчитать наличные
	_UpdateCash(deltatime);
	_UpdateMorale(deltatime);
	_UpdateContentment(deltatime);
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

void Cell::AddCompletedTask(const Task::CompletedTaskInfo& completedTask)
{
	_completedTasks.push_back(completedTask);
}