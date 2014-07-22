#ifndef WORLD_MAP_LAYER_H
#define WORLD_MAP_LAYER_H

#include "cocos2d.h"

#include "ConvexHullTest.h"

#include "WorldMap.h"

class WorldMapLayer : public cocos2d::CCLayer
{
public:
	WorldMapLayer();
	virtual bool init(void) override;

	static cocos2d::CCScene* scene(void);

	void menuCloseCallback(cocos2d::CCObject *Sender);

	CREATE_FUNC(WorldMapLayer);
	
    virtual void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event) override;
	virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event) override;
    virtual void ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event) override;

	virtual void visit(void) override;

	cocos2d::CCPoint projectOnMap(cocos2d::CCPoint screenPoint);
	cocos2d::CCPoint projectOnScreen(cocos2d::CCPoint mapPoint);
private:
	ConvexHullTest _hull1;
	bool _isPointInHull;

	cocos2d::CCPoint _touchFirstPoint;

	void _IdleUpdate(float timeDelta);

	WorldMap _worldMap;
	float _mapScale;
	cocos2d::CCPoint _mapShift;
};

#endif
