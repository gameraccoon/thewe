#ifndef WORLD_MAP_LAYER_H
#define WORLD_MAP_LAYER_H

#include "cocos2d.h"

#include "ArbitraryHull.h"
#include "MapProjector.h"
#include "Region.h"

class WorldMapLayer : public cocos2d::CCLayer
{
public:
	WorldMapLayer(MapProjector* projector);

	virtual bool init(void) override;

	void menuCloseCallback(cocos2d::CCObject *Sender);

	void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event) override;
	void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event) override;
	void ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event) override;

	void ModifyZoom(float multiplier);

	void SetInputEnabled(bool isEnabled);

	virtual void visit() override;

private:
	Region::Ptr GetRegionUnderPoint(const cocos2d::CCPoint& point);

	Region::Ptr _tappedRegion;

	cocos2d::CCPoint _touchLastPoint;

	MapProjector *_mapProjector;

	cocos2d::CCLayer *_mapGui;

	bool _isInputEnabled;
};

#endif
