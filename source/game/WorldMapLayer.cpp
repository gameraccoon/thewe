#include "WorldMapLayer.h"

#include "WorldMap.h"
#include "GameScene.h"
#include "MapGuiLayer.h"

WorldMapLayer::WorldMapLayer(MapProjector* projector)
	: _mapProjector(projector)
	, _isInputEnabled(true)
	, _mapGui(nullptr)
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

	Cell::Ptr cell1(std::make_shared<Cell>(Cell(Point(200.0f, 200.0f))));
	Cell::Ptr cell2(std::make_shared<Cell>(Cell(Point(100.0f, 100.0f))));
	cell1->AddChild(cell2);

	WorldMap::Instance().AddCell(cell1);
	WorldMap::Instance().AddCell(cell2);

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
		addChild(AddSpriteToProjector(_mapProjector, cell->GetLocation(), Point(-15.0f, -10.0f), "pin.png", true));
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
			Cell::Ptr cell = GetCellUnderPoint(point);
			
			if (cell)
			{
				dynamic_cast<GameScene*>(this->getParent())->ShowCellScreen(cell);
			}
			else
			{
				Region::Ptr region = GetRegionUnderPoint(point);

				if (region)
				{
					dynamic_cast<GameScene*>(this->getParent())->ShowRegionInfo(region);
				}
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

Region::Ptr WorldMapLayer::GetRegionUnderPoint(const Point& point) const
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

Cell::Ptr WorldMapLayer::GetCellUnderPoint(const Point& point) const
{
	for (Cell::Ptr cell : WorldMap::Instance().GetCells())
	{
		Point projectedPoint = point - _mapProjector->ProjectOnScreen(cell->GetLocation());
		if (_cellHull.Contain(projectedPoint))
		{
			return cell;
		}
	}

	return Cell::Ptr();
}

void WorldMapLayer::ModifyZoom(float multiplier)
{
	_mapProjector->SetScale(_mapProjector->GetScale() * multiplier);
}