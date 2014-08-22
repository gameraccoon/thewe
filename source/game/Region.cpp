#include "Region.h"

#include "Log.h"

Region::Region(const Info &info)
{
	_Init(info);
	_CheckValues();
}

void Region::_Init(const Info &info)
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

void Region::_CheckValues() const
{
	if (_population < 0.0f)
	{
		Log::Instance().writeWarning("Wrong population value");
	}
}