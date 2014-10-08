#ifndef WORLD_MAP_LAYER_H
#define WORLD_MAP_LAYER_H

#include <cocos2d.h>

#include "ArbitraryHull.h"
#include "MapProjector.h"
#include "Region.h"
#include "Cell.h"
#include "CellGameInterface.h"
#include "CellMapWidget.h"
#include "TownMapWidget.h"

class GameScene;

class WorldMapLayer : public cocos2d::Layer
{
public:
	WorldMapLayer(GameScene *gameScene, MapProjector* projector);
	~WorldMapLayer(void);

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

	void CreateCell(const Cell::Info &info, Cell::State state, Utils::GameTime constructionTime);
	void DeleteCell(CellMapWidget *widget);

private:
	enum CONTENT_Z_ORDER
	{
		Z_MAP = 0,
		Z_LINKS,
		Z_TOWN,
		Z_CELL,
		Z_CELL_MENU
	};

	typedef std::vector<CellMapWidget *> CellWidgetsList;
	typedef CellWidgetsList::iterator CellWidgetsIter;
	typedef std::vector<TownMapWidget *> TownWidgetsList;
	typedef TownWidgetsList::iterator TownWidgetsIter;

private:
	GameScene *_gameScene;

	Cell::WeakPtr _nextCellParent;

	Region::WeakPtr _GetRegionUnderPoint(const Vector2& point) const;
	Cell::WeakPtr _GetCellUnderPoint(const Vector2& point) const;
	Town::WeakPtr _GetTownUnderPoint(const Vector2& point);
	
	CellMapWidget* _CreateCellWidget(Cell::Ptr cell);
	TownMapWidget* _CreateTownWidget(Town::Ptr town);

	void _UpdateNetwork();
	void _RecursiveUpdateNetworkVisualiser(cocos2d::DrawNode *visualiser, Cell::WeakPtr cell);
	void _OnTownSelect(Town::WeakPtr town);
	void RecalculateTouches(const std::vector<cocos2d::Touch* > &touches, bool updateView);
	void ResetTouches();

	CellWidgetsList _cellWidgetsList;
	TownWidgetsList _townWidgetsList;

	CellMenuSelector *_cellMenu;
	cocos2d::Layer *_cellGameInterface;

	Vector2 _touchLastPoint;
	Vector2 _touchFirstPos;

	MapProjector *_mapProjector;

	cocos2d::Layer *_mapGui;
	cocos2d::DrawNode *_networkVisualiser;

	bool _isInputEnabled;

	Vector2 _touchesCenter;
	int _lastTouchesCount;
	float _avgTouchDistance;
	bool _isTouchesCountUpdated;
};

#endif
