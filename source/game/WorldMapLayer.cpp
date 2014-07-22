#include "WorldMapLayer.h"

WorldMapLayer::WorldMapLayer()
{
}

bool WorldMapLayer::init(void)
{
	if (!cocos2d::CCLayer::init())
	{
		return false;
	}

	cocos2d::CCRect rect = _worldMap.getSprite()->getTextureRect();

	_mapShift = cocos2d::CCPoint(0.0f + rect.size.width/2.0f, 0.0f + rect.size.height/2.0f);
	_mapScale = 2.5f;

	_worldMap.getSprite()->setPosition(_mapShift);
	_worldMap.getSprite()->setScale(_mapScale);

	CCLayer::addChild(_worldMap.getSprite());
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
	_touchFirstPoint = touch->getLocation() - _worldMap.getSprite()->getPosition();

	if (_hull1.Contain(touch->getLocation()))
	{
		_isPointInHull = !_isPointInHull;
		_mapScale *= 1.25f;
	}
	else
	{
		_mapScale *= 0.8f;
	}
	_worldMap.getSprite()->setScale(_mapScale);
}

void WorldMapLayer::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	cocos2d::CCTouch *touch = static_cast<cocos2d::CCTouch*>(touches->anyObject());
	cocos2d::CCPoint projected = projectOnMap(touch->getLocation());
}

void WorldMapLayer::ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* pEvent)
{
	cocos2d::CCTouch *touch = static_cast<cocos2d::CCTouch*>(touches->anyObject());
	_mapShift = touch->getLocation() - _touchFirstPoint;
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
	_worldMap.getSprite()->setPosition(_mapShift);
}

cocos2d::CCPoint WorldMapLayer::projectOnMap(cocos2d::CCPoint screenPoint)
{
	return (screenPoint - _mapShift) / _mapScale;
}

cocos2d::CCPoint WorldMapLayer::projectOnScreen(cocos2d::CCPoint mapPoint)
{
	return (mapPoint * _mapScale) + _mapShift;
}
