#ifndef REGION_H
#define REGION_H

#include "ArbitraryHull.h"

#include <memory>

class Region
{
public:
	typedef std::shared_ptr<Region> Ptr;
	typedef std::vector<ArbitraryHull> HullsArray;

	const ArbitraryHull& GetHullByIndex(int index) const;
	const HullsArray& GetHullsArray(void) const;
	
	void AddHull(ArbitraryHull &hull);
	void EditHull(int index, const ArbitraryHull &hull);

private:
	HullsArray _hulls;

	float _population;

	std::string shortDescription;
};

#endif // REGION_H