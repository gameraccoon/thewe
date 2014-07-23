#include "WorldMapLayer.h"

WorldMapLayer::WorldMapLayer()
	: _mapProjector(cocos2d::CCPoint(0.0f, 0.0f), 2.5f)
{
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

	// фикс проблемы с обязательным using
	{
		using namespace cocos2d;
		CCLayer::schedule(schedule_selector(WorldMapLayer::_IdleUpdate));
	}

	_hull1.AddPoint(ccp(100, 100));
	_hull1.AddPoint(ccp(500, 900));
	_hull1.AddPoint(ccp(700, 1000));
	_hull1.AddPoint(ccp(900, 950));
	_hull1.AddPoint(ccp(900, 750));
	_hull1.AddPoint(ccp(600, 750));
	_hull1.AddPoint(ccp(600, 400));
	_hull1.AddPoint(ccp(900, 400));

	_isPointInHull = false;

	return true;
}

cocos2d::CCScene* WorldMapLayer::scene(void)
{
	cocos2d::CCScene *scene = cocos2d::CCScene::create();

	WorldMapLayer *layer = WorldMapLayer::create();

	scene->addChild(layer);

	return scene;
}

void  WorldMapLayer::menuCloseCallback(cocos2d::CCObject *Sender)
{
}

void WorldMapLayer::ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	cocos2d::CCTouch *touch = static_cast<cocos2d::CCTouch*>(touches->anyObject());
	_touchLastPoint = touch->getLocation();

	if (_hull1.Contain(touch->getLocation()))
	{
		_isPointInHull = !_isPointInHull;
	}
}

void WorldMapLayer::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
}

void WorldMapLayer::ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* pEvent)
{
	cocos2d::CCTouch *touch = static_cast<cocos2d::CCTouch*>(touches->anyObject());
	_mapProjector.SetShift(_mapProjector.GetShift() - _touchLastPoint + touch->getLocation());
	_touchLastPoint = touch->getLocation();
}

void WorldMapLayer::visit(void)
{
	cocos2d::ccColor4F color;
	if (_isPointInHull)
	{
		color.r = 0.0f;
		color.g = 0.6f;
		color.b = 1.0f;
		color.a = 0.7f;
	}
	else
	{
		color.r = 1.0f;
		color.g = 0.6f;
		color.b = 0.0f;
		color.a = 0.7f;
	}

	_hull1.Draw(color);

	CCLayer::visit();
}

void WorldMapLayer::_IdleUpdate(float timeDelta)
{
}
