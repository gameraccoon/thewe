#ifndef WORLD_MAP_LAYER_H
#define WORLD_MAP_LAYER_H

#include <cocos2d.h>

#include "ArbitraryHull.h"
#include "MapProjector.h"
#include "MessageManager.h"
#include "Region.h"
#include "Cell.h"
#include "CellGameInterface.h"
#include "CellMapWidget.h"
#include "TownMapWidget.h"
#include "InvestigatorMapWidget.h"

class GameScene;

class WorldMapLayer : public cocos2d::Layer, public MessageReceiver
{
public:
	WorldMapLayer(GameScene *gameScene, MapProjector* projector);
	~WorldMapLayer(void);

	virtual bool init(void) override;
	virtual void update(float dt) override;
	virtual void AcceptMessage(const Message &msg) override;

	void menuCloseCallback(cocos2d::Ref *Sender);

	void TouchesBegan(const std::vector<cocos2d::Touch* > &touches, cocos2d::Event* event);
	void TouchesEnded(const std::vector<cocos2d::Touch* > &touches, cocos2d::Event* event);
	void TouchesMoved(const std::vector<cocos2d::Touch* > &touches, cocos2d::Event* event);

	void ModifyZoom(float multiplier);
	void HideCellGameInterface(void);

	void SetMapInputEnabled(bool isEnabled);
	void SetGuiEnabled(bool isEnabled);
	void SetNextCellParent(Cell::WeakPtr parent);
	void SetLinkCellChildren(Cell::WeakPtr children);

	Cell::Ptr CreateCell(Cell::Info info, Cell::State state);
	void DeleteCell(CellMapWidget *widget);

	void PushSessionFailScreen(void);
	void PushSessionWinScreen(void);

	CellMapWidget* GetCellMapWidget(Cell::Ptr cell) const;

private:
	enum CONTENT_Z_ORDER
	{
		Z_MAP = 0,
		Z_LINKS,
		Z_INVESTIGATOR,
		Z_TOWN,
		Z_CELL,
		Z_CELL_MENU,
		Z_MAP_GUI
	};

	typedef std::vector<CellMapWidget *> CellWidgets;
	typedef CellWidgets::iterator CellWidgetsIter;
	typedef std::vector<TownMapWidget *> TownWidgets;
	typedef TownWidgets::iterator TownWidgetsIter;
	typedef std::vector<InvestigatorMapWidget *> InvestigatorWidgets;
	typedef InvestigatorWidgets::iterator InvestigatorWidgetsIter;

private:
	GameScene *_gameScene;

	Cell::WeakPtr _nextCellParent;
	Cell::WeakPtr _linkCellChildren;

	Region::WeakPtr _GetRegionUnderPoint(const Vector2& point) const;
	Cell::WeakPtr _GetCellUnderPoint(const Vector2& point);
	Town::WeakPtr _GetTownUnderPoint(const Vector2& point);
	
	CellMapWidget* _CreateCellWidget(Cell::Ptr cell);
	TownMapWidget* _CreateTownWidget(Town::Ptr town);
	InvestigatorMapWidget* _CreateInvestigatorWidget(Investigator::Ptr investigator);

	void _UpdateNetwork();
	void _RecursiveUpdateNetworkVisualiser(cocos2d::DrawNode *visualiser, Cell::WeakPtr cell);
	void _OnTownSelect(Town::WeakPtr town);
	void RecalculateTouches(const std::vector<cocos2d::Touch* > &touches, bool updateView);
	void ResetTouches();
	void BackToMainMenuCallback(cocos2d::Ref *sender);

	CellWidgets _cellWidgets;
	TownWidgets _townWidgets;
	InvestigatorWidgets _investigatorWidgets;

	CellMenuSelector *_cellMenu;
	cocos2d::Layer *_cellGameInterface;
	cocos2d::Layer *_townsDrawLayer;

	Vector2 _touchLastPoint;
	Vector2 _touchFirstPos;

	MapProjector *_mapProjector;

	cocos2d::Layer *_mapGui;
	cocos2d::DrawNode *_networkVisualiser;

	bool _isInputEnabled;
	bool _isSessionScreenShowed;

	Vector2 _touchesCenter;
	int _lastTouchesCount;
	float _avgTouchDistance;
	bool _isTouchesCountUpdated;
};

#endif
