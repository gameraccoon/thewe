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

cocos2d::Sprite* AddSpriteToProjector(MapProjector *projector, Vector2 location, Vector2 shift, std::string spriteName, bool dontScale, float scale = 1.0f)
{
	cocos2d::Sprite *sprite = cocos2d::Sprite::create(spriteName.c_str());
	sprite->retain();
	projector->AddMapPart(Drawable::CastFromCocos(sprite), location, shift, scale, dontScale);
	return sprite;
}

bool WorldMapLayer::init(void)
{
	if (!cocos2d::Layer::init())
	{
		return false;
	}
	
	_networkVisualiser = cocos2d::DrawNode::create();
	_UpdateNetwork();
	addChild(_networkVisualiser, 2);

	cocos2d::Sprite * spr = AddSpriteToProjector(_mapProjector, Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f), "WorldMap.png", false);
	addChild(spr);

	_cellHull.PushPoint(Vector2(-20.0f, -20.0f));
	_cellHull.PushPoint(Vector2(50.0f, -20.0f));
	_cellHull.PushPoint(Vector2(50.0f, 50.0f));
	_cellHull.PushPoint(Vector2(-20.0f, 50.0f));

	Vector2 origin = cocos2d::Director::sharedDirector()->getVisibleOrigin();
	Vector2 screen = cocos2d::Director::sharedDirector()->getVisibleSize();

	SetGuiEnabled(true);

	for (const Cell::Ptr cell : World::Instance().GetCells())
	{
		addChild(AddSpriteToProjector(_mapProjector, cell->GetInfo().location, Vector2(-15.0f, -10.0f), "pin.png", true, 0.6f), 2, (int)E_MAP_OBJECT_TAG::MAP_OBJ_CELL);
	}
	
	for (const Town::Ptr town : World::Instance().GetTowns())
	{
		addChild(AddSpriteToProjector(_mapProjector, town->GetLocation(), Vector2(-15.0f, -10.0f), "town.png",
			false, town->GetSpriteScale()), 1, (int)E_MAP_OBJECT_TAG::MAP_OBJ_TOWN);
	}
	

	// сообщаем где находится центр окна вывода
	_mapProjector->SetScreenCenter(origin + screen / 2.0f);
	// ставим спрайт карты ровно в центр экрана
	_mapProjector->SetLocation(Vector2(0.0f, 0.0f));
	// ставим скейл, чтобы экран правильно отмасштабировался
	_mapProjector->SetScale(1.0f);

	this->setTouchEnabled(true);
	this->setKeyboardEnabled(true);

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
		addChild(_mapGui, 3);
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

void WorldMapLayer::onTouchesBegan(const std::vector<cocos2d::Touch* > &touches, cocos2d::Event* event)
{
	if (_isInputEnabled)
	{
		cocos2d::Touch *touch = touches.at(0);//dynamic_cast<cocos2d::CCTouch*>(touches->anyObject());
		_touchLastPoint = touch->getLocation();
		_touchFirstPos = touch->getLocation();
	}
}

void WorldMapLayer::onTouchesEnded(const std::vector<cocos2d::Touch* > &touches, cocos2d::Event* event)
{
	if (_isInputEnabled)
	{
		cocos2d::Touch *touch = touches.at(0);//dynamic_cast<cocos2d::CCTouch *>(touches->anyObject());
		Vector2 point = touch->getLocation();
		Vector2 v = _touchFirstPos - point;
		
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

void WorldMapLayer::onTouchesMoved(const std::vector<cocos2d::Touch* > &touches, cocos2d::Event* event)
{
	_UpdateNetwork();

	if (_isInputEnabled)
	{
		cocos2d::Touch *touch = touches.at(0);//dynamic_cast<cocos2d::CCTouch*>(touches->anyObject());

		_mapProjector->ShiftView(-_touchLastPoint + touch->getLocation());
		_touchLastPoint = touch->getLocation();
	}
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
	for (Cell::Ptr cell : World::Instance().GetCells())
	{
		Vector2 projectedPoint = point - _mapProjector->ProjectOnScreen(cell->GetInfo().location);
		if (_cellHull.Contain(projectedPoint))
		{
			return cell;
		}
	}

	return Cell::WeakPtr();
}

Town::WeakPtr WorldMapLayer::_GetTownUnderPoint(const Vector2& point)
{
	for (Town::Ptr town : World::Instance().GetTowns())
	{
		Vector2 projectedPoint = point - _mapProjector->ProjectOnScreen(town->GetLocation());

		cocos2d::CCSprite *town_sprite = dynamic_cast<cocos2d::CCSprite *>(getChildByTag((int)E_MAP_OBJECT_TAG::MAP_OBJ_TOWN));
		if (!town_sprite)
		{
			return Town::Ptr();
		}

		cocos2d::Rect tex_rect = town_sprite->getTextureRect();

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
		info.location = town.lock()->GetLocation();
		info.cash = 100;
		info.morale = 1.0f;
		info.contentment = 0.1f;
		info.membersCount = 5;

		Cell::Ptr cell = std::make_shared<Cell>(Cell(info));
		World::Instance().AddCell(cell);
		addChild(AddSpriteToProjector(_mapProjector, cell->GetInfo().location, Vector2(-15.0f, -10.0f), "pin.png", true), 2, (int)E_MAP_OBJECT_TAG::MAP_OBJ_CELL);

		_mapProjector->Update();

		World::Instance().SetFirstLaunch(false);
	}
	else if (!_nextCellParent.expired())
	{
		Cell::Info info;
		info.parent = _nextCellParent.lock().get();
		info.town = town;
		info.location = town.lock()->GetLocation();
		info.cash = 100;
		info.morale = 1.0f;
		info.contentment = 0.1f;
		info.membersCount = 5;

		Cell::Ptr cell = Cell::Create(info);
		World::Instance().AddCell(cell);
		addChild(AddSpriteToProjector(_mapProjector, cell->GetInfo().location, Vector2(-15.0f, -10.0f), "pin.png", true), 2, (int)E_MAP_OBJECT_TAG::MAP_OBJ_CELL);

		_mapProjector->Update();

		_nextCellParent.lock()->AddChild(cell);
		_nextCellParent = Cell::WeakPtr();
	}
}

void WorldMapLayer::_DrawCellsLinksRecurcively(Cell::WeakPtr cell)
{
	Vector2 p1(_mapProjector->ProjectOnScreen(cell.lock()->GetInfo().location));

	for (Cell::Ptr child : cell.lock()->GetChildrens())
	{
		Vector2 p2(_mapProjector->ProjectOnScreen(child->GetInfo().location));
		cocos2d::ccDrawLine(p1, p2);

		_DrawCellsLinksRecurcively(child);
	}
}

void WorldMapLayer::ModifyZoom(float multiplier)
{
	_mapProjector->SetScale(_mapProjector->GetScale() * multiplier);
	_UpdateNetwork();
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

	for (Cell::Ptr child : cell.lock()->GetChildrens())
	{
		Vector2 p2(_mapProjector->ProjectOnScreen(child->GetInfo().location));
		visualiser->drawSegment(p1, p2, 4.0f, Color(1.0f, 0.0f, 0.0f, 1.0f));

		// рекурсивный вызов
		_RecursiveUpdateNetworkVisualiser(visualiser, child);
	}
}
