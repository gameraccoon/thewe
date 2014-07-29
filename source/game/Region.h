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
	typedef std::vector<ArbitraryHull> HullsArray;

public:
	Region(void);
	Region(const Info &info);

	void Init(const Info &info);

	const ArbitraryHull& GetHullByIndex(int index) const;
	const HullsArray& GetHullsArray(void) const;
	
	void AddHull(ArbitraryHull &hull);
	void EditHull(int index, const ArbitraryHull &hull);

	Info GetInfo(void) const;

private:
	HullsArray _hulls;

	float _population;

	std::string _name;
	std::string _shortDescription;
};

#endif // REGION_H