#ifndef MAP_PROJECTOR_H
#define MAP_PROJECTOR_H

#include <memory>
#include <cocos2d.h>
#include "Point.h"
#include "ArbitraryHull.h"

class MapProjector
{
public:
	MapProjector(Point mapSize);

	Point ProjectOnMap(Point screenPoint) const;
	Point ProjectOnScreen(Point mapPoint) const;

	ArbitraryHull ProjectOnMap(const ArbitraryHull& screenHull) const;
	ArbitraryHull ProjectOnScreen(const ArbitraryHull& mapHull) const;

	void SetLocation(Point mapLocation);
	void SetScale(float scale);

	void ShiftView(Point screenDelta);

	Point GetLocation() const;
	float GetScale() const;

	void SetScreenCenter(Point centerPos);

	void AddMapPart(Point location, Point shift, cocos2d::CCNode *node);
	cocos2d::CCSprite* AddSprite(Point location, Point shift, std::string spriteName);

	void RemoveMapPart(const cocos2d::CCNode *node);

private:
	struct MapPart
	{
		/** Умный указатель с возможностью устонавливать свои правила удаления объектов */
		typedef std::shared_ptr<cocos2d::CCNode> NodePtr;
		/** Положение объекта в мире */
		Point location;
		/** Сдвиг центра объекта */
		Point shift;
		/** Указатель на объект */
		NodePtr node;
	};

	typedef std::vector<MapPart> MapParts;

	void CheckBoundings();

	void UpdateNodes();

	float _mapScale;
	Point _mapLocation;

	Point _screenCenter;

	Point _mapSize;

	MapParts _mapParts;
};

#endif // MAP_PROJECTOR_H