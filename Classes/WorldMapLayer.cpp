#include "WorldMapLayer.h"

#include "GameScene.h"
#include "World.h"
#include "GameScene.h"
#include "MapGuiLayer.h"
#include "TaskManager.h"
#include "SessionEndScreen.h"
#include "WorldLoader.h"
#include "Log.h"
#include <luabind/luabind.hpp>

static const float MAX_MAP_SCALE = 1.5f;

WorldMapLayer::WorldMapLayer(GameScene *gameScene, MapProjector* projector)
	: _mapProjector(projector)
	, _isInputEnabled(true)
	, _isSessionScreenShowed(false)
	, _mapGui(nullptr)
	, _gameScene(gameScene)
	, _nextCellParent()
	, _linkCellChildren()
	, _cellMenu(nullptr)
	, _cellGameInterface(nullptr)
{
	init();
}

WorldMapLayer::~WorldMapLayer(void)
{
	MessageManager::Instance().UnregisterReceiver(dynamic_cast<MessageReceiver *>(this));
}

bool WorldMapLayer::init(void)
{
	if (!cocos2d::Layer::init())
	{
		return false;
	}

	MessageManager::Instance().RegisterReceiver(dynamic_cast<MessageReceiver *>(this));

	cocos2d::EventListenerTouchAllAtOnce *touches = cocos2d::EventListenerTouchAllAtOnce::create();
	touches->onTouchesBegan = CC_CALLBACK_2(WorldMapLayer::TouchesBegan, this);
	touches->onTouchesMoved = CC_CALLBACK_2(WorldMapLayer::TouchesMoved, this);
	touches->onTouchesEnded = CC_CALLBACK_2(WorldMapLayer::TouchesEnded, this);
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touches, this);

	_townsDrawLayer = cocos2d::Layer::create();
	_townsDrawLayer->setPosition(0.0f, 0.0f);
	_townsDrawLayer->setVisible(true);
	addChild(_townsDrawLayer, Z_TOWN);

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

	for (const Cell::Ptr cell : World::Instance().GetCellsNetwork().GetActiveCells())
	{
		CellMapWidget *widget = _CreateCellWidget(cell);
		_cellWidgets.push_back(widget);
		addChild(widget, Z_CELL);
	}
	
	for (const Town::Ptr town : World::Instance().GetTowns())
	{
		TownMapWidget *widget = _CreateTownWidget(town);
		_townWidgets.push_back(widget);
		_townsDrawLayer->addChild(widget, 0);
	}
	
	// say where is screen center
	_mapProjector->SetScreenCenter(origin + screen / 2.0f);
	// send map sprite to the center of world
	_mapProjector->SetLocation(Vector2(0.0f, 0.0f));
	// set scale to first time
	_mapProjector->SetScale(0.01f);

	_cellMenu = CellMenuSelector::create(_mapProjector, this);
	addChild(_cellMenu, Z_CELL_MENU);

	scheduleUpdate();

	_UpdateNetwork();

	return true;
}

void WorldMapLayer::update(float dt)
{
	if (!_isSessionScreenShowed)
	{
		if (World::Instance().IsGameOver())
		{
			PushSessionFailScreen();
			_isSessionScreenShowed = true;
		}
		else if (World::Instance().GetWorldCapturingState() >= 1.0f)
		{
			PushSessionWinScreen();
			_isSessionScreenShowed = true;
		}
	}

	// allow to draw towns olny if cell spinoff creation enabled
	_townsDrawLayer->setVisible(!_nextCellParent.expired() || World::Instance().IsFirstLaunch());
}

void WorldMapLayer::AcceptMessage(const Message &msg)
{
	if (msg.name == "AddInvestigatorWidget")
	{
		Investigator::Ptr investigator = World::Instance().GetInvestigatorByUid(msg.param);
		if (investigator)
		{
			InvestigatorMapWidget *widget = _CreateInvestigatorWidget(investigator);
			addChild(widget, Z_INVESTIGATOR);
			_investigatorWidgets.push_back(widget);

			if (World::Instance().GetTutorialState() == "WaitForCatchingFirstInvestigator")
			{
				dynamic_cast<GameScene*>(getParent())->MoveViewToPoint(
					investigator->GetInvestigationRoot().lock()->GetInfo().town.lock()->GetLocation());
				_UpdateNetwork();
			}
		}
	}
	else if (msg.name == "DeleteInvestigatorWidget")
	{
		for (InvestigatorWidgetsIter it = _investigatorWidgets.begin(); it != _investigatorWidgets.end(); ++it)
		{
			InvestigatorMapWidget *widget = (*it);
			if (widget->GetInvestigatorUid() == msg.param)
			{
				removeChild(widget);
				it = _investigatorWidgets.erase(it);
				break;
			}
		}
	}
	else if (msg.name == "DeleteCellWidget")
	{
		for (CellWidgetsIter it = _cellWidgets.begin(); it != _cellWidgets.end(); ++it)
		{
			CellMapWidget *widget = (*it);
			if (widget->GetCellUid() == msg.param)
			{
				removeChild(widget);
				_mapProjector->RemoveMapPart(widget->GetProjectorUid());
				_mapProjector->Update();
				_UpdateNetwork();
				it = _cellWidgets.erase(it);
				delete widget;
				break;
			}
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

void WorldMapLayer::SetLinkCellChildren(Cell::WeakPtr children)
{
	_linkCellChildren = children;
}

Cell::Ptr WorldMapLayer::CreateCell(Cell::Info info, Cell::State state)
{	
	info.state = state;

	Cell::Ptr cell = Cell::Create(info);
	
	Cell::Ptr parent = info.parent.lock();
	if (parent)
	{
		parent->AddChild(cell);
	}

	World::Instance().GetCellsNetwork().AddCell(cell);

	CellMapWidget *widget = _CreateCellWidget(cell);
	_cellWidgets.push_back(widget);
	addChild(widget, Z_CELL);

	_UpdateNetwork();

	return cell;
}

void WorldMapLayer::PushSessionFailScreen(void)
{
	CustomSessionEndScreen::ConstructionInfo desc;
	desc.bigMessage = "Network Destroyed";
	desc.shortDesc = "Your actions have led to the declassification of the network.";
	desc.backgrndColor = cocos2d::Color3B(255, 0, 0);
	desc.messageColor = cocos2d::Color3B(255, 36, 0);
	desc.showStaticstics = true;
	desc.callback = CC_CALLBACK_1(WorldMapLayer::BackToMainMenuCallback, this);

	CustomSessionEndScreen *screen = new CustomSessionEndScreen(desc);
	screen->setPosition(0.0f, 0.0f);
	screen->autorelease();

	SetMapInputEnabled(false);
	SetGuiEnabled(false);

	addChild(screen, Z_MAP_GUI);
}

void WorldMapLayer::PushSessionWinScreen(void)
{
	CustomSessionEndScreen::ConstructionInfo desc;
	desc.bigMessage = "World Absorbed";
	desc.shortDesc = "Your network keeps the whole world under control.";
	desc.backgrndColor = cocos2d::Color3B(0, 200, 0);
	desc.messageColor = cocos2d::Color3B(0, 200, 36);
	desc.showStaticstics = true;
	desc.callback = CC_CALLBACK_1(WorldMapLayer::BackToMainMenuCallback, this);

	CustomSessionEndScreen *screen = new CustomSessionEndScreen(desc);
	screen->setPosition(0.0f, 0.0f);
	screen->autorelease();

	SetMapInputEnabled(false);
	SetGuiEnabled(false);

	addChild(screen, Z_MAP_GUI);
}

CellMapWidget* WorldMapLayer::GetCellMapWidget(Cell::WeakPtr cell) const
{
	Cell::Ptr cellHardPtr = cell.lock();
	for (CellMapWidget *widget : _cellWidgets)
	{
		if (widget->GetCell().lock() == cellHardPtr)
		{
			return widget;
		}
	}

	return nullptr;
}

void WorldMapLayer::menuCloseCallback(cocos2d::Ref *Sender)
{
}

void WorldMapLayer::TouchesBegan(const std::vector<cocos2d::Touch* > &touches, cocos2d::Event* event)
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

void WorldMapLayer::TouchesEnded(const std::vector<cocos2d::Touch* > &touches, cocos2d::Event* event)
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
				Cell::Ptr cell = _GetCellUnderPoint(point).lock();
				if (cell)
				{
					if (  !_linkCellChildren.expired() 
						&& _linkCellChildren.lock()->IsState(Cell::State::AUTONOMY) 
						&& cell != _linkCellChildren.lock() 
						&& cell->IsState(Cell::State::READY))
					{
						Cell::Ptr relinked = _linkCellChildren.lock();
						relinked->GetInfo().state = Cell::State::READY;
						relinked->GetInfo().stateBegin = 0;
						relinked->GetInfo().stateDuration = 0;

						World::Instance().GetCellsNetwork().RelinkCells(cell, relinked);
						_linkCellChildren.reset();

						_UpdateNetwork();
					}
					else
					{
						if (World::Instance().GetCellsNetwork().IsCellRelinkable(cell) && cell->IsState(Cell::State::AUTONOMY)) {
							_linkCellChildren = cell;
						}

						Vector2 cell_pos = Vector2(-700, 200);//cell->GetInfo().location;
						Vector2 menu_pos = _mapProjector->ProjectOnScreen(cell_pos);

						_cellMenu->DisappearImmedaitely();
						_cellMenu->AppearWithAnimation(cell, menu_pos);
					}

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

void WorldMapLayer::TouchesMoved(const std::vector<cocos2d::Touch* > &touches, cocos2d::Event* event)
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

void WorldMapLayer::BackToMainMenuCallback(cocos2d::Ref *sender)
{
	_gameScene->GoToMainMenu();
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
	widget->SetProjectorUid(uid);
	widget->retain();

	return widget;
}

InvestigatorMapWidget* WorldMapLayer::_CreateInvestigatorWidget(Investigator::Ptr investigator)
{
	InvestigatorMapWidget *widget = new InvestigatorMapWidget(investigator, _mapProjector, this);
	widget->autorelease();
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

Cell::WeakPtr WorldMapLayer::_GetCellUnderPoint(const Vector2& point)
{
	for (CellMapWidget *widget : _cellWidgets)
	{
		Cell::Ptr cell = widget->GetCell().lock();

		if (!cell)
		{
			Log::Instance().writeWarning("Dead cell under point");
			return Cell::WeakPtr();
		}

		Vector2 projectedPoint = point - _mapProjector->ProjectOnScreen(cell->GetInfo().location);
		
		float xbegin, xend, ybegin, yend;
		widget->GetHitArea(xbegin, xend, ybegin, yend);

		cocos2d::Rect rect;
		rect.setRect(xbegin, ybegin, xend, yend);

		if (rect.containsPoint(projectedPoint))
		{
			if (!cell->IsState(Cell::State::READY) && !cell->IsState(Cell::State::AUTONOMY)) {
				return Cell::WeakPtr();	
			}

			return cell;
		}
	}

	return Cell::WeakPtr();
}

Town::WeakPtr WorldMapLayer::_GetTownUnderPoint(const Vector2& point)
{
	for (TownMapWidget *widget : _townWidgets)
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
			info.parent = Cell::WeakPtr();
			info.town = town;
			info.location = town.lock()->GetLocation();
			info.cash = GameInfo::Instance().GetInt("CELL_STARTUP_MONEY");
			info.morale = GameInfo::Instance().GetFloat("CELL_STARTUP_MORALE");
			info.devotion = GameInfo::Instance().GetFloat("CELL_STARTUP_DEVOTION");
			info.membersCount = GameInfo::Instance().GetInt("CELL_STARTUP_MEMBERS");
			info.ratsCount = GameInfo::Instance().GetInt("CELL_STARTUP_RATS_COUNT");
			info.techUnitsCount = GameInfo::Instance().GetInt("CELL_STARTUP_TECH_UNITS_COUNT");
			info.experience = 0;
			info.fame = 0.0f;
			info.stateBegin = 0;
			info.stateDuration = 0;
			info.specialization = Cell::Specialization::NORMAL;
			info.townHeartPounding = GameInfo::Instance().GetFloat("CELL_STARTUP_TOWN_HEART_POUNDING");
			info.townInfluence = GameInfo::Instance().GetFloat("CELL_STARTUP_TOWN_INFLUENCE");
			info.townWelfare = GameInfo::Instance().GetFloat("CELL_STARTUP_TOWN_WELFARE");
			Cell::Ptr cell = CreateCell(info, Cell::State::READY);

			World::Instance().SetFirstLaunch(false);
			World::Instance().GetCellsNetwork().SetRootCell(cell);

			if (World::Instance().GetTutorialState() == "FirstCell")
			{
				World::Instance().RunTutorialFunction("AfterCreatingFirstCell");
			}

			MessageManager::Instance().PutMessage(Message("SaveGame"));
		}
		else if (Cell::Ptr parent = _nextCellParent.lock())
		{

			if (parent->GetInfo().cash >= GameInfo::Instance().GetInt("CELL_SPINOFF_CASH_PRICE") &&
				parent->GetInfo().membersCount >= GameInfo::Instance().GetInt("CELL_SPINOFF_MEMBERS_PRICE"))
			{
				Cell::Info info;
				if (World::Instance().GetTutorialState() == "ReadyToCreateSpinoff")
				{
					World::Instance().RunTutorialFunction("OnCreateFirstSpinoff");
				}

				info.parent = parent;
				info.town = town;
				info.location = town.lock()->GetLocation();
				info.cash = GameInfo::Instance().GetInt("CELL_STARTUP_MONEY");
				info.morale = GameInfo::Instance().GetFloat("CELL_STARTUP_MORALE");
				info.devotion = GameInfo::Instance().GetFloat("CELL_STARTUP_DEVOTION");
				info.membersCount = GameInfo::Instance().GetInt("CELL_STARTUP_MEMBERS");
				info.ratsCount = GameInfo::Instance().GetInt("CELL_STARTUP_RATS_COUNT");
				info.techUnitsCount = GameInfo::Instance().GetInt("CELL_STARTUP_TECH_UNITS_COUNT");
				info.experience = 0;
				info.fame = 0.0f;
				info.specialization = Cell::Specialization::NORMAL;
				info.townHeartPounding = GameInfo::Instance().GetFloat("CELL_STARTUP_TOWN_HEART_POUNDING");
				info.townInfluence = GameInfo::Instance().GetFloat("CELL_STARTUP_TOWN_INFLUENCE");
				info.townWelfare = GameInfo::Instance().GetFloat("CELL_STARTUP_TOWN_WELFARE");

				parent->GetInfo().cash -= GameInfo::Instance().GetInt("CELL_SPINOFF_CASH_PRICE");
				parent->GetInfo().membersCount -= GameInfo::Instance().GetInt("CELL_SPINOFF_MEMBERS_PRICE");

				info.stateBegin = Utils::GetGameTime();
				info.stateDuration = GameInfo::Instance().GetTime("CELL_CONSTRUCTION_TIME");

				CreateCell(info, Cell::State::CONSTRUCTION);

				MessageManager::Instance().PutMessage(Message("SaveGame"));
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
	if (!World::Instance().GetCellsNetwork().GetRootCell().expired())
	{
		_networkVisualiser->clear();
		_RecursiveUpdateNetworkVisualiser(_networkVisualiser, World::Instance().GetCellsNetwork().GetRootCell());
	}
}

void WorldMapLayer::_RecursiveUpdateNetworkVisualiser(cocos2d::DrawNode *visualiser, Cell::WeakPtr cell)
{
	Vector2 p1(_mapProjector->ProjectOnScreen(cell.lock()->GetInfo().location));

	for (Cell::WeakPtr child : cell.lock()->GetChildren())
	{
		Cell::Ptr childPtr = child.lock();
		if (childPtr)
		{
			Vector2 p2(_mapProjector->ProjectOnScreen(childPtr->GetInfo().location));
			visualiser->drawSegment(p1, p2, 1.0f, Color(1.0f, 0.0f, 0.0f, 1.0f));

			// recursive
			_RecursiveUpdateNetworkVisualiser(visualiser, child);
		}
	}
}
