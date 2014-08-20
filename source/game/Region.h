#ifndef REGION_H
#define REGION_H

#include "ArbitraryHull.h"

#include <memory>

class Region
{
public:
	struct Info
	{
		float population;
		std::string name;
		std::string desc;
	};

	typedef std::shared_ptr<Region> Ptr;
	typedef std::weak_ptr<Region> WeakPtr;
	typedef std::vector<ArbitraryHull> HullsArray;

public:
	Region(const Info &info);

	const ArbitraryHull& GetHullByIndex(int index) const;
	const HullsArray& GetHullsArray(void) const;
	
	void AddHull(ArbitraryHull &hull);
	void EditHull(int index, const ArbitraryHull &hull);

	Info GetInfo(void) const;

private:
	void _Init(const Info &info);
	HullsArray _hulls;

	float _population;

	std::string _name;
	std::string _shortDescription;
};

#endif // REGION_H