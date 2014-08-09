#include "Town.h"

Town::Town(const Info &info)
	: _name(info.name)
	, _desc(info.desc)
	, _population(info.population)
	, _rect(info.rect)
{
}

Town::Info Town::GetInfo(void) const
{
	Info info;

	info.name = _name;
	info.desc = _desc;
	info.population = _population;
	info.rect = _rect;
	
	return info;
}

const std::string Town::GetName(void) const
{
	return _name;
}

const std::string Town::GetDesc(void) const
{
	return _desc;
}

Point Town::GetLocation(void) const
{
	return _rect.origin;
}

Town& Town::operator= (const Town & other)
{
	return *this;
}