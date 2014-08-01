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

	void SetLocation(Point mapLocation);
	void SetScale(float scale);

	void ShiftView(Point delta);

	Point GetShift() const;
	float GetScale() const;

	void SetScreenCenter(Point centerPos);

	void AddNode(Point location, Point shift, cocos2d::CCNode *node);

private:
	struct MapPart
	{
		/** Положение спрайта в мире */
		Point location;
		/** Сдвиг центра спрайта */
		Point shift;
		/** Спрайт */
		cocos2d::CCNode *node;
	};

	void CheckBoundings();

	void UpdateNodes();

	float _mapScale;
	Point _mapLocation;

	Point _screenCenter;

	Point _mapSpriteSize;

	std::vector<MapPart> _nodesToProject;
};

#endif // MAP_PROJECTOR_H