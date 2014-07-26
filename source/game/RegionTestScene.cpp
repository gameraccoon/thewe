#include "RegionTestScene.h"

#include "RegionDrawLayer.h"

RegionTestScene::RegionTestScene(void)
{
	init();
}

bool RegionTestScene::init(void)
{
	if (!CCScene::init())
	{
		return false;
	}
	
	cocos2d::CCDirector *director = cocos2d::CCDirector::sharedDirector();
	cocos2d::CCSize screen = director->getVisibleSize();
	cocos2d::CCPoint origin = director->getVisibleOrigin();

	RegionDrawLayer *region = new RegionDrawLayer();

	addChild(region);

	return true;
}

void RegionTestScene::_MenuInputListener(cocos2d::CCObject *sender)
{
}