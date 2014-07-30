#include "Cell.h"

Cell::Cell(Point location)
{
	_worldLocation = location;
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