#include "WorldMapLayer.h"

using namespace cocos2d;

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

	CCLayer::addChild(_movingSprite);
	CCLayer::setTouchEnabled(true);
	CCLayer::schedule(schedule_selector(WorldMapLayer::_IdleUpdate));

	//CCLayer::ccTouchMoved

	//cocos2d::CCSize visible_size = cocos2d::CCDirector::sharedDirector()->getVisibleSize();
	//cocos2d::CCPoint origin = cocos2d::CCDirector::sharedDirector()->getVisibleOrigin();
	
	//cocos2d::CCSprite *sprite = cocos2d::CCSprite::create("../_gamedata/Image.png");
	//sprite->setPosition(ccp(visible_size.width/2.0f + origin.x, visible_size.height/2.0f + origin.y));
	//addChild(sprite);

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

void WorldMapLayer::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	cocos2d::CCTouch *touch = (cocos2d::CCTouch *)touches->anyObject();
	_touchPos = touch->getLocation();

	//const float duration = 0.4f;
	//cocos2d::CCMoveTo *move = cocos2d::CCMoveTo::create(duration, _touchPos);
	//_movingSprite->runAction(move);
}

void WorldMapLayer::ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
	_touchPos = touch->getLocation();
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