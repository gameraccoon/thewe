#include "WorldMapScene.h"

bool WorldMapLayer::init(void)
{
	if (!cocos2d::CCLayer::init())
	{
		return false;
	}

	_movingSprite = cocos2d::CCSprite::create("../_gamedata/monster.png");
	
	cocos2d::CCRect rect = _movingSprite->getTextureRect();
	cocos2d::CCPoint pos(0.0f + rect.size.width/2.0f, 0.0f + rect.size.height/2.0f);
	_movingSprite->setPosition(pos);
	_movingSprite->setScale(2.5f);

	_touchPos = pos;

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
	cocos2d::CCTouch *touch = (cocos2d::CCTouch *)touches->anyObject();
	_touchPos = touch->getLocation();

	if (_hull1.Contain(_touchPos))
	{
		_isPointInHull = !_isPointInHull;
	}
	
	//CCLayer::ccTouchesBegan

	//const float duration = 0.4f;
	//cocos2d::CCMoveTo *move = cocos2d::CCMoveTo::create(duration, _touchPos);
	//_movingSprite->runAction(move); 
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
	cocos2d::CCPoint pos = _movingSprite->getPosition();

	cocos2d::CCPoint dir = _touchPos - pos;
	
	if (dir.getLength() >= 5.0f)
	{
		dir = dir.normalize();

		float velocity = 500.0f;
	
		pos.x += dir.x * velocity * timeDelta;
		pos.y += dir.y * velocity * timeDelta;

		_movingSprite->setPosition(pos);
	}
}

void WorldMapLayer::_MenuInputListener(cocos2d::CCObject *sender)
{
	cocos2d::CCMenuItemImage *item = (cocos2d::CCMenuItemImage *)sender;
	int key = item->getTag();
} 