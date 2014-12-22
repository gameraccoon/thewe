#ifndef MAP_PROJECTOR_H
#define MAP_PROJECTOR_H

#include <memory>
#include <string>

#include <cocos2d.h>

#include "Vector2.h"
#include "ArbitraryHull.h"

class MapProjector : public cocos2d::Layer
{
public:
	MapProjector();

	static MapProjector* create();

	/** Set screen center of the camera view */
	void SetScreenCenter(Vector2 centerPos);
	void SetMapSize(const Vector2& mapSize);

	/** Project point from screen coordinates to map coordinates */
	Vector2 ProjectOnMap(Vector2 screenPoint) const;
	/** Project point from map coordinates to screen coordinates */
	Vector2 ProjectOnScreen(Vector2 mapPoint) const;

	/** Project hull from screen coordinates to map coordinates */
	ArbitraryHull ProjectOnMap(const ArbitraryHull& screenHull) const;
	/** Project hull from map coordinates to screen coordinates */
	ArbitraryHull ProjectOnScreen(const ArbitraryHull& mapHull) const;

	/** Set new camera location on the map */
	void SetLocation(Vector2 mapLocation);
	/** Set new scale of the view */
	void SetScale(float scale);

	/** Shift view with screen coordinates */
	void ShiftView(Vector2 screenDelta);

	/** Get camera location in the world */
	Vector2 GetLocation() const;
	/** Get current view scale */
	float GetScale() const;

	/** Update all elements position */
	void Update();
	
	/** Add new element that will be projected as map part */
	int AddProjectedChild(cocos2d::Node *node, Vector2 location, Vector2 shift, float scale, bool dontScale = false);
	
	/** Remove and destroy some element */
	virtual void removeChild(cocos2d::Node* child, bool cleanup = true) override;
	virtual void removeChildByTag(int tag, bool cleanup = true) override;

private:
	struct MapPart
	{
		/** World location */
		Vector2 location;
		/** Objects center shift in screen coordinates */
		Vector2 shift;
		/** Initial scale of the object */
		float initialScale;
		/** Are we need to scale object with map */
		bool isScalable;
		/** Graphical object */
		cocos2d::Node *node;
		/** Unique tag */
		int tag;
	};

	/** Projected parts of map */
	typedef std::vector<MapPart> MapParts;

private:
	/** Checking intersection of the screen borders and correct position if needed */
	void _CheckBoundings();

	int _GetNewPartUid(void) const;

	/** Update position of all projected elements */
	void _UpdateNodes();

private:
	Vector2 _viewLocation;
	float _viewScale;

	Vector2 _screenCenter;

	Vector2 _mapSize;

	MapParts _mapParts;
};

#endif // MAP_PROJECTOR_H
