#include "Region.h"

Region::Region(void)
	: _population(0.0f)
	, _name("Unnamed")
	, _shortDescription("None")
{
}

Region::Region(const Info &info)
{
	Init(info);
}

void Region::Init(const Info &info)
{
	_population = info.population;
	_name = info.name;
	_shortDescription = info.desc;
}

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

Region::Info Region::GetInfo(void) const
{
	Info info;
	info.population = _population;
	info.name = _name;
	info.desc = _shortDescription;

	return info;
}