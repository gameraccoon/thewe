#ifndef MAP_PROJECTOR_H
#define MAP_PROJECTOR_H

#include <cocos2d.h>
#include "Point.h"
#include "ArbitraryHull.h"

class MapProjector
{
public:
	MapProjector(Point shift, float scale);

	Point ProjectOnMap(Point screenPoint) const;
	Point ProjectOnScreen(Point mapPoint) const;

	ArbitraryHull ProjectOnMap(const ArbitraryHull& screenHull) const;
	ArbitraryHull ProjectOnScreen(const ArbitraryHull& screenHull) const;

	cocos2d::CCSprite* GetSprite() const;

	void SetShift(Point shift);
	void SetScale(float scale);

	Point GetShift();
	float GetScale();

	void SetScreenCenter(Point centerPos);

private:
	float _mapScale;
	Point _mapShift;
	cocos2d::CCSprite *_mapSprite;

	Point _screenCenter;
};

#endif // MAP_PROJECTOR_H