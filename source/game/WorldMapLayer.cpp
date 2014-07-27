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

bool WorldMapLayer::init(void)
{
	if (!cocos2d::CCLayer::init())
	{
		return false;
	}

	addChild(_mapProjector->GetSprite());
	setTouchEnabled(true);
    setKeypadEnabled(true);

	cocos2d::CCPoint origin = cocos2d::CCDirector::sharedDirector()->getVisibleOrigin();
	cocos2d::CCSize screen = cocos2d::CCDirector::sharedDirector()->getVisibleSize();

	SetGuiEnabled(true);

	// сообщаем где находится центр окна вывода
	_mapProjector->SetScreenCenter(origin + screen / 2.0f);
	// ставим спрайт карты ровно в центр экрана
	_mapProjector->SetShift(origin + screen / 2.0f);

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
		_mapGui = new MapGuiLayer();
		addChild(_mapGui);
		_mapGui->autorelease();
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
		_isTapTouch = true;
	}
}

void WorldMapLayer::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	if (_isInputEnabled)
	{
		if (_isTapTouch)
		{
			dynamic_cast<GameScene*>(this->getParent())->ShowRegionInfo("Italy");
		}
	}
}

void WorldMapLayer::ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	if (_isInputEnabled)
	{
		cocos2d::CCTouch *touch = dynamic_cast<cocos2d::CCTouch*>(touches->anyObject());

		_mapProjector->SetShift(_mapProjector->GetShift() - _touchLastPoint + touch->getLocation());
		_touchLastPoint = touch->getLocation();

		_isTapTouch = false;
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
			ArbitraryHull projectedHull;
			for (auto &point : hull.GetPoints())
			{
				projectedHull.PushPoint(_mapProjector->ProjectOnScreen(point));
			}
			
			projectedHull.Draw();
		}
	}
}

Region::Ptr WorldMapLayer::GetRegionUnderPoint(const cocos2d::CCPoint& point)
{
	for (Region::Ptr region : WorldMap::Instance().GetRegions())
	{
		const Region::HullsArray &array = region->GetHullsArray();

		for (const ArbitraryHull &hull : array)
		{
			if (hull.Contain(_mapProjector->ProjectOnMap(point)))
			{
				return region;
			}
		}
	}

	return Region::Ptr();
}


void WorldMapLayer::ModifyZoom(float multiplier)
{
	_mapProjector->SetScale(_mapProjector->GetScale() * multiplier);
}