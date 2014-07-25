#ifndef WORLD_MAP_LAYER_H
#define WORLD_MAP_LAYER_H

#include "cocos2d.h"

#include "ArbitraryHull.h"
#include "MapProjector.h"

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

	virtual void visit() override;
private:
	cocos2d::CCPoint _touchLastPoint;

	MapProjector _mapProjector;
};

#endif
