#include "Region.h"

const ArbitraryHull& Region::GetHullByIndex(int index) const
{
	return _hulls.at(index);
}

const Region::HullsArray& Region::GetHullsArray(void) const
{
	return _hulls;
}

void Region::AddHull(ArbitraryHull &hull)
{
	_hulls.push_back(hull);
}

void Region::EditHull(int index, const ArbitraryHull &hull)
{
	_hulls.at(index) = hull;
}