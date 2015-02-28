#include "WorldMapLayer.h"

#include "GameScene.h"
#include "World.h"
#include "GameScene.h"
#include "TaskManager.h"
#include "SessionEndScreen.h"
#include "Log.h"
#include <luabind/luabind.hpp>

#include <LevelUpEffect.h>

static const float MAX_MAP_SCALE = 1.5f;
static const float INITIAL_TOWN_SCALE = 0.17f;
static const float INITIAL_CELL_SCALE = 0.2f;

WorldMapLayer::WorldMapLayer(GameScene *gameScene, MapProjector* projector)
	: _mapProjector(projector)
	, _isSessionScreenShowed(false)
	, _isMapMovementsEnabled(true)
	, _gameScene(gameScene)
	, _nextCellParent()
{
	MessageManager::Instance().RegisterReceiver(this, "AddInvestigatorWidget");
	MessageManager::Instance().RegisterReceiver(this, "DeleteInvestigatorWidget");
	MessageManager::Instance().RegisterReceiver(this, "DeleteCellWidget");
	MessageManager::Instance().RegisterReceiver(this, "SpinoffWithDragEnded");
	MessageManager::Instance().RegisterReceiver(this, "RelinkWithDragEnded");
	MessageManager::Instance().RegisterReceiver(this, "SpinoffWithDragBegan");
	MessageManager::Instance().RegisterReceiver(this, "RelinkWithDragBegan");
	MessageManager::Instance().RegisterReceiver(this, "EnableMapScrolling");
	MessageManager::Instance().RegisterReceiver(this, "DisableMapScrolling");
	MessageManager::Instance().RegisterReceiver(this, "RelinkCell");
	MessageManager::Instance().RegisterReceiver(this, "CreateCell");
	MessageManager::Instance().RegisterReceiver(this, "ScrollZoomIn");
	MessageManager::Instance().RegisterReceiver(this, "ScrollZoomOut");

	init();
}

WorldMapLayer::~WorldMapLayer(void)
{
}

bool WorldMapLayer::init(void)
{
	if (!cocos2d::Layer::init())
	{
		return false;
	}

	cocos2d::EventListenerTouchAllAtOnce *touches = cocos2d::EventListenerTouchAllAtOnce::create();
	touches->onTouchesBegan = CC_CALLBACK_2(WorldMapLayer::TouchesBegan, this);
	touches->onTouchesMoved = CC_CALLBACK_2(WorldMapLayer::TouchesMoved, this);
	touches->onTouchesEnded = CC_CALLBACK_2(WorldMapLayer::TouchesEnded, this);
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touches, this);

	_networkVisualiser = cocos2d::DrawNode::create();
	addChild(_networkVisualiser, Z_LINKS);

	cocos2d::Sprite *mapSprite = cocos2d::Sprite::create("gamefield/WorldMap.png");
	mapSprite->retain();
	_mapProjector->AddMapPart(Drawable::CastFromCocos(mapSprite), Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f), 1.0f, false);
	_mapProjector->SetMapSize(mapSprite->getTextureRect().size);
	addChild(mapSprite, Z_MAP);

	Vector2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();
	Vector2 screen = cocos2d::Director::getInstance()->getVisibleSize();


	for (const Cell::Ptr cell : World::Instance().GetCellsNetwork().GetActiveCells())
	{
		CellMapWidget *widget = CreateCellWidget(cell);
		_cellWidgets.push_back(widget);
		addChild(widget, Z_CELL);
	}

	for (const Town::Ptr town : World::Instance().GetTowns())
	{
		TownMapWidget *widget = CreateTownWidget(town);
		_townWidgets.push_back(widget);
		addChild(widget, Z_TOWN);
	}

	SetTownsVisibility(false);

	_effectsAbsolute = new EffectsLayer();
	_effectsAbsolute->autorelease();
	_effectsAbsolute->setPosition(0.0f, 0.0f);
	_effectsGameField = new EffectsLayer();
	_effectsGameField->autorelease();
	_effectsGameField->setPosition(0.0f, 0.0f);
	addChild(_effectsAbsolute, Z_EFFECTS_ABSOLUTE);
	addChild(_effectsGameField, Z_EFFECTS_GAME_FIELD);

	// say where is screen center
	_mapProjector->SetScreenCenter(origin + screen / 2.0f);
	// send map sprite to the center of world
	_mapProjector->SetLocation(Vector2(0.0f, 0.0f));
	// set scale to first time
	_mapProjector->SetScale(0.01f);

	scheduleUpdate();

	UpdateMapElements();

	/**
	Testing part - start
	*/

	// Если так создать иерархичесое отображение, то всё в порядке. Отобразится главный нод и его дочерний элемент
	cocos2d::Sprite * sp = cocos2d::Sprite::create("effects/cell_shining.png");
	cocos2d::Sprite * sp1 = cocos2d::Sprite::create("gamefield/cell.png");
	sp->addChild(sp1);
	addChild(sp, 10);
	sp->setPosition(this->getContentSize().width / 4, this->getContentSize().height / 2);

	// Если главный нод - DefaultLevelUpEffect, то ни он ни его дочерние элементы не будут отображаться.
	VisualEffects::DefaultLevelUpEffect * lu = VisualEffects::DefaultLevelUpEffect::create
		("effects/cell_shining.png", "effects/cell_yellow_rays.png", "effects/bonus_icon.png", "arial.ttf", 10, 10, this);
	addChild(lu);
	lu->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
	lu->init();

	/**
	Testing part - end
	*/

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

	SetTownsVisibility(World::Instance().IsFirstLaunch());
}

void WorldMapLayer::AcceptMessage(const Message &msg)
{
	if (msg.is("AddInvestigatorWidget"))
	{
		Investigator::Ptr investigator = World::Instance().GetInvestigatorByUid(msg.variables.GetInt("UID"));
		if (investigator)
		{
			InvestigatorMapWidget *widget = CreateInvestigatorWidget(investigator);
			addChild(widget, Z_INVESTIGATOR);
			_investigatorWidgets.push_back(widget);

			if (World::Instance().GetTutorialManager().IsTutorialStateAvailable("WaitForFirstInvestigator"))
			{
				dynamic_cast<GameScene*>(getParent())->MoveViewToPoint(
					investigator->GetInvestigationRoot().lock()->GetTown().lock()->GetLocation());
				UpdateMapElements();
			}
		}
	}
	else if (msg.is("DeleteInvestigatorWidget"))
	{
		for (InvestigatorWidgetsIter it = _investigatorWidgets.begin(); it != _investigatorWidgets.end(); ++it)
		{
			InvestigatorMapWidget *widget = (*it);
			if (widget->GetInvestigatorUid() == msg.variables.GetInt("UID"))
			{
				removeChild(widget);
				it = _investigatorWidgets.erase(it);
				break;
			}
		}
	}
	else if (msg.is("DeleteCellWidget"))
	{
		for (CellWidgetsIter it = _cellWidgets.begin(); it != _cellWidgets.end(); ++it)
		{
			CellMapWidget *widget = (*it);
			if (widget->GetCellUid() == msg.variables.GetInt("UID"))
			{
				removeChild(widget);
				_mapProjector->RemoveMapPart(widget->GetProjectorUid());
				_mapProjector->Update();
				UpdateMapElements();
				it = _cellWidgets.erase(it);
				delete widget;
				break;
			}
		}
	}
	else if (msg.is("SpinoffWithDragEnded") || msg.is("RelinkWithDragEnded"))
	{
		_isMapMovementsEnabled = true;
		SetTownsVisibility(msg.variables.GetBool("SHOW_TOWNS"));
	}
	else if (msg.is("SpinoffWithDragBegan") || msg.is("RelinkWithDragBegan"))
	{
		_isMapMovementsEnabled = false;
		SetTownsVisibility(msg.variables.GetBool("SHOW_TOWNS"));
	}
	else if (msg.is("EnableMapScrolling")) {
		_isMapMovementsEnabled = true;
	} else if (msg.is("DisableMapScrolling")) {
		_isMapMovementsEnabled = false;
	} else if (msg.is("ScrollZoomIn")) {
		ModifyZoom(1.25f);
	} else if (msg.is("ScrollZoomOut")) {
		ModifyZoom(0.8f);
	}
	else if (msg.is("RelinkCell"))
	{
		Cell::Ptr relink_cell = World::Instance().GetCellsNetwork().GetCellByUid(msg.variables.GetInt("RELINK_CELL_UID"));
		Cell::Ptr parent_cell = World::Instance().GetCellsNetwork().GetCellByUid(msg.variables.GetInt("PARENT_CELL_UID"));

		if (relink_cell->IsState(Cell::State::AUTONOMY) && parent_cell->IsState(Cell::State::READY))
		{
			relink_cell->SetState(Cell::State::READY);

			World::Instance().GetCellsNetwork().RelinkCells(parent_cell, relink_cell);

			UpdateMapElements();
		}
	}
	else if (msg.is("CreateCell"))
	{
		Town::WeakPtr town = World::Instance().GetTownByName(msg.variables.GetString("TOWN_NAME"));
		Cell::WeakPtr parent = World::Instance().GetCellsNetwork().GetCellByUid(msg.variables.GetInt("PARENT_UID"));

		if (parent.lock()->GetCash() >= GameInfo::Instance().GetInt("CELL_SPINOFF_CASH_PRICE") &&
			parent.lock()->GetMembersCount() >= GameInfo::Instance().GetInt("CELL_SPINOFF_MEMBERS_PRICE"))
		{
			Cell::Ptr cell = CreateCell(parent, town);

			cell->SetLocation(town.lock()->GetLocation());
			cell->SetCash(GameInfo::Instance().GetInt("CELL_STARTUP_MONEY"));
			cell->SetExperience(0);

			// test code
			for (int m=0;m<3;m++) {
				cell->AddMember(Member::create("thug", 3));
				cell->AddMember(Member::create("geek", 3));
			}

			parent.lock()->SetCash(parent.lock()->GetCash() - GameInfo::Instance().GetInt("CELL_SPINOFF_CASH_PRICE"));

			cell->SetState(Cell::State::CONSTRUCTION, Utils::GetGameTime(), GameInfo::Instance().GetTime("CELL_CONSTRUCTION_TIME"));

			const std::set<std::string>& test = World::Instance().GetTutorialManager().GetTutorialStatements();
			if (World::Instance().GetTutorialManager().IsTutorialStateAvailable("ReadyToCreateSpinoff"))
			{
				World::Instance().GetTutorialManager().RemoveCurrentTutorial();
				World::Instance().GetTutorialManager().RunTutorialFunction("OnCreateFirstSpinoff");
			}

			MessageManager::Instance().PutMessage(Message("SaveGame"));
		}
	}
}

void WorldMapLayer::SetNextCellParent(Cell::WeakPtr parent)
{
	_nextCellParent = parent;
}

Cell::Ptr WorldMapLayer::CreateCell(Cell::WeakPtr parent, Town::WeakPtr town)
{
	Cell::Ptr cell = Cell::Create(town);

	if (!parent.expired())
	{
		parent.lock()->AddChild(cell);
	}

	World::Instance().GetCellsNetwork().AddCell(cell);

	CellMapWidget *widget = CreateCellWidget(cell);
	_cellWidgets.push_back(widget);
	addChild(widget, Z_CELL);

	UpdateMapElements();

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

TownMapWidget* WorldMapLayer::GetNearestTownWidget(const Vector2 &pointOnScreen, float radius) const
{
	TownMapWidget *result = nullptr;

	float min_dist = std::numeric_limits<float>::max();
	for (TownWidgets::const_iterator iter = _townWidgets.begin(); iter != _townWidgets.end(); ++iter) {
		TownMapWidget *widget = (*iter);
		float dist = (*iter)->getPosition().getDistance(pointOnScreen);
		if (dist <= radius && dist < min_dist) {
			result = widget;
			min_dist = dist;
		}
	}

	return result;
}

CellMapWidget* WorldMapLayer::GetNearestCellWidget(const Vector2 &pointOnScreen, float radius) const
{
	CellMapWidget *result = nullptr;

	float min_dist = std::numeric_limits<float>::max();
	for (CellWidgets::const_iterator iter = _cellWidgets.begin(); iter != _cellWidgets.end(); ++iter) {
		CellMapWidget *widget = (*iter);
		float dist = (*iter)->getPosition().getDistance(pointOnScreen);
		if (dist <= radius && dist < min_dist) {
			result = widget;
			min_dist = dist;
		}
	}

	return result;
}

bool WorldMapLayer::IsCellMenuOpened(void) const
{
	return false;
}

bool WorldMapLayer::IsCellMenuOpenedFor(Cell::WeakPtr cell) const
{
	if (IsCellMenuOpened()) {
		return false;
	}
	return false;
}

bool WorldMapLayer::IsCellMenuSpinoffMode(void) const
{
	return false;
}

void WorldMapLayer::AddEffectAbsolute(Effect *effect)
{
	_effectsAbsolute->AddEffect(effect);
}

void WorldMapLayer::AddEffectGameField(Effect *effect)
{
	_effectsGameField->AddEffect(effect);
}

void WorldMapLayer::menuCloseCallback(cocos2d::Ref *Sender)
{
}

void WorldMapLayer::TouchesBegan(const std::vector<cocos2d::Touch* > &touches, cocos2d::Event* event)
{
	cocos2d::Touch *touch = touches.at(0);
	_touchLastPoint = touch->getLocation();
	_touchFirstPos = touch->getLocation();

	if (GetCellUnderPoint(touch->getLocation()).expired()) {
		MessageManager::Instance().PutMessage(Message("CloseCellMenu"));
	}

	ResetTouches();
}

void WorldMapLayer::TouchesEnded(const std::vector<cocos2d::Touch* > &touches, cocos2d::Event* event)
{
	if (touches.size() == 1)
	{
		cocos2d::Touch *touch = touches.at(0);
		Vector2 point = touch->getLocation();
		Vector2 v = _touchFirstPos - point;

		/*
		if (GetCellUnderPoint(point).expired()) {
			MessageManager::Instance().PutMessage(Message("CloseCellMenu"));
		}
		*/

		const float size = v.Size();
		const float tolerance = 5.0f;

		if (size <= tolerance)
		{
			Town::WeakPtr town = GetTownUnderPoint(point);
			if (!town.expired())
			{
				OnTownSelect(town);
				return;
			}
		}
	}
}

void WorldMapLayer::TouchesMoved(const std::vector<cocos2d::Touch* > &touches, cocos2d::Event* event)
{
	if (!_isMapMovementsEnabled) {
		return;
	}

	if (touches.size() > 0)
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

	UpdateMapElements();
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
		{
			_mapProjector->SetScale(std::min(MAX_MAP_SCALE, _mapProjector->GetScale() * (newAvgDistance / _avgTouchDistance)));
		}
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

CellMapWidget* WorldMapLayer::CreateCellWidget(Cell::Ptr cell)
{
	CellMapWidget *widget = new CellMapWidget(this, _mapProjector, cell);

	int uid = _mapProjector->AddMapPart(Drawable::CastFromCocos(widget), cell->GetTown().lock()->GetLocation(),
		Vector2(0.0f, 0.0f), INITIAL_CELL_SCALE, true);
	_mapProjector->Update();

	cocos2d::Rect tex = widget->GetCellRect();
	float w = tex.size.width * widget->getScaleX();
	float h = tex.size.height * widget->getScaleY();

	widget->SetHitArea(cocos2d::Rect(-(w / 2.0f), -(h / 2.0f), w, h));
	widget->SetProjectorUid(uid);
	widget->retain();

	return widget;
}

TownMapWidget* WorldMapLayer::CreateTownWidget(Town::Ptr town)
{
	TownMapWidget *widget = new TownMapWidget(town);

	int uid = _mapProjector->AddMapPart(Drawable::CastFromCocos(widget), town->GetInfo().location, Vector2(0.0f, 0.0f), INITIAL_TOWN_SCALE, true);
	_mapProjector->Update();

	cocos2d::Rect tex = widget->GetTownRect();
	float w = tex.size.width * widget->getScaleX();
	float h = tex.size.height * widget->getScaleY();

	widget->SetHitArea(cocos2d::Rect(-(w / 2.0f), -(h / 2.0f), w, h));
	widget->SetProjectorUid(uid);
	widget->retain();

	return widget;
}

InvestigatorMapWidget* WorldMapLayer::CreateInvestigatorWidget(Investigator::Ptr investigator)
{
	InvestigatorMapWidget *widget = new InvestigatorMapWidget(investigator, _mapProjector, this);
	widget->autorelease();
	return widget;
}

Region::WeakPtr WorldMapLayer::GetRegionUnderPoint(const Vector2& point) const
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

Cell::WeakPtr WorldMapLayer::GetCellUnderPoint(const Vector2& point)
{
	for (CellMapWidget *widget : _cellWidgets)
	{
		Cell::Ptr cell = widget->GetCell().lock();

		if (!cell)
		{
			WRITE_WARN("Dead cell under point");
			return Cell::WeakPtr();
		}

		Vector2 projectedPoint = point - _mapProjector->ProjectOnScreen(cell->GetLocation());

		cocos2d::Rect rect = widget->GetHitArea();

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

Town::WeakPtr WorldMapLayer::GetTownUnderPoint(const Vector2& point)
{
	for (TownMapWidget *widget : _townWidgets)
	{
		Town::Ptr town = widget->GetTown();

		Vector2 projectedPoint = point - _mapProjector->ProjectOnScreen(town->GetLocation());

		cocos2d::Rect rect = widget->GetHitArea();

		if (rect.containsPoint(projectedPoint))
		{
			return town;
		}
	}

	return Town::WeakPtr();
}

void WorldMapLayer::OnTownSelect(Town::WeakPtr town)
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
			Cell::Ptr cell = CreateCell(Cell::WeakPtr(), town);

			cell->SetLocation(town.lock()->GetLocation());
			cell->SetCash(GameInfo::Instance().GetInt("CELL_STARTUP_MONEY"));
			cell->SetExperience(0);
			cell->SetState(Cell::State::READY);

			// test code
			for (int m=0;m<3;m++) {
				cell->AddMember(Member::create("thug", 3));
				cell->AddMember(Member::create("geek", 3));
			}

			World::Instance().SetFirstLaunch(false);
			World::Instance().GetCellsNetwork().SetRootCell(cell);

			if (World::Instance().GetTutorialManager().IsTutorialStateAvailable("FirstCell"))
			{
				World::Instance().GetTutorialManager().RemoveCurrentTutorial();
				World::Instance().GetTutorialManager().RunTutorialFunction("AfterCreatingFirstCell");
			}

			MessageManager::Instance().PutMessage(Message("SaveGame"));
		}
	}
}

void WorldMapLayer::SetTownsVisibility(bool visibility)
{
	for (TownMapWidget *widget : _townWidgets) {
		widget->SetTownImageVisible(visibility);
	}
}

void WorldMapLayer::ModifyZoom(float multiplier)
{
	_mapProjector->SetScale(_mapProjector->GetScale() * multiplier);
	UpdateMapElements();
}

void WorldMapLayer::HideCellGameInterface(void)
{
}

void WorldMapLayer::UpdateMapElements()
{
	UpdateTowns();
	UpdateCells();
	UpdateNetwork();
}

void WorldMapLayer::UpdateCells()
{
	Vector2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();
	Vector2 screen = cocos2d::Director::getInstance()->getVisibleSize();
	Vector2 visibleSize = origin + screen;
	cocos2d::Rect screenRect(0.0f, 0.0f, visibleSize.x, visibleSize.y);

	CellWidgets widgetsToScale;

	for (auto cellWidget : _cellWidgets)
	{
		Vector2 firstPoint = cellWidget->getPosition() - cellWidget->getContentSize() * INITIAL_CELL_SCALE;
		Vector2 secondPoint = cellWidget->getPosition() + cellWidget->getContentSize() * INITIAL_CELL_SCALE;

		cocos2d::Rect rect = cocos2d::Rect(firstPoint.x, firstPoint.y, secondPoint.x, secondPoint.y);

		if (cellWidget->isVisible() && rect.intersectsRect(screenRect))
		{
			widgetsToScale.push_back(cellWidget);
		}
	}

	if (widgetsToScale.size() > 1)
	{
		float minDistance = 999999.0f;
		for (int i = 0, xMax = widgetsToScale.size() - 2; i <= xMax; i++)
			for (int j = i + 1, yMax = widgetsToScale.size() - 1; j <= yMax; j++)
		{
			float dist = Vector2(widgetsToScale[i]->getPosition() - widgetsToScale[j]->getPosition()).Size();
			if (dist < minDistance)
			{
				minDistance = dist;
			}
		}

		float calcScale = minDistance * 0.005;
		float scale = INITIAL_CELL_SCALE < calcScale ? INITIAL_CELL_SCALE : calcScale;
		for (auto widget : widgetsToScale)
		{
			widget->setScale(scale);
		}
	}
}

void WorldMapLayer::UpdateTowns()
{
	Vector2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();
	Vector2 screen = cocos2d::Director::getInstance()->getVisibleSize();
	Vector2 visibleSize = origin + screen;
	cocos2d::Rect screenRect(0.0f, 0.0f, visibleSize.x, visibleSize.y);

	TownWidgets widgetsToScale;

	for (auto townWidget : _townWidgets)
	{
		Vector2 firstPoint = townWidget->getPosition() - townWidget->getContentSize() * INITIAL_TOWN_SCALE;
		Vector2 secondPoint = townWidget->getPosition() + townWidget->getContentSize() * INITIAL_TOWN_SCALE;

		cocos2d::Rect rect = cocos2d::Rect(firstPoint.x, firstPoint.y, secondPoint.x, secondPoint.y);

		if (townWidget->isVisible() && rect.intersectsRect(screenRect))
		{
			widgetsToScale.push_back(townWidget);
		}
	}

	if (widgetsToScale.size() > 1)
	{
		float minDistance = 999999.0f;
		for (int i = 0, xMax = widgetsToScale.size() - 2; i <= xMax; i++)
			for (int j = i + 1, yMax = widgetsToScale.size() - 1; j <= yMax; j++)
		{
			float dist = Vector2(widgetsToScale[i]->getPosition() - widgetsToScale[j]->getPosition()).Size();
			if (dist < minDistance)
			{
				minDistance = dist;
			}
		}

		float calcScale = minDistance * 0.005;
		float scale = INITIAL_TOWN_SCALE < calcScale ? INITIAL_TOWN_SCALE : calcScale;
		for (auto widget : widgetsToScale)
		{
			widget->setScale(scale);
		}
	}
}

void WorldMapLayer::UpdateNetwork()
{
	if (!World::Instance().GetCellsNetwork().GetRootCell().expired())
	{
		_networkVisualiser->clear();
		RecursiveUpdateNetworkVisualiser(_networkVisualiser, World::Instance().GetCellsNetwork().GetRootCell());
	}
}

void WorldMapLayer::RecursiveUpdateNetworkVisualiser(cocos2d::DrawNode *visualiser, Cell::WeakPtr cell)
{
	Vector2 p1(_mapProjector->ProjectOnScreen(cell.lock()->GetLocation()));

	for (Cell::WeakPtr child : cell.lock()->GetChildren())
	{
		Cell::Ptr childPtr = child.lock();
		if (childPtr)
		{
			Vector2 p2(_mapProjector->ProjectOnScreen(childPtr->GetLocation()));
			visualiser->drawSegment(p1, p2, 1.0f, Color(1.0f, 0.0f, 0.0f, 1.0f));

			// recursive
			RecursiveUpdateNetworkVisualiser(visualiser, child);
		}
	}
}
