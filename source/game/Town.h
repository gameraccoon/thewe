#ifndef TOWN_H
#define TOWN_H

#include "MapProjector.h"
#include "Region.h"

class Town
{
public:
	struct Info
	{
		Region::Ptr region;
		std::string name;
		std::string desc;
		float population;
		float rank;
		float spriteScale;
		Point location;
	};

	typedef std::shared_ptr<Town> Ptr;

public:
	Town(const Info &info);

	Info GetInfo(void) const;

	const std::string GetName(void) const;
	const std::string GetDesc(void) const;

	Point GetLocation(void) const;
	float GetSpriteScale(void) const;
	Region::Ptr GetRegion(void) const;

	Town& operator= (const Town & other);

private:
	Region::Ptr _region;

	const std::string _name;
	const std::string _desc;
	const float _population;
	const float _rank;
	const float _spriteScale;
	const Point _location;
};

#endif // TONW_H