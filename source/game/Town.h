#ifndef TOWN_H
#define TOWN_H

#include "MapProjector.h"
#include "Region.h"

class Town
{
public:
	struct Info
	{
		Region::WeakPtr region;
		std::string name;
		std::string desc;
		float population;
		float rank;
		float spriteScale;
		Point location;
	};

	typedef std::shared_ptr<Town> Ptr;
	typedef std::weak_ptr<Town> WeakPtr;
public:
	Town(const Info &info);
	static Ptr Create(const Info &info);

	Info GetInfo(void) const;

	const std::string GetName(void) const;
	const std::string GetDesc(void) const;

	Point GetLocation(void) const;
	float GetSpriteScale(void) const;
	Region::WeakPtr GetRegion(void) const;

	void _CheckValues() const;

	Town& operator= (const Town & other);

private:
	Region::WeakPtr _region;

	const std::string _name;
	const std::string _desc;
	const float _population;
	const float _rank;
	const float _spriteScale;
	const Point _location;
};

#endif // TONW_H