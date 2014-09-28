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
	, _cellMenu(nullptr)
	, _cellGameInterface(nullptr)
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
	addChild(_networkVisualiser, 2);

	cocos2d::Sprite * spr = AddSpriteToProjector(_mapProjector, Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f), "WorldMap.png", false);
	addChild(spr);

	Vector2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();
	Vector2 screen = cocos2d::Director::getInstance()->getVisibleSize();

	SetGuiEnabled(true);

	for (const Cell::Ptr cell : World::Instance().GetCells())
	{
		_AddCellToRender(cell);	
	}
	
	for (const Town::Ptr town : World::Instance().GetTowns())
	{
		cocos2d::Sprite *sprite = AddSpriteToProjector(_mapProjector, town->GetLocation(), Vector2(0.0f, 0.0f), "town.png",
			true, 0.17f);
		
		// обновляем проекцию чтобы узнать реальный размер элемента
		_mapProjector->Update();

		cocos2d::Rect tex = sprite->getTextureRect();
		float w = tex.size.width * sprite->getScaleX();
		float h = tex.size.height * sprite->getScaleY();
		town->SetHitArea(-(w / 2.0f), -(h / 2.0f), w, h);

		addChild(sprite, 1, town->GetUid());
	}
	
	// сообщаем где находится центр окна вывода
	_mapProjector->SetScreenCenter(origin + screen / 2.0f);
	// ставим спрайт карты ровно в центр экрана
	_mapProjector->SetLocation(Vector2(0.0f, 0.0f));
	// ставим скейл, чтобы экран правильно отмасштабировался
	_mapProjector->SetScale(1.0f);

	_cellMenu = new CellMenuSelector(_mapProjector, this);
	_cellMenu->autorelease();
	addChild(_cellMenu, 3);

	this->setTouchEnabled(true);
	this->setKeyboardEnabled(true);

	_UpdateNetwork();

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
		addChild(_mapGui, 4);
		_mapGui->autorelease();
		_mapProjector->SetScale(_mapProjector->GetScale());
	}
}

void WorldMapLayer::SetNextCellParent(Cell::WeakPtr parent)
{
	_nextCellParent = parent;
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

		if (_cellMenu->isVisible() && _GetCellUnderPoint(_touchLastPoint).expired())
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

			Cell::WeakPtr cell = _GetCellUnderPoint(point);

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

	// положение средней точки между всеми тачами
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
		_mapProjector->SetScale(_mapProjector->GetScale() * (newAvgDistance / _avgTouchDistance));
	}

	_avgTouchDistance = newAvgDistance;
	_touchesCenter = newTouchPoint;

	_isTouchesCountUpdated = false;
}

void WorldMapLayer::ResetTouches()
{
	_isTouchesCountUpdated = true;
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
		
		float xbegin, xend, ybegin, yend;
		cell->GetHitArea(xbegin, xend, ybegin, yend);

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
	for (Town::Ptr town : World::Instance().GetTowns())
	{
		Vector2 projectedPoint = point - _mapProjector->ProjectOnScreen(town->GetLocation());
		
		float xbegin, xend, ybegin, yend;
		town->GetHitArea(xbegin, xend, ybegin, yend);

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
		_AddCellToRender(cell);
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
		_AddCellToRender(cell);

		_mapProjector->Update();

		_nextCellParent.lock()->AddChild(cell);
		_nextCellParent = Cell::WeakPtr();

		_UpdateNetwork();
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

	for (Cell::Ptr child : cell.lock()->GetChildrens())
	{
		Vector2 p2(_mapProjector->ProjectOnScreen(child->GetInfo().location));
		visualiser->drawSegment(p1, p2, 4.0f, Color(1.0f, 0.0f, 0.0f, 1.0f));

		// рекурсивный вызов
		_RecursiveUpdateNetworkVisualiser(visualiser, child);
	}
}

void WorldMapLayer::_AddCellToRender(Cell::Ptr cell)
{
	cocos2d::Sprite *sprite = AddSpriteToProjector(_mapProjector, cell->GetInfo().location, Vector2(0.0f, 0.0f), "cell.png", true, 0.2f);
	
	// обновляем проекцию чтобы узнать реальный размер элемента
	_mapProjector->Update();

	cocos2d::Rect tex = sprite->getTextureRect();
	float w = tex.size.width * sprite->getScaleX();
	float h = tex.size.height * sprite->getScaleY();
	cell->SetHitArea(-(w / 2.0f), -(h / 2.0f), w, h);

	sprite->setTag(cell->GetUid());
	sprite->setUserData(static_cast<void *>(cell.get()));
	addChild(sprite, 2);
}
