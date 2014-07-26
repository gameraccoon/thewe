#include "WorldMapScene.h"

#include "WorldMap.h"

#include "RegionInfoScene.h"

WorldMapLayer::WorldMapLayer(void)
	: _mapProjector(cocos2d::CCPoint(0.0f, 0.0f), 3.0f)
{
	init();
}

bool WorldMapLayer::init(void)
{
	if (!cocos2d::CCLayer::init())
	{
		return false;
	}

	CCLayer::addChild(_mapProjector.GetSprite());
	CCLayer::setTouchEnabled(true);
    CCLayer::setKeypadEnabled(true);

	cocos2d::CCPoint origin = cocos2d::CCDirector::sharedDirector()->getVisibleOrigin();
	cocos2d::CCSize screen = cocos2d::CCDirector::sharedDirector()->getVisibleSize();

	// сообщаем где находится центр окна вывода
	_mapProjector.SetScreenCenter(origin + screen / 2.0f);
	// ставим спрайт карты ровно в центр экрана
	_mapProjector.SetShift(origin + screen / 2.0f);

	return true;
}

void WorldMapLayer::menuCloseCallback(cocos2d::CCObject *Sender)
{
}

void WorldMapLayer::ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	cocos2d::CCTouch *touch = dynamic_cast<cocos2d::CCTouch*>(touches->anyObject());
	_touchLastPoint = touch->getLocation();
	_tappedRegion = GetRegionUnderPoint(touch->getLocation());
}

void WorldMapLayer::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	cocos2d::CCTouch *touch = dynamic_cast<cocos2d::CCTouch*>(touches->anyObject());

	if (GetRegionUnderPoint(touch->getLocation()) == _tappedRegion && _tappedRegion != nullptr)
	{
		cocos2d::CCDirector *director = cocos2d::CCDirector::sharedDirector();
		cocos2d::CCScene *scene= new RegionInfoScene();
		director->pushScene(scene);
	}
}

void WorldMapLayer::ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	cocos2d::CCTouch *touch = dynamic_cast<cocos2d::CCTouch*>(touches->anyObject());

	_mapProjector.SetShift(_mapProjector.GetShift() - _touchLastPoint + touch->getLocation());
	_touchLastPoint = touch->getLocation();
}

void WorldMapLayer::visit()
{
	CCLayer::visit();

	for (auto regionIterator : WorldMap::Instance().GetRegions())
	{
		ArbitraryHull hull = regionIterator.second->GetHull();
		ArbitraryHull projectedHull;
		for (auto &point : hull.GetPoints())
		{
			projectedHull.PushPoint(_mapProjector.ProjectOnScreen(point));
		}
		projectedHull.Draw();
	}
}

Region::Ptr WorldMapLayer::GetRegionUnderPoint(const cocos2d::CCPoint& point)
{
	for (auto regionIterator : WorldMap::Instance().GetRegions())
	{
		ArbitraryHull hull = regionIterator.second->GetHull();
		if (hull.Contain(_mapProjector.ProjectOnMap(point)))
		{
			return regionIterator.second;
		}
	}
	return Region::Ptr();
}
