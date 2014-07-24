#include "WorldMapScene.h"

WorldMapLayer::WorldMapLayer(void)
	: _mapProjector(cocos2d::CCPoint(0.0f, 0.0f), 2.5f)
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
	
	_worldMap.AddRegion("Italy", std::make_shared<Region>(Region()));
	Region::Ptr region = _worldMap.GetRegion("Italy");
	ConvexHullTest hull = region->GetHull();
	hull.AddPoint(ccp(100, 100));
	hull.AddPoint(ccp(500, 800));
	hull.AddPoint(ccp(700, 900));
	hull.AddPoint(ccp(900, 800));
	hull.AddPoint(ccp(900, 750));
	hull.AddPoint(ccp(600, 750));
	hull.AddPoint(ccp(600, 400));
	hull.AddPoint(ccp(900, 400));
	region->SetHull(hull);
	_mapProjector.SetScale(4.0f);

	cocos2d::CCPoint origin = cocos2d::CCDirector::sharedDirector()->getVisibleOrigin();
	cocos2d::CCSize screen = cocos2d::CCDirector::sharedDirector()->getVisibleSize();

	_mapProjector.SetScreenCenter(origin + screen / 2.0f);

	return true;
}

void WorldMapLayer::menuCloseCallback(cocos2d::CCObject *Sender)
{
}

void WorldMapLayer::ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	cocos2d::CCTouch *touch = static_cast<cocos2d::CCTouch*>(touches->anyObject());
	_touchLastPoint = touch->getLocation();
}

void WorldMapLayer::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
}

void WorldMapLayer::ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	cocos2d::CCTouch *touch = (cocos2d::CCTouch *)touches->anyObject();
	_mapProjector.SetShift(_mapProjector.GetShift() - _touchLastPoint + touch->getLocation());
	_touchLastPoint = touch->getLocation();
}

void WorldMapLayer::visit()
{
	CCLayer::visit();

	for (auto regionIterator : _worldMap.GetRegions())
	{
		ConvexHullTest hull = regionIterator.second->GetHull();
		ConvexHullTest projectedHull;
		for (auto &point : hull.GetPoints())
		{
			projectedHull.AddPoint(_mapProjector.ProjectOnScreen(point));
		}
		projectedHull.Draw();
	}
}
