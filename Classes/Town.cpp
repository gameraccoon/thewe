#include "Town.h"

#include "Log.h"

Town::Town(const Info &info)
	: _region(info.region)
	, _name(info.name)
	, _desc(info.desc)
	, _population(info.population)
	, _rank(info.rank)
	, _location(info.location)
	, _spriteScale(info.spriteScale)
{
	_CheckValues();
}

Town::Ptr Town::Create(const Info &info)
{
	return std::make_shared<Town>(info);
}

Town::Info Town::GetInfo(void) const
{
	Info info;

	info.region = _region;
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

Vector2 Town::GetLocation(void) const
{
	return _location;
}

float Town::GetSpriteScale(void) const
{
	return _spriteScale;
}

Region::WeakPtr Town::GetRegion(void) const
{
	return _region;
}

Town& Town::operator= (const Town & other)
{
	return *this;
}

void Town::_CheckValues() const
{
	if (_population < 0.0f)
	{
		Log::Instance().writeWarning("Wrong population value");
	}

	if (_region.expired())
	{
		Log::Instance().writeWarning("Dead reference to region");
	}

	if (_spriteScale < 0.0f)
	{
		Log::Instance().writeWarning("Negative sprite scale");
	}
}