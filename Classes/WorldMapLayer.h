#ifndef WORLD_MAP_LAYER_H
#define WORLD_MAP_LAYER_H

#include <cocos2d.h>

#include "ArbitraryHull.h"
#include "MapProjector.h"
#include "Region.h"
#include "Cell.h"
#include "CellGameInterface.h"
#include "TownInfoLayer.h"

class GameScene;

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
	void HideCellGameInterface(void);

	void SetMapInputEnabled(bool isEnabled);
	void SetGuiEnabled(bool isEnabled);
	void SetNextCellParent(Cell::WeakPtr parent);

private:
	GameScene *_gameScene;

	Cell::WeakPtr _nextCellParent;

	Region::WeakPtr _GetRegionUnderPoint(const Vector2& point) const;
	Cell::WeakPtr _GetCellUnderPoint(const Vector2& point) const;
	Town::WeakPtr _GetTownUnderPoint(const Vector2& point);
	
	void _UpdateNetwork();
	void _RecursiveUpdateNetworkVisualiser(cocos2d::DrawNode *visualiser, Cell::WeakPtr cell);
	void _AddCellToRender(Cell::Ptr cell);
	void _OnTownSelect(Town::WeakPtr town);
	void _DrawCellsLinksRecurcively(Cell::WeakPtr cell);

	void RecalculateTouches(const std::vector<cocos2d::Touch* > &touches, bool updateView);
	void ResetTouches();

	CellMenuSelector *_cellMenu;
	cocos2d::Layer *_cellGameInterface;

	Vector2 _touchLastPoint;
	Vector2 _touchFirstPos;

	MapProjector *_mapProjector;

	cocos2d::Layer *_mapGui;
	cocos2d::DrawNode *_networkVisualiser;

	bool _isInputEnabled;

	bool _isPlacingCell;

	Vector2 _touchesCenter;
	int _lastTouchesCount;
	float _avgTouchDistance;
	bool _isTouchesCountUpdated;
};

#endif
