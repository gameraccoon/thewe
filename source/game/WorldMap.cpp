#include "WorldMap.h"

bool WorldMapLayer::init(void)
{
	if (!cocos2d::CCLayer::init())
	{
		return false;
	}

	cocos2d::CCSize visible_size = cocos2d::CCDirector::sharedDirector()->getVisibleSize();
	cocos2d::CCPoint origin = cocos2d::CCDirector::sharedDirector()->getVisibleOrigin();
	
	cocos2d::CCSprite *sprite = cocos2d::CCSprite::create("../_gamedata/WorldMap.png");
	sprite->setPosition(ccp(visible_size.width/2.0f + origin.x, visible_size.height/2.0f + origin.y));
	addChild(sprite);

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