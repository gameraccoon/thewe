#ifndef REGION_H
#define REGION_H

#include "ArbitraryHull.h"

#include <memory>

class Region
{
public:
	typedef std::shared_ptr<Region> Ptr;
	ArbitraryHull GetHull() const;
	void SetHull(const ArbitraryHull& newHull);
private:
	ArbitraryHull _hull;
};

#endif // REGION_H