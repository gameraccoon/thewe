#include "Cell.h"

Cell::Cell(Point location)
	: _parentCell(nullptr)
{
	_worldLocation = location;
}

Cell::Cell(const Info &info)
	: _parentCell(info.parent)
	, _region(info.region)
	, _worldLocation(info.location)
	, _cash(info.cach)
	, _moralValue(info.morale)
	, _contentment(info.contentment)
	, _membersCount(info.membersNum)
{
}

int Cell::GetMembersCount() const
{
	return _membersCount;
}

float Cell::GetCash() const
{
	return _cash;
}

float Cell::GetMoralValue() const
{
	return _moralValue;
}

float Cell::GetContentment() const
{
	return _contentment;
}

Point Cell::GetLocation()
{
	return _worldLocation;
}

Cell* Cell::GetParent() const
{
	return _parentCell;
}

void Cell::_SetParent(Cell* cell)
{
	_parentCell = cell;
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

const std::vector<Cell::Ptr>& Cell::GetChildren()
{
	return _childCells;
}

Cell::Info Cell::GetInfo(void) const
{
	Info info;

	info.parent = _parentCell;
	info.region = _region;
	info.location = _worldLocation;
	info.cach = _cash;
	info.morale = _moralValue;
	info.contentment = _contentment;
	info.membersNum = _membersCount;

	return info;
}