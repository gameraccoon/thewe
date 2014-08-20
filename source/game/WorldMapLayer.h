#ifndef WORLD_MAP_LAYER_H
#define WORLD_MAP_LAYER_H

#include <cocos2d.h>

#include "ArbitraryHull.h"
#include "MapProjector.h"
#include "Region.h"
#include "Cell.h"

#include "TownInfoLayer.h"

class GameScene;

class WorldMapLayer : public cocos2d::CCLayer
{
public:
	WorldMapLayer(GameScene *gameScene, MapProjector* projector);

	virtual bool init(void) override;
	virtual void visit() override;

	void menuCloseCallback(cocos2d::CCObject *Sender);

	void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event) override;
	void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event) override;
	void ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event) override;

	void ModifyZoom(float multiplier);

	void SetMapInputEnabled(bool isEnabled);
	void SetGuiEnabled(bool isEnabled);
	void SetNextCellParent(Cell::WeakPtr parent);

private:
	enum E_MAP_OBJECT_TAG
	{
		MAP_OBJ_NONE
		,MAP_OBJ_CELL
		,MAP_OBJ_TOWN
		,MAP_OBJ_REGION
	};

private:
	GameScene *_gameScene;

	Cell::WeakPtr _nextCellParent;

	Region::WeakPtr _GetRegionUnderPoint(const Point& point) const;
	Cell::WeakPtr _GetCellUnderPoint(const Point& point) const;
	Town::WeakPtr _GetTownUnderPoint(const Point& point);

	void _AddCellToRender(Cell::WeakPtr cell);
	void _OnTownSelect(Town::WeakPtr town);
	void _DrawCellsLinksRecurcively(Cell::WeakPtr cell);

	Point _touchLastPoint;
	Point _touchFirstPos;

	MapProjector *_mapProjector;

	cocos2d::CCLayer *_mapGui;

	bool _isInputEnabled;

	bool _isPlacingCell;

	ArbitraryHull _cellHull;
};

#endif
