#ifndef TOWN_H
#define TOWN_H

#include "MapProjector.h"

class Town
{
public:
	struct Info
	{
		std::string name;
		std::string desc;
		int population;
		float rank;
		cocos2d::CCRect rect;
	};

	typedef std::shared_ptr<Town> Ptr;

public:
	Town(const Info &info);

	Info GetInfo(void) const;

	const std::string GetName(void) const;
	const std::string GetDesc(void) const;

	Point GetLocation(void) const;

	Town& operator= (const Town & other);

private:
	const std::string _name;
	const std::string _desc;
	const int _population;
	const cocos2d::CCRect _rect;
};

#endif // TONW_H