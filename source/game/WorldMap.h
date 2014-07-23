#ifndef WORLD_MAP_H
#define WORLD_MAP_H

#include <cocos2d.h>
#include <string>

#include "Region.h"

class WorldMap
{
public:
private:
	std::map<std::string, Region> regions;
};

#endif // WORLD_MAP_H
