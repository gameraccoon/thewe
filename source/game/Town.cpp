#include "Town.h"

Town::Town(const Info &info)
	: _name(info.name)
	, _desc(info.desc)
	, _population(info.population)
	, _rank(info.rank)
	, _location(info.location)
	, _spriteScale(info.spriteScale)
{
}

Town::Info Town::GetInfo(void) const
{
	Info info;

	info.name = _name;
	info.desc = _desc;
	info.population = _population;
	info.rank = _rank;
	info.location = _location;
	info.spriteScale = _spriteScale;

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
	return _location;
}

float Town::GetSpriteScale(void) const
{
	return _spriteScale;
}

Town& Town::operator= (const Town & other)
{
	return *this;
}