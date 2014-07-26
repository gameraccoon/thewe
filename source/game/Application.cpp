#include "Application.h"

#include "GameScene.h"

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
	cocos2d::CCDirector *director = cocos2d::CCDirector::sharedDirector();
	cocos2d::CCEGLView *glview = cocos2d::CCEGLView::sharedOpenGLView();

	director->setOpenGLView(glview);
	director->setDisplayStats(true);
	director->setAnimationInterval(1.0 / 60.0);

	_menuScene = cocos2d::CCScene::create();
	
	_btnRunWorldTestScene = CCMenuItemImage::create("../_gamedata/btn-map-normal.png",
		"../_gamedata/btn-map-selected.png", _menuScene, menu_selector(AppDelegate::_MenuInputListener)); 
	_btnRunCountryScene = CCMenuItemImage::create("../_gamedata/btn-region-normal.png",
		"../_gamedata/btn-region-selected.png", _menuScene, menu_selector(AppDelegate::_MenuInputListener)); 

	CCSize client = director->getVisibleSize();
	CCPoint origin = director->getVisibleOrigin();
	CCPoint pos;
	pos.x = origin.x + client.width / 2.0f;
	pos.y = origin.y + client.height - 100.0f;

	_btnRunWorldTestScene->setPosition(pos - cocos2d::CCPoint(0.0f, 100.0f));
	_btnRunWorldTestScene->setTag(MENU_ITEM_RUN_WORLD_SCENE);
	_btnRunWorldTestScene->setScale(5.0f);
	_btnRunCountryScene->setPosition(pos - cocos2d::CCPoint(0.0f, 400.0f));
	_btnRunCountryScene->setTag(MENU_ITEM_RUN_REGION_SCENE);
	_btnRunCountryScene->setScale(5.0f);

	_mainMenu = CCMenu::create(_btnRunWorldTestScene, _btnRunCountryScene, NULL);
	_mainMenu->setPosition(0.0f, 0.0f);

	_menuScene->addChild(_mainMenu);

	director->runWithScene(_menuScene);

	return true;
}

void AppDelegate::applicationDidEnterBackground()
{
}

void AppDelegate::applicationWillEnterForeground()
{
}

void AppDelegate::_MenuInputListener(CCObject *sender)
{
	CCDirector *director = CCDirector::sharedDirector();
	CCScene *scene = NULL;

	cocos2d::CCMenuItemImage *item = dynamic_cast<cocos2d::CCMenuItemImage*>(sender);

	int tag = item->getTag();

	switch (tag)
	{
	case MENU_ITEM_RUN_WORLD_SCENE:
		scene = new GameScene();
		director->pushScene(scene);
		scene->autorelease();
		break;
	case MENU_ITEM_RUN_REGION_SCENE:
		break;
	default: break;
	}
}