#include "RegionInfoScene.h"

#include "RegionInfoLayer.h"

RegionInfoScene::RegionInfoScene(void)
{
	init();
}

bool RegionInfoScene::init(void)
{
	if (!CCScene::init())
	{
		return false;
	}
	
	cocos2d::CCDirector *director = cocos2d::CCDirector::sharedDirector();
	cocos2d::CCSize screen = director->getVisibleSize();
	cocos2d::CCPoint origin = director->getVisibleOrigin();

	RegionInfoLayer *region = new RegionInfoLayer();

	addChild(region);

	return true;
}

void RegionInfoScene::_MenuInputListener(cocos2d::CCObject *sender)
{
}