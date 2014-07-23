#ifndef MAP_PROJECTOR_H
#define MAP_PROJECTOR_H

#include "WorldMap.h"

class MapProjector
{
public:
	MapProjector(cocos2d::CCPoint shift, float scale);

	cocos2d::CCPoint ProjectOnMap(cocos2d::CCPoint screenPoint) const;
	cocos2d::CCPoint ProjectOnScreen(cocos2d::CCPoint mapPoint) const;

	cocos2d::CCSprite* GetSprite() const;

	void SetShift(cocos2d::CCPoint shift);
	void SetScale(float scale);

	cocos2d::CCPoint GetShift();
	float GetScale();

	void SetScreenCenter(cocos2d::CCPoint centerPos);
private:
	WorldMap _worldMap;
	float _mapScale;
	cocos2d::CCPoint _mapShift;
	cocos2d::CCSprite *_mapSprite;

	cocos2d::CCPoint _screenCenter;
};

#endif // MAP_PROJECTOR_H