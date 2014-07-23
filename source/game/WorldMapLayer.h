#ifndef WORLD_MAP_LAYER_H
#define WORLD_MAP_LAYER_H

#include "cocos2d.h"

#include "ConvexHullTest.h"
#include "WorldMap.h"

class WorldMapLayer : public cocos2d::CCLayer
{
public:
	WorldMapLayer(void);

	virtual bool init(void);

	void menuCloseCallback(cocos2d::CCObject *Sender);

	CREATE_FUNC(WorldMapLayer);

	void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
	void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
	void ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event);

	cocos2d::CCPoint projectOnMap(cocos2d::CCPoint screenPoint);

private:
	enum E_BUTTONTS_TAGS
	{
		BTN_SAVE_XML,
		BTN_TOGGLE,
	};

private:
	cocos2d::CCPoint _touchPos;
	
	cocos2d::CCPoint _touchFirstPoint;
	cocos2d::CCPoint _mapShift;

	WorldMap _worldMap;

	float _mapScale;
};

#endif
