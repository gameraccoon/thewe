#include "RegionInfoLayer.h"

#include "GameScene.h"

RegionInfoLayer::RegionInfoLayer(const Region::Info &info)
	:_regionInfo(info)			
{
	init();
}

bool RegionInfoLayer::init(void)
{
	if (!cocos2d::Layer::init())
	{
		return false;
	}

	cocos2d::Director *director = cocos2d::Director::sharedDirector();
	Vector2 screen = director->getVisibleSize();
	Vector2 origin = director->getVisibleOrigin();

	{
		using namespace cocos2d;
		_btnBack = cocos2d::CCMenuItemImage::create("btn-back-arrow-normal.png", "btn-back-arrow-selected.png",
			this, menu_selector(RegionInfoLayer::_MenuInputListener));
	}

	Vector2 pos;
	pos.x = origin.x + screen.x / 2.0f;
	pos.y = origin.y + screen.y - 100.0f;

	_btnBack->setTag(MENU_ITEM_BACK);
	_btnBack->setScale(5.0f);
	_btnBack->setPosition(pos - Vector2(-820.0f, 100.0f));

	char popul[64];
	char name[128];

	/*
#ifdef CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	sprintf_s(popul, "Pupulation %.1f millions", _regionInfo.population);
	sprintf_s(name, "Name %s", _regionInfo.name.c_str());
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	sprintf_l(popul, "Pupulation %.1f millions", _regionInfo.population);
	sprintf_l(name, "Name %s", _regionInfo.name.c_str());
#endif
*/

	_populationText = cocos2d::CCLabelTTF::create(popul, "Arial", 64);
	_regionNameText = cocos2d::CCLabelTTF::create(name, "Arial", 64);
	_shortDescText = cocos2d::CCLabelTTF::create(_regionInfo.desc.c_str(), "Arial", 64);

	_populationText->setPosition(Vector2(450.0f, screen.y - 100.0f));
	_regionNameText->setPosition(Vector2(450.0f, screen.y - 200.0f));
	_shortDescText->setPosition(Vector2(850.0f, screen.y - 800.0f));
	_shortDescText->setDimensions(Vector2(1500.0f, 1000.0f));
	_shortDescText->setHorizontalAlignment(cocos2d::kCCTextAlignmentLeft);

	cocos2d::CCMenu *menu = cocos2d::CCMenu::create(_btnBack, NULL);
	menu->setPosition(0.0f, 0.0f);

	_bkgDraw = cocos2d::CCDrawNode::create();
	_InitBackground(_bkgDraw);
	
	addChild(_bkgDraw, 0);
	addChild(menu, 1);
	addChild(_populationText, 1);
	addChild(_regionNameText, 1);
	addChild(_shortDescText, 1);
	setTouchEnabled(true);

	return true;
}

void RegionInfoLayer::_MenuInputListener(cocos2d::CCObject *sender)
{
	cocos2d::CCMenuItemImage *item = dynamic_cast<cocos2d::CCMenuItemImage*>(sender);

	int tag = item->getTag();

	switch (tag)
	{
	case MENU_ITEM_BACK:
		dynamic_cast<GameScene*>(getParent())->ShowMap();
		break;
	default: break;
	}
}

void RegionInfoLayer::_InitBackground(cocos2d::CCDrawNode *background) const
{
	if (!background)
	{
		return;
	}

	cocos2d::CCPoint screen = cocos2d::CCDirector::sharedDirector()->getVisibleSize();
	cocos2d::CCPoint origin = cocos2d::CCDirector::sharedDirector()->getVisibleOrigin();

	cocos2d::CCPoint vertices[4];
	vertices[0] = origin;
	vertices[1] = origin + cocos2d::CCPoint(screen.x, 0.0f);
	vertices[2] = origin + cocos2d::CCPoint(screen.x, screen.y);
	vertices[3] = origin + cocos2d::CCPoint(0.0f, screen.y);

	cocos2d::ccColor4F fill, border;
	
	fill.r = 0.3f;
	fill.g = 0.3f;
	fill.b = 0.3f;
	fill.a = 0.7f;
	
	border.r = 0.6f;
	border.g = 0.6f;
	border.b = 0.6f;
	border.a = 0.7f;

	background->drawPolygon(vertices, 4, fill, 50.0f, border);
}
