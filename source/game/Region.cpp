#include "Region.h"

ArbitraryHull Region::GetHull() const
{
	return _hull;
}

void Region::SetHull(const ArbitraryHull& newHull)
{
	_hull = newHull;
}