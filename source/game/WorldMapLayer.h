#ifndef WORLD_MAP_LAYER_H
#define WORLD_MAP_LAYER_H

#include "cocos2d.h"

#include "ConvexHullTest.h"

#include "MapProjector.h"

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


private:
	ConvexHullTest _hull1;
	bool _isPointInHull;

	cocos2d::CCPoint _touchLastPoint;

	void _IdleUpdate(float timeDelta);

	MapProjector _mapProjector;
};

#endif
