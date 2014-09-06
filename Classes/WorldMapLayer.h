#ifndef WORLD_MAP_LAYER_H
#define WORLD_MAP_LAYER_H

#include <cocos2d.h>

#include "ArbitraryHull.h"
#include "MapProjector.h"
#include "Region.h"
#include "Cell.h"

#include "TownInfoLayer.h"

class GameScene;
class CellMenuSelector;

class WorldMapLayer : public cocos2d::Layer
{
public:
	WorldMapLayer(GameScene *gameScene, MapProjector* projector);

	virtual bool init(void) override;

	void menuCloseCallback(cocos2d::Ref *Sender);

	void onTouchesBegan(const std::vector<cocos2d::Touch* > &touches, cocos2d::Event* event) override;
	void onTouchesEnded(const std::vector<cocos2d::Touch* > &touches, cocos2d::Event* event) override;
	void onTouchesMoved(const std::vector<cocos2d::Touch* > &touches, cocos2d::Event* event) override;

	void ModifyZoom(float multiplier);

	void SetMapInputEnabled(bool isEnabled);
	void SetGuiEnabled(bool isEnabled);
	void SetNextCellParent(Cell::WeakPtr parent);

private:
	enum class E_MAP_OBJECT_TAG
	{
		MAP_OBJ_NONE
		,MAP_OBJ_CELL
		,MAP_OBJ_TOWN
		,MAP_OBJ_REGION
	};

private:
	GameScene *_gameScene;

	Cell::WeakPtr _nextCellParent;

	bool _IsCellGameInterfaceOnScreen(void) const;

	void _ShowCellGameInterface(Cell::Ptr cell);
	void _HideCellGameInterface(void);

	Region::WeakPtr _GetRegionUnderPoint(const Vector2& point) const;
	Cell::WeakPtr _GetCellUnderPoint(const Vector2& point) const;
	Town::WeakPtr _GetTownUnderPoint(const Vector2& point);
	
	void _UpdateNetwork();
	void _RecursiveUpdateNetworkVisualiser(cocos2d::DrawNode *visualiser, Cell::WeakPtr cell);
	void _AddCellToRender(Cell::WeakPtr cell);
	void _OnTownSelect(Town::WeakPtr town);
	void _DrawCellsLinksRecurcively(Cell::WeakPtr cell);

	CellMenuSelector *_cellMenu;

	Vector2 _touchLastPoint;
	Vector2 _touchFirstPos;

	MapProjector *_mapProjector;

	cocos2d::Layer *_mapGui;
	cocos2d::DrawNode *_networkVisualiser;

	bool _isInputEnabled;

	bool _isPlacingCell;

	ArbitraryHull _cellHull;
};

#endif
