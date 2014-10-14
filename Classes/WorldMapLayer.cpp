#include "WorldMapLayer.h"

#include "GameScene.h"
#include "World.h"
#include "GameScene.h"
#include "MapGuiLayer.h"
#include "TaskManager.h"
#include "Log.h"

static const float MAX_MAP_SCALE = 1.5f;

WorldMapLayer::WorldMapLayer(GameScene *gameScene, MapProjector* projector)
	: _mapProjector(projector)
	, _isInputEnabled(true)
	, _mapGui(nullptr)
	, _gameScene(gameScene)
	, _nextCellParent()
	, _cellMenu(nullptr)
	, _cellGameInterface(nullptr)
{
	init();
}

WorldMapLayer::~WorldMapLayer(void)
{
	for (TownWidgetsIter it = _townWidgetsList.begin(); it != _townWidgetsList.end(); ++it)
	{
		TownMapWidget *widget = (*it);
		/* Is this widget must be removed form projector ? How ?
		_mapProjector->RemoveMapPart(widget->GetProjectorUid());
		*/
		removeChild(widget);
		delete widget;
	}

	for (CellWidgetsIter it = _cellWidgetsList.begin(); it != _cellWidgetsList.end(); ++it)
	{
		CellMapWidget *widget = (*it);
		/* Is this widget must be removed form projector ? How ?
		_mapProjector->RemoveMapPart(widget->GetProjectorUid());
		*/
		removeChild(widget);
		delete widget;
	}
}

bool WorldMapLayer::init(void)
{
	if (!cocos2d::Layer::init())
	{
		return false;
	}
	
	_networkVisualiser = cocos2d::DrawNode::create();
	addChild(_networkVisualiser, Z_LINKS);

	cocos2d::Sprite *mapSprite = cocos2d::Sprite::create("WorldMap.png");
	mapSprite->retain();
	_mapProjector->AddMapPart(Drawable::CastFromCocos(mapSprite), Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f), 1.0f, false);
	_mapProjector->SetMapSize(mapSprite->getTextureRect().size);
	addChild(mapSprite, Z_MAP);

	Vector2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();
	Vector2 screen = cocos2d::Director::getInstance()->getVisibleSize();

	SetGuiEnabled(true);

	for (const Cell::Ptr cell : World::Instance().GetCells())
	{
		CellMapWidget *widget = _CreateCellWidget(cell);
		_cellWidgetsList.push_back(widget);
		addChild(widget, Z_CELL);
	}
	
	for (const Town::Ptr town : World::Instance().GetTowns())
	{
		TownMapWidget *widget = _CreateTownWidget(town);
		_townWidgetsList.push_back(widget);
		addChild(widget, Z_TOWN);
	}
	
	// say where is screen center
	_mapProjector->SetScreenCenter(origin + screen / 2.0f);
	// send map sprite to the center of world
	_mapProjector->SetLocation(Vector2(0.0f, 0.0f));
	// set scale to first time
	_mapProjector->SetScale(0.01f);

	_cellMenu = new CellMenuSelector(_mapProjector, this);
	_cellMenu->autorelease();
	addChild(_cellMenu, Z_CELL_MENU);

	setTouchEnabled(true);
	setKeyboardEnabled(true);
	scheduleUpdate();

	_UpdateNetwork();

	return true;
}

void WorldMapLayer::update(float dt)
{
	if (World::Instance().IsGameOver())
	{
		// push game over screen
	}

	// check for widgets existance and create new if needed
	for (Investigator::Ptr investigator : World::Instance().GetInvestigators())
	{
		if (!getChildByTag(investigator->GetUid()))
		{
			InvestigatorMapWidget *widget = _CreateInvestigatorWidget(investigator);
			addChild(widget, Z_INVESTIGATOR);
			_investigatorWidgetsList.push_back(widget);
		}
	}

	// check for model existance and delete widget if needed
	for (InvestigatorMapWidget *widget : _investigatorWidgetsList)
	{
		Investigator::Ptr existed = widget->GetInvestigator();
		bool founded = false;

		for (Investigator::Ptr investigator : World::Instance().GetInvestigators())
		{
			if (existed == investigator)
			{
				founded = true;
				break;
			}
		}

		if (!founded)
		{
			removeChild(widget);
			delete widget;
		}
	}
}

void WorldMapLayer::SetMapInputEnabled(bool isEnabled)
{
	_isInputEnabled = isEnabled;
}

void WorldMapLayer::SetGuiEnabled(bool isEnabled)
{
	if (_mapGui && !isEnabled)
	{
		removeChild(_mapGui);
		_mapGui = nullptr;
	}
	else if (!_mapGui && isEnabled)
	{
		_mapGui = new MapGuiLayer(_mapProjector);
		addChild(_mapGui, Z_MAP_GUI);
		_mapGui->autorelease();
		_mapProjector->SetScale(_mapProjector->GetScale());
	}
}

void WorldMapLayer::SetNextCellParent(Cell::WeakPtr parent)
{
	_nextCellParent = parent;
}

void WorldMapLayer::CreateCell(const Cell::Info &info, Cell::State state)
{
	Cell::Ptr cell = Cell::Create(info);
	
	if (info.parent)
	{
		info.parent->AddChild(cell);
	}

	cell->GetInfo().state = state;

	World::Instance().AddCell(cell);

	CellMapWidget *widget = _CreateCellWidget(cell);
	_cellWidgetsList.push_back(widget);
	addChild(widget, Z_CELL);

	_UpdateNetwork();
}

void WorldMapLayer::DeleteCell(CellMapWidget *widget)
{
	// we need to delete cell from the network
	Cell::Ptr cell = widget->GetCell();

	removeChild(widget);
	_mapProjector->RemoveMapPart(widget->GetProjectorUid());

	delete widget;
}

void WorldMapLayer::menuCloseCallback(cocos2d::Ref *Sender)
{
}

void WorldMapLayer::onTouchesBegan(const std::vector<cocos2d::Touch* > &touches, cocos2d::Event* event)
{
	if (_isInputEnabled)
	{
		cocos2d::Touch *touch = touches.at(0);
		_touchLastPoint = touch->getLocation();
		_touchFirstPos = touch->getLocation();

        if (_cellMenu->isVisible())
		{
			_cellMenu->DisappearWithAnimation();
		}

		ResetTouches();
	}
}

void WorldMapLayer::onTouchesEnded(const std::vector<cocos2d::Touch* > &touches, cocos2d::Event* event)
{
	if (_isInputEnabled)
	{
		if (touches.size() == 1)
		{
			cocos2d::Touch *touch = touches.at(0);
			Vector2 point = touch->getLocation();
			Vector2 v = _touchFirstPos - point;

			const float size = v.Size();
			const float tolerance = 5.0f;

			if (size <= tolerance)
			{
				Cell::WeakPtr cell = _GetCellUnderPoint(point);
				if (!cell.expired())
				{
					Vector2 cell_pos = cell.lock()->GetInfo().location;
					Vector2 menu_pos = _mapProjector->ProjectOnScreen(cell_pos);

					_cellMenu->DisappearImmedaitely();
					_cellMenu->AppearWithAnimation(cell, menu_pos);

					return;
				}
				else
				{
					_cellMenu->DisappearWithAnimation();		
				}

				Town::WeakPtr town = _GetTownUnderPoint(point);
				if (!town.expired())
				{
					_OnTownSelect(town);
					return;
				}
			}
		}
	}
}

void WorldMapLayer::onTouchesMoved(const std::vector<cocos2d::Touch* > &touches, cocos2d::Event* event)
{
	if (_isInputEnabled && touches.size() > 0)
	{
		if (!_isTouchesCountUpdated && (unsigned)_lastTouchesCount == touches.size())
		{
			RecalculateTouches(touches, true);
		}
		else
		{
			RecalculateTouches(touches, false);
		}
	}

	_UpdateNetwork();
}

void WorldMapLayer::RecalculateTouches(const std::vector<cocos2d::Touch* > &touches, bool updateView)
{
	_lastTouchesCount = touches.size();

	// middle point of all touches
	Vector2 newTouchPoint(0.0f, 0.0f);
	for (const cocos2d::Touch* touch : touches)
	{
		newTouchPoint += touch->getLocation();
	}
	newTouchPoint /= _lastTouchesCount;

	float newAvgDistance = 0;
	for (const cocos2d::Touch* touch : touches)
	{
		newAvgDistance += (newTouchPoint - touch->getLocation()).Size();
	}
	newAvgDistance /= _lastTouchesCount;

	if (updateView)
	{
		_mapProjector->ShiftView(newTouchPoint - _touchesCenter);
		if (_avgTouchDistance > 0.01 && newAvgDistance > 0.01)
		_mapProjector->SetScale(std::min(MAX_MAP_SCALE, _mapProjector->GetScale() * (newAvgDistance / _avgTouchDistance)));
	}

	_avgTouchDistance = newAvgDistance;
	_touchesCenter = newTouchPoint;

	_isTouchesCountUpdated = false;
}

void WorldMapLayer::ResetTouches()
{
	_isTouchesCountUpdated = true;
}

CellMapWidget* WorldMapLayer::_CreateCellWidget(Cell::Ptr cell)
{
	CellMapWidget *widget = new CellMapWidget(cell);
	
	int uid = _mapProjector->AddMapPart(Drawable::CastFromCocos(widget), cell->GetInfo().location, Vector2(0.0f, 0.0f), 0.2f, true);
	_mapProjector->Update();

	cocos2d::Rect tex = widget->GetCellRect();
	float w = tex.size.width * widget->getScaleX();
	float h = tex.size.height * widget->getScaleY();
	
	widget->SetHitArea(-(w / 2.0f), -(h / 2.0f), w, h);
	widget->setTag(cell->GetUid());
	widget->SetProjectorUid(uid);
	widget->retain();

	return widget;
}

TownMapWidget* WorldMapLayer::_CreateTownWidget(Town::Ptr town)
{
	TownMapWidget *widget = new TownMapWidget(town);
	
	int uid = _mapProjector->AddMapPart(Drawable::CastFromCocos(widget), town->GetInfo().location, Vector2(0.0f, 0.0f), 0.17f, true);
	_mapProjector->Update();

	cocos2d::Rect tex = widget->GetTownRect();
	float w = tex.size.width * widget->getScaleX();
	float h = tex.size.height * widget->getScaleY();
	
	widget->SetHitArea(-(w / 2.0f), -(h / 2.0f), w, h);
	widget->setTag(town->GetUid());
	widget->SetProjectorUid(uid);
	widget->retain();

	return widget;
}

InvestigatorMapWidget* WorldMapLayer::_CreateInvestigatorWidget(Investigator::Ptr investigator)
{
	InvestigatorMapWidget *widget = new InvestigatorMapWidget(investigator, _mapProjector);
	widget->retain();
	widget->setTag(investigator->GetUid());
	return widget;
}

Region::WeakPtr WorldMapLayer::_GetRegionUnderPoint(const Vector2& point) const
{
	Vector2 projectedClickPoint = _mapProjector->ProjectOnMap(point);
	for (Region::Ptr region : World::Instance().GetRegions())
	{
		const Region::HullsArray &array = region->GetHullsArray();

		for (const ArbitraryHull &hull : array)
		{
			if (hull.Contain(projectedClickPoint))
			{
				return region;
			}
		}
	}

	return Region::WeakPtr();
}

Cell::WeakPtr WorldMapLayer::_GetCellUnderPoint(const Vector2& point) const
{
	for (CellMapWidget *widget : _cellWidgetsList)
	{
		Cell::Ptr cell = widget->GetCell();
		if (cell->GetInfo().state != Cell::READY)
		{
			continue;
		}

		Vector2 projectedPoint = point - _mapProjector->ProjectOnScreen(cell->GetInfo().location);
		
		float xbegin, xend, ybegin, yend;
		widget->GetHitArea(xbegin, xend, ybegin, yend);

		cocos2d::Rect rect;
		rect.setRect(xbegin, ybegin, xend, yend);

		if (rect.containsPoint(projectedPoint))
		{
			return cell;
		}
	}

	return Cell::WeakPtr();
}

Town::WeakPtr WorldMapLayer::_GetTownUnderPoint(const Vector2& point)
{
	for (TownMapWidget *widget : _townWidgetsList)
	{
		Town::Ptr town = widget->GetTown();

		Vector2 projectedPoint = point - _mapProjector->ProjectOnScreen(town->GetLocation());
		
		float xbegin, xend, ybegin, yend;
		widget->GetHitArea(xbegin, xend, ybegin, yend);

		cocos2d::Rect rect;
		rect.setRect(xbegin, ybegin, xend, yend);

		if (rect.containsPoint(projectedPoint))
		{
			return town;
		}
	}

	return Town::WeakPtr();
}

void WorldMapLayer::_OnTownSelect(Town::WeakPtr town)
{
	bool avaliable = World::Instance().IsTownAvaliableToPlaceCell(town);

	if (!avaliable)
	{
		return;
	}

	Town::Ptr townptr = town.lock();

	if (!townptr->IsCellPresented())
	{
		if (World::Instance().IsFirstLaunch())
		{
			Cell::Info info;
			info.parent = nullptr;
			info.town = town;
			info.location = town.lock()->GetLocation();
			info.cash = GameInfo::Instance().GetInt("CELL_STARTUP_MONEY");
			info.morale = GameInfo::Instance().GetFloat("CELL_STARTUP_MORALE");
			info.contentment = GameInfo::Instance().GetFloat("CELL_STARTUP_CONTENTMENT");
			info.membersCount = GameInfo::Instance().GetInt("CELL_STARTUP_MEMBERS");
			info.constructionBegin = Utils::GetGameTime();
			info.constructionDuration = GameInfo::Instance().GetFloat("CELL_CONSTRUCTION_TIME");
			CreateCell(info, Cell::READY);

			World::Instance().SetFirstLaunch(false);
		}
		else if (!_nextCellParent.expired())
		{
			Cell::Info info;
			info.parent = _nextCellParent.lock().get();

			if (info.parent->GetInfo().cash >= GameInfo::Instance().GetInt("CELL_SPINOFF_CASH_PRICE") &&
				info.parent->GetInfo().membersCount >= GameInfo::Instance().GetInt("CELL_SPINOFF_MEMBERS_PRICE"))
			{
				info.town = town;
				info.location = town.lock()->GetLocation();
				info.cash = GameInfo::Instance().GetInt("CELL_STARTUP_MONEY");
				info.morale = GameInfo::Instance().GetFloat("CELL_STARTUP_MORALE");
				info.contentment = GameInfo::Instance().GetFloat("CELL_STARTUP_CONTENTMENT");
				info.membersCount = GameInfo::Instance().GetInt("CELL_STARTUP_MEMBERS");
				info.constructionBegin = Utils::GetGameTime();
				info.constructionDuration = GameInfo::Instance().GetFloat("CELL_CONSTRUCTION_TIME");

				info.parent->GetInfo().cash -= GameInfo::Instance().GetInt("CELL_SPINOFF_CASH_PRICE");
				info.parent->GetInfo().membersCount -= GameInfo::Instance().GetInt("CELL_SPINOFF_MEMBERS_PRICE");

				CreateCell(info, Cell::CONSTRUCTION);
			}

			_nextCellParent = Cell::Ptr();
		}
	}
}

void WorldMapLayer::ModifyZoom(float multiplier)
{
	_mapProjector->SetScale(_mapProjector->GetScale() * multiplier);
	_UpdateNetwork();
}

void WorldMapLayer::HideCellGameInterface(void)
{
	_cellMenu->DisappearWithAnimation();
}

void WorldMapLayer::_UpdateNetwork()
{
	if (!World::Instance().GetRootCell().expired())
	{
		_networkVisualiser->clear();
		_RecursiveUpdateNetworkVisualiser(_networkVisualiser, World::Instance().GetRootCell());
	}
}

void WorldMapLayer::_RecursiveUpdateNetworkVisualiser(cocos2d::DrawNode *visualiser, Cell::WeakPtr cell)
{
	Vector2 p1(_mapProjector->ProjectOnScreen(cell.lock()->GetInfo().location));

	for (Cell::Ptr child : cell.lock()->GetChildren())
	{
		Vector2 p2(_mapProjector->ProjectOnScreen(child->GetInfo().location));
		visualiser->drawSegment(p1, p2, 1.0f, Color(1.0f, 0.0f, 0.0f, 1.0f));

		// recursive
		_RecursiveUpdateNetworkVisualiser(visualiser, child);
	}
}
