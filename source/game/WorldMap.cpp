#include "WorldMap.h"

static const cocos2d::CCPoint MAP_INITIAL_SIZE = cocos2d::CCPoint(1390.0f, 1003.0f);

WorldMap::WorldMap()
{
	// ToDo: вынести инициализацию спрайтов в отдельный класс
	_mapSprite = cocos2d::CCSprite::create("../_gamedata/WorldMap.png");
}

cocos2d::CCSprite* WorldMap::getSprite() const
{
	return _mapSprite;
}
