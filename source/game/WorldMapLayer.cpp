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
