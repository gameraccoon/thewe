#ifndef WORLD_MAP_H
#define WORLD_MAP_H

#include <cocos2d.h>
#include <string>

#include "Region.h"

class WorldMap
{
public:
	typedef std::map<std::string, Region::Ptr> RegionsMap;

	Region::Ptr GetRegion(std::string regionName) const;
	void AddRegion(std::string regionName, Region::Ptr region);

	const RegionsMap& GetRegions() const;
private:
	std::map<std::string, Region::Ptr> _regions;
};

#endif // WORLD_MAP_H
