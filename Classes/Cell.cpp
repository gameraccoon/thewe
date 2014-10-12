#include "Cell.h"

#include "World.h"
#include "Log.h"

Cell::Cell(const Info &info)
	: _info(info)
	, _state(READY)
	, _currentTask()
	, _uid(World::Instance().GetNewUid())
{
	if (_info.constructionDuration <= 0)
	{
		Log::Instance().writeError("Construction duration less or equals than zero");
	}

	info.town.lock()->SetCellPresented(true);

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

void Cell::UpdateToTime(Utils::GameTime time)
{
	if (_info.state == CONSTRUCTION && time > _info.constructionBegin + _info.constructionDuration)
	{
		_info.state = READY;
	}

	_CheckValues();
}

void Cell::_UpdateCash(float deltalime)
{
}

void Cell::_UpdateMorale(float deltatime)
{
}

void Cell::_UpdateContentment(float deltatime)
{
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

float Cell::GetConstructionProgress(Utils::GameTime time) const
{
	return 1.0f - ((float)((_info.constructionBegin + _info.constructionDuration) - time)) / ((float)_info.constructionDuration);
}
