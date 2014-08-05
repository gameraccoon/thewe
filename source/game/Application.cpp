#include "Application.h"

#include "GameScene.h"
#include "Point.h"
#include "PlayersProfiles.h"

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

	cocos2d::CCFileUtils::sharedFileUtils()->addSearchPath("../_gamedata/textures");

	ProfilesManager::Instance().LoadProfiles();

	_menuScene = cocos2d::CCScene::create();
	
	_btnRunGame = CCMenuItemImage::create("btn-start-game-normal.png", "btn-start-game-selected.png",
		_menuScene, menu_selector(AppDelegate::_MenuInputListener));
	_btnTestScene1 = CCMenuItemImage::create("btn-test1-normal.png", "btn-test1-selected.png",
		_menuScene, menu_selector(AppDelegate::_MenuInputListener));
	_btnExitGame = CCMenuItemImage::create("btn-exit-normal.png", "btn-exit-selected.png",
		_menuScene, menu_selector(AppDelegate::_MenuInputListener));

	Point client = director->getVisibleSize();
	Point origin = director->getVisibleOrigin();
	Point pos(origin.x + client.x / 2.0f, origin.y + client.y - 100.0f);

	_btnRunGame->setPosition(pos - Point(0.0f, 100.0f));
	_btnRunGame->setTag(MENU_ITEM_RUN_GAME);
	_btnRunGame->setScale(5.0f);
	_btnTestScene1->setPosition(pos - Point(0.0f, 400.0f));
	_btnTestScene1->setTag(MENU_ITEM_TEST_SCENE_1);
	_btnTestScene1->setScale(5.0f);
	_btnExitGame->setPosition(pos - Point(0.0f, 700.0f));
	_btnExitGame->setTag(MENU_ITEM_EXIT);
	_btnExitGame->setScale(5.0f);

	_mainMenu = CCMenu::create(_btnRunGame, _btnTestScene1, _btnExitGame, NULL);
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
	case MENU_ITEM_RUN_GAME:
		scene = new GameScene();
		director->pushScene(scene);
		scene->autorelease();
		break;
	case MENU_ITEM_TEST_SCENE_1:
		break;
	case MENU_ITEM_EXIT:
		{
			cocos2d::CCDirector *director = cocos2d::CCDirector::sharedDirector();
			director->end();
		}
		break;
	default: break;
	}
}
