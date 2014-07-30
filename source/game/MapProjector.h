#ifndef MAP_PROJECTOR_H
#define MAP_PROJECTOR_H

#include <cocos2d.h>
#include "Point.h"
#include "ArbitraryHull.h"

class MapProjector
{
public:
	MapProjector(Point spriteSize);

	Point ProjectOnMap(Point screenPoint) const;
	Point ProjectOnScreen(Point mapPoint) const;

	ArbitraryHull ProjectOnMap(const ArbitraryHull& screenHull) const;
	ArbitraryHull ProjectOnScreen(const ArbitraryHull& screenHull) const;

	//cocos2d::CCSprite* GetSprite() const;

	void SetShift(Point shift);
	void SetScale(float scale);

	Point GetShift() const;
	float GetScale() const;

	void SetScreenCenter(Point centerPos);

	void AddSprite(Point location, Point shift, cocos2d::CCSprite *sprite);

private:
	struct LocatedSprite
	{
		/** Положение спрайта в мире */
		Point location;
		/** Сдвиг центра спрайта */
		Point shift;
		/** Спрайт */
		cocos2d::CCSprite *sprite;
	};

	float _mapScale;
	Point _mapShift;
	//cocos2d::CCSprite *_mapSprite;

	Point _screenCenter;

	Point _mapSpriteSize;

	std::vector<LocatedSprite> _spritesToProject;
};

#endif // MAP_PROJECTOR_H