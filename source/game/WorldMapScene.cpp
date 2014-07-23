#include "WorldMapScene.h"

bool WorldMapLayer::init(void)
{
	if (!cocos2d::CCLayer::init())
	{
		return false;
	}



	cocos2d::CCDirector *director = cocos2d::CCDirector::sharedDirector();
	cocos2d::CCSize screen = director->getVisibleSize();
	cocos2d::CCPoint origin = director->getVisibleOrigin();

	_printPos = cocos2d::CCLabelTTF::create("X: 0, Y: 0", "Arial", 32);
	_printPos->setPosition(cocos2d::CCPoint(origin.x + 200, origin.y + screen.height - 100));

	_printNum = cocos2d::CCLabelTTF::create("Num Points: 0", "Arial", 32);
	_printNum->setPosition(cocos2d::CCPoint(origin.x + 200, origin.y + screen.height - 150));

	CCLayer::addChild(_printPos);
	CCLayer::addChild(_printNum);
	CCLayer::addChild(_movingSprite);
	CCLayer::setTouchEnabled(true);
	//CCLayer::schedule(schedule_selector(WorldMapLayer::_IdleUpdate));

	/*_hull1.AddPoint(ccp(100, 100));
	_hull1.AddPoint(ccp(500, 900));
	_hull1.AddPoint(ccp(700, 1000));
	_hull1.AddPoint(ccp(900, 950));
	_hull1.AddPoint(ccp(900, 750));
	_hull1.AddPoint(ccp(600, 750));
	_hull1.AddPoint(ccp(600, 400));
	_hull1.AddPoint(ccp(900, 400));*/

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

void WorldMapLayer::menuCloseCallback(cocos2d::CCObject *Sender)
{
}

void WorldMapLayer::ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	cocos2d::CCTouch *touch = (cocos2d::CCTouch *)touches->anyObject();

	_hull1.AddPoint(touch->getLocation());

	char string[64];
	sprintf_s(string, "Num Points: %d", _hull1.GetPointsNum());
	_printNum->setString(string);
}

void WorldMapLayer::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
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
}

void WorldMapLayer::ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	cocos2d::CCTouch *touch = (cocos2d::CCTouch *)touches->anyObject();
	cocos2d::CCPoint point = touch->getLocation();

	char string[64];
	sprintf_s(string, "X: %d, Y: %d", (int)point.x, (int)point.y);
	_printPos->setString(string);
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

void WorldMapLayer::_MenuInputListener(cocos2d::CCObject *sender)
{
	cocos2d::CCMenuItemImage *item = (cocos2d::CCMenuItemImage *)sender;
	int key = item->getTag();
} 