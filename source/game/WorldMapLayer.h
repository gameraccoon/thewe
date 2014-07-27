#ifndef WORLD_MAP_LAYER_H
#define WORLD_MAP_LAYER_H

#include "cocos2d.h"

#include "ArbitraryHull.h"
#include "MapProjector.h"
#include "Region.h"

class WorldMapLayer : public cocos2d::CCLayer
{
public:
	WorldMapLayer(void);

	virtual bool init(void) override;

	void menuCloseCallback(cocos2d::CCObject *Sender);

	CREATE_FUNC(WorldMapLayer);

	void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event) override;
	void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event) override;
	void ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event) override;

	virtual void visit() override;

private:
	Region::Ptr GetRegionUnderPoint(const cocos2d::CCPoint& point);

	Region::Ptr _tappedRegion;

	cocos2d::CCPoint _touchLastPoint;

	MapProjector _mapProjector;
};

#endif
