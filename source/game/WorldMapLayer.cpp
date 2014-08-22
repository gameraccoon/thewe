#include "WorldMapLayer.h"

#include "GameScene.h"
#include "World.h"
#include "GameScene.h"
#include "MapGuiLayer.h"
#include "TaskManager.h"
#include "Log.h"

WorldMapLayer::WorldMapLayer(GameScene *gameScene, MapProjector* projector)
	: _mapProjector(projector)
	, _isInputEnabled(true)
	, _mapGui(nullptr)
	, _gameScene(gameScene)
	, _nextCellParent()
{
	init();
}

cocos2d::CCSprite* AddSpriteToProjector(MapProjector *projector, Point location, Point shift, std::string spriteName, bool dontScale, float scale = 1.0f)
{
	cocos2d::CCSprite *sprite = new cocos2d::CCSprite();
	sprite->initWithFile(spriteName.c_str());
	projector->AddMapPart(Drawable::CastFromCocos(sprite), location, shift, scale, dontScale);
	return sprite;
}

bool WorldMapLayer::init(void)
{
	if (!cocos2d::CCLayer::init())
	{
		return false;
	}
	
	cocos2d::CCSprite * spr = AddSpriteToProjector(_mapProjector, Point(0.0f, 0.0f), Point(0.0f, 0.0f), "WorldMap.png", false);


	addChild(spr);
	setTouchEnabled(true);
    setKeypadEnabled(true);

	_cellHull.PushPoint(Point(-20.0f, -20.0f));
	_cellHull.PushPoint(Point(50.0f, -20.0f));
	_cellHull.PushPoint(Point(50.0f, 50.0f));
	_cellHull.PushPoint(Point(-20.0f, 50.0f));

	Point origin = cocos2d::CCDirector::sharedDirector()->getVisibleOrigin();
	Point screen = cocos2d::CCDirector::sharedDirector()->getVisibleSize();

	SetGuiEnabled(true);

	for (const Cell::Ptr cell : World::Instance().GetCells())
	{
		addChild(AddSpriteToProjector(_mapProjector, cell->GetInfo().location, Point(-15.0f, -10.0f), "pin.png", true), 2, MAP_OBJ_CELL);
	}
	
	for (const Town::Ptr town : World::Instance().GetTowns())
	{
		addChild(AddSpriteToProjector(_mapProjector, town->GetLocation(), Point(-15.0f, -10.0f), "town.png",
			false, town->GetSpriteScale()), 1, MAP_OBJ_TOWN);
	}
	
	// сообщаем где находится центр окна вывода
	_mapProjector->SetScreenCenter(origin + screen / 2.0f);
	// ставим спрайт карты ровно в центр экрана
	_mapProjector->SetLocation(Point(0.0f, 0.0f));
	// ставим скейл, чтобы экран правильно отмасштабировался
	_mapProjector->SetScale(1.0f);

	return true;
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
		addChild(_mapGui);
		_mapGui->autorelease();
		_mapProjector->SetScale(_mapProjector->GetScale());
	}
}

void WorldMapLayer::SetNextCellParent(Cell::WeakPtr parent)
{
	_nextCellParent = parent;
}

void WorldMapLayer::menuCloseCallback(cocos2d::CCObject *Sender)
{
}

void WorldMapLayer::ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	if (_isInputEnabled)
	{
		cocos2d::CCTouch *touch = dynamic_cast<cocos2d::CCTouch*>(touches->anyObject());
		_touchLastPoint = touch->getLocation();
		_touchFirstPos = touch->getLocation();
	}
}

void WorldMapLayer::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	if (_isInputEnabled)
	{
		cocos2d::CCTouch *touch = dynamic_cast<cocos2d::CCTouch *>(touches->anyObject());
		Point point = touch->getLocation();
		Point v = _touchFirstPos - point;
		
		const float size = v.Size();
		const float tolerance = 5.0f;

		if (size <= tolerance)
		{
			Cell::WeakPtr cell = _GetCellUnderPoint(point);
			if (!cell.expired())
			{
				dynamic_cast<GameScene*>(this->getParent())->ShowCellScreen(cell);
				return;
			}

			Town::WeakPtr town = _GetTownUnderPoint(point);
			_gameScene->ShowTownInfo(town);
			if (!town.expired())
			{
				_OnTownSelect(town);
				return;
			}

			Region::WeakPtr region = _GetRegionUnderPoint(point);
			if (!region.expired())
			{
				dynamic_cast<GameScene*>(this->getParent())->ShowRegionInfo(region);
				return;
			}
		}
	}
}

void WorldMapLayer::ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	if (_isInputEnabled)
	{
		cocos2d::CCTouch *touch = dynamic_cast<cocos2d::CCTouch*>(touches->anyObject());

		_mapProjector->ShiftView(-_touchLastPoint + touch->getLocation());
		_touchLastPoint = touch->getLocation();
	}
}

void WorldMapLayer::visit()
{
	CCLayer::visit();

	for (Region::Ptr region : World::Instance().GetRegions())
	{
		const Region::HullsArray &array = region->GetHullsArray();

		for (const ArbitraryHull &hull : array)
		{
			// медленная операция
			_mapProjector->ProjectOnScreen(hull).Draw();
		}
	}

	Cell::WeakPtr root = World::Instance().GetRootCell();

	if (!root.expired())
	{
		Color color(1.0f, 0.0f, 0.0f);
		cocos2d::ccDrawColor4F(color.r, color.g, color.b, color.a);

		_DrawCellsLinksRecurcively(root);
	}
}

Region::WeakPtr WorldMapLayer::_GetRegionUnderPoint(const Point& point) const
{
	Point projectedClickPoint = _mapProjector->ProjectOnMap(point);
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

Cell::WeakPtr WorldMapLayer::_GetCellUnderPoint(const Point& point) const
{
	for (Cell::Ptr cell : World::Instance().GetCells())
	{
		Point projectedPoint = point - _mapProjector->ProjectOnScreen(cell->GetInfo().location);
		if (_cellHull.Contain(projectedPoint))
		{
			return cell;
		}
	}

	return Cell::WeakPtr();
}

Town::WeakPtr WorldMapLayer::_GetTownUnderPoint(const Point& point)
{
	for (Town::Ptr town : World::Instance().GetTowns())
	{
		Point projectedPoint = point - _mapProjector->ProjectOnScreen(town->GetLocation());

		cocos2d::CCSprite *town_sprite = dynamic_cast<cocos2d::CCSprite *>(getChildByTag(MAP_OBJ_TOWN));
		if (!town_sprite)
		{
			return Town::Ptr();
		}

		cocos2d::CCRect tex_rect = town_sprite->getTextureRect();

		float actual_w = tex_rect.size.width * town_sprite->getScaleX();
		float actual_h = tex_rect.size.height * town_sprite->getScaleY();

		cocos2d::CCRect rect;
		rect.setRect(-(actual_w/2.0f), -(actual_h/2.0f), actual_w, actual_h);

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

	if (World::Instance().IsFirstLaunch())
	{
		Cell::Info info;
		info.parent = nullptr;
		info.town = town;
		info.location = town._Get()->GetLocation();
		info.cash = 100;
		info.morale = 1.0f;
		info.contentment = 0.1f;
		info.membersCount = 5;

		Cell::Ptr cell = Cell::Create(info);
		World::Instance().AddCell(cell);
		addChild(AddSpriteToProjector(_mapProjector, cell->GetInfo().location, Point(-15.0f, -10.0f), "pin.png", true), 2, MAP_OBJ_CELL);

		_mapProjector->Update();

		World::Instance().SetFirstLaunch(false);
	}
	else if (!_nextCellParent.expired())
	{
		Cell::Info info;
		info.parent = _nextCellParent._Get();
		info.town = town;
		info.location = town._Get()->GetLocation();
		info.cash = 100;
		info.morale = 1.0f;
		info.contentment = 0.1f;
		info.membersCount = 5;

		Cell::Ptr cell = Cell::Create(info);
		World::Instance().AddCell(cell);
		addChild(AddSpriteToProjector(_mapProjector, cell->GetInfo().location, Point(-15.0f, -10.0f), "pin.png", true), 2, MAP_OBJ_CELL);

		_mapProjector->Update();

		_nextCellParent._Get()->AddChild(cell);
		_nextCellParent = Cell::WeakPtr();
	}
}

void WorldMapLayer::_DrawCellsLinksRecurcively(Cell::WeakPtr cell)
{
	Point p1(_mapProjector->ProjectOnScreen(cell._Get()->GetInfo().location));

	for (Cell::Ptr child : cell._Get()->GetChildrens())
	{
		Point p2(_mapProjector->ProjectOnScreen(child->GetInfo().location));
		cocos2d::ccDrawLine(p1, p2);

		_DrawCellsLinksRecurcively(child);
	}
}

void WorldMapLayer::ModifyZoom(float multiplier)
{
	_mapProjector->SetScale(_mapProjector->GetScale() * multiplier);
}