#ifndef WORLD_MAP_LAYER_H
#define WORLD_MAP_LAYER_H

#include "cocos2d.h"

#include "ArbitraryHull.h"
#include "MapProjector.h"
#include "Region.h"
#include "Cell.h"

#include "TownInfoLayer.h"

class WorldMapLayer : public cocos2d::CCLayer
{
public:
	WorldMapLayer(MapProjector* projector);

	virtual bool init(void) override;
	virtual void visit() override;

	void menuCloseCallback(cocos2d::CCObject *Sender);

	void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event) override;
	void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event) override;
	void ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event) override;

	void ModifyZoom(float multiplier);

	void SetMapInputEnabled(bool isEnabled);
	void SetGuiEnabled(bool isEnabled);

private:
	enum E_MAP_OBJECT_TAG
	{
		MAP_OBJ_NONE
		,MAP_OBJ_CELL
		,MAP_OBJ_TOWN
		,MAP_OBJ_REGION
	};

private:

	Region::Ptr GetRegionUnderPoint(const Point& point) const;
	Cell::Ptr GetCellUnderPoint(const Point& point) const;
	Region::Ptr _GetRegionUnderPoint(const Point& point) const;
	Cell::Ptr _GetCellUnderPoint(const Point& point) const;
	void _AddCellToRender(Cell::Ptr cell);
	Town::Ptr GetTownUnderPoint(const Point& point);

	Point _touchLastPoint;
	Point _touchFirstPos;

	MapProjector *_mapProjector;

	cocos2d::CCLayer *_mapGui;

	bool _isInputEnabled;

	bool _isPlacingCell;

	ArbitraryHull _cellHull;
};

#endif
