#ifndef REGION_H
#define REGION_H

#include "ConvexHullTest.h"

#include <memory>

class Region
{
public:
	typedef std::shared_ptr<Region> Ptr;
	ConvexHullTest GetHull() const;
	void SetHull(const ConvexHullTest& newHull);
private:
	ConvexHullTest _hull;
};

#endif // REGION_H