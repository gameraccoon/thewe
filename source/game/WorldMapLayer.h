#ifndef WORLD_MAP_LAYER_H
#define WORLD_MAP_LAYER_H

#include "cocos2d.h"

#include "ArbitraryHull.h"
#include "MapProjector.h"
#include "Region.h"
#include "Cell.h"

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

	void SetMapInputEnabled(bool isEnabled);
	void SetGuiEnabled(bool isEnabled);

	virtual void visit() override;

private:
	Region::Ptr GetRegionUnderPoint(const Point& point) const;
	Cell::Ptr GetCellUnderPoint(const Point& point) const;

	Point _touchLastPoint;
	Point _touchFirstPos;

	MapProjector *_mapProjector;

	cocos2d::CCLayer *_mapGui;

	bool _isInputEnabled;

	ArbitraryHull _cellHull;
};

#endif
