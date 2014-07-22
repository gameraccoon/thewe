#ifndef WORLD_MAP_H
#define WORLD_MAP_H

#include <cocos2d.h>

class WorldMap
{
public:
	WorldMap();
	cocos2d::CCSprite* getSprite() const;
private:
	cocos2d::CCSprite *_mapSprite;
};

#endif // WORLD_MAP_H
