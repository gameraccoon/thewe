#include "WorldMapLayer.h"

#include "WorldMap.h"
#include "GameScene.h"
#include "MapGuiLayer.h"

WorldMapLayer::WorldMapLayer(MapProjector* projector)
	: _mapProjector(projector)
	, _isInputEnabled(true)
	, _mapGui(nullptr)
	, _isPlacingCell(true)
{
	init();
}

cocos2d::CCSprite* AddSpriteToProjector(MapProjector *projector, Point location, Point shift, std::string spriteName, bool dontScale = false)
{
	cocos2d::CCSprite *sprite = new cocos2d::CCSprite();
	sprite->initWithFile(spriteName.c_str());
	projector->AddMapPart(Drawable::CastFromCocos(sprite), location, shift, 1.0f, dontScale);
	return sprite;
}

bool WorldMapLayer::init(void)
{
	if (!cocos2d::CCLayer::init())
	{
		return false;
	}
	
	cocos2d::CCSprite * spr = AddSpriteToProjector(_mapProjector, Point(0.0f, 0.0f), Point(0.0f, 0.0f), "WorldMap.png");
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

	for (const Cell::Ptr cell : WorldMap::Instance().GetCells())
	{
		_AddCellToRender(cell);
	}
	
	// сообщаем где находится центр окна вывода
	_mapProjector->SetScreenCenter(origin + screen / 2.0f);
	// ставим спрайт карты ровно в центр экрана
	_mapProjector->SetLocation(Point(0.0f, 0.0f));
	// обновляем положение всех элементов на экране
	_mapProjector->Update();

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
			Cell::Ptr cell = _GetCellUnderPoint(point);
			
			if (cell)
			{
				dynamic_cast<GameScene*>(this->getParent())->ShowCellScreen(cell);
				return;
			}
			Region::Ptr region = _GetRegionUnderPoint(point);

			if (region)
			{
				if (!_isPlacingCell)
				{
					dynamic_cast<GameScene*>(this->getParent())->ShowRegionInfo(region);
				}
				else
				{
					Cell::Info cellInfo;
					cellInfo.location = _mapProjector->ProjectOnMap(point);
					cellInfo.region = _GetRegionUnderPoint(point);
					Cell::Ptr cell = std::make_shared<Cell>(Cell(cellInfo));
					_AddCellToRender(cell);
					WorldMap::Instance().AddCell(cell);
					_mapProjector->Update();
				}
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

	for (Region::Ptr region : WorldMap::Instance().GetRegions())
	{
		const Region::HullsArray &array = region->GetHullsArray();

		for (const ArbitraryHull &hull : array)
		{
			// медленная операция
			_mapProjector->ProjectOnScreen(hull).Draw();
		}
	}
}

Region::Ptr WorldMapLayer::_GetRegionUnderPoint(const Point& point) const
{
	Point projectedClickPoint = _mapProjector->ProjectOnMap(point);
	for (Region::Ptr region : WorldMap::Instance().GetRegions())
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

	return Region::Ptr();
}

Cell::Ptr WorldMapLayer::_GetCellUnderPoint(const Point& point) const
{
	for (Cell::Ptr cell : WorldMap::Instance().GetCells())
	{
		Point projectedPoint = point - _mapProjector->ProjectOnScreen(cell->GetInfo().location);
		if (_cellHull.Contain(projectedPoint))
		{
			return cell;
		}
	}

	return Cell::Ptr();
}

void WorldMapLayer::_AddCellToRender(Cell::Ptr cell)
{
	addChild(AddSpriteToProjector(_mapProjector, cell->GetInfo().location, Point(-15.0f, -10.0f), "pin.png", true));
}

void WorldMapLayer::ModifyZoom(float multiplier)
{
	_mapProjector->SetScale(_mapProjector->GetScale() * multiplier);
}