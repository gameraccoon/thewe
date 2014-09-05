#include "Application.h"

#include "GameScene.h"
#include "Vector2.h"
#include "PlayersProfiles.h"

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
	cocos2d::Director *director = cocos2d::Director::getInstance();
	cocos2d::GLView *glview = director->getOpenGLView();
	
	if(!glview) 
	{
        glview = cocos2d::GLView::create("Samsung Galaxy S II");
    }

	// Take Samsung Galaxy S2 resolution as reference
	float dr_w = 800;
	float dr_h = 480;

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	glview->setFrameZoomFactor(1.0f);
	glview->setFrameSize(dr_w, dr_h);
	glview->setFrameZoomFactor(1.0f);
	cocos2d::FileUtils::getInstance()->addSearchPath("../../Resources/textures");
	cocos2d::FileUtils::getInstance()->addSearchPath("../../Resources/worldinfo");
	cocos2d::FileUtils::getInstance()->addSearchPath("../../Resources/saves");
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	cocos2d::FileUtils::getInstance()->addSearchPath("textures");
	cocos2d::FileUtils::getInstance()->addSearchPath("worldinfo");
	cocos2d::FileUtils::getInstance()->addSearchPath("saves");
#endif


	director->setOpenGLView(glview);
	director->setDisplayStats(true);
	director->setAnimationInterval(1.0 / 60.0);

	cocos2d::Size sr = glview->getFrameSize();

	//director->setContentScaleFactor(sr.width / dr_w);
	glview->setDesignResolutionSize(dr_w, dr_h, ResolutionPolicy::FIXED_HEIGHT);

	ProfilesManager::Instance().LoadProfiles();

	_menuScene = cocos2d::CCScene::create();
	
	{
		using namespace cocos2d;
		_btnRunGame = MenuItemImage::create("btn-start-game-normal.png", "btn-start-game-selected.png",
			CC_CALLBACK_1(AppDelegate::_MenuInputListener, this));
		_btnTestScene1 = MenuItemImage::create("btn-test1-normal.png", "btn-test1-selected.png",
			CC_CALLBACK_1(AppDelegate::_MenuInputListener, this));
		_btnExitGame = MenuItemImage::create("btn-exit-normal.png", "btn-exit-selected.png",
			CC_CALLBACK_1(AppDelegate::_MenuInputListener, this));
	}

	Vector2 client = director->getVisibleSize();
	Vector2 origin = director->getVisibleOrigin();
	Vector2 center(origin.x + client.x / 2.0f, origin.y + client.y - 100.0f);

	_btnRunGame->setPosition(center - Vector2(0.0f, 0.0f));
	_btnRunGame->setTag(MENU_ITEM_RUN_GAME);
	_btnRunGame->setScale(3.0f);
	_btnTestScene1->setPosition(center - Vector2(0.0f, 135.0f));
	_btnTestScene1->setTag(MENU_ITEM_TEST_SCENE_1);
	_btnTestScene1->setScale(3.0f);
	_btnExitGame->setPosition(center - Vector2(0.0f, 270.0f));
	_btnExitGame->setTag(MENU_ITEM_EXIT);
	_btnExitGame->setScale(3.0f);

	_mainMenu = cocos2d::Menu::create(_btnRunGame, _btnTestScene1, _btnExitGame, NULL);
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

void AppDelegate::_MenuInputListener(cocos2d::Ref *sender)
{
	cocos2d::Director *director = cocos2d::Director::getInstance();
	cocos2d::Scene *scene = NULL;

	cocos2d::MenuItemImage *item = dynamic_cast<cocos2d::MenuItemImage*>(sender);

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
			cocos2d::Director *director = cocos2d::Director::getInstance();
			director->end();
		}
		break;
	default: break;
	}
}
