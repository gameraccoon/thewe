#include "RegionInfoLayer.h"

#include "WorldMap.h"

RegionInfoLayer::RegionInfoLayer(void)
{
	init();
}

bool RegionInfoLayer::init(void)
{
	if (!CCLayer::init())
	{
		return false;
	}

	cocos2d::CCDirector *director = cocos2d::CCDirector::sharedDirector();
	cocos2d::CCSize screen = director->getVisibleSize();
	cocos2d::CCPoint origin = director->getVisibleOrigin();

	_btnBack = cocos2d::CCMenuItemImage::create("../_gamedata/btn-save-normal.png",
		"../_gamedata/btn-save-selected.png", this, menu_selector(RegionInfoLayer::_MenuInputListener));

	CCPoint pos;
	pos.x = origin.x + screen.width / 2.0f;
	pos.y = origin.y + screen.height - 100.0f;

	_btnBack->setTag(MENU_ITEM_BACK);
	_btnBack->setPosition(pos - ccp(-700.0f, 500.0f));

	cocos2d::CCMenu *menu = cocos2d::CCMenu::create(_btnBack, NULL);
	menu->setPosition(0.0f, 0.0f);
	
	addChild(menu);
	setTouchEnabled(true);

	return true;
}

void RegionInfoLayer::ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	cocos2d::CCLayer::ccTouchesBegan(touches, event);
}

void RegionInfoLayer::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	cocos2d::CCLayer::ccTouchesEnded(touches, event);
}

void RegionInfoLayer::ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	cocos2d::CCLayer::ccTouchesMoved(touches, event);
}

void RegionInfoLayer::_MenuInputListener(cocos2d::CCObject *sender)
{
	cocos2d::CCMenuItemImage *item = dynamic_cast<cocos2d::CCMenuItemImage*>(sender);

	int tag = item->getTag();

	switch (tag)
	{
	case MENU_ITEM_BACK:
		cocos2d::CCDirector::sharedDirector()->popScene();
		break;
	default: break;
	}
}