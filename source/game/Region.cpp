#include "Region.h"

ConvexHullTest Region::GetHull() const
{
	return _hull;
}

void Region::SetHull(const ConvexHullTest& newHull)
{
	_hull = newHull;
}