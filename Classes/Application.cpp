#include "Application.h"

#include "GameScene.h"
#include "MainMenuScene.h"
#include "SplashScreenScene.h"
#include "Vector2.h"
#include "FileUtils.h"
#include "Log.h"
#include "WorldLoader.h"

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

	std::string basePath = Utils::GetResourcesPath();

	cocos2d::FileUtils::getInstance()->addSearchPath(basePath + "textures");
	cocos2d::FileUtils::getInstance()->addSearchPath(basePath + "worldinfo");
	cocos2d::FileUtils::getInstance()->addSearchPath(basePath + "saves");
	cocos2d::FileUtils::getInstance()->addSearchPath(basePath + "scripts");
	cocos2d::FileUtils::getInstance()->addSearchPath(basePath + "fonts");
	
	director->setOpenGLView(glview);
	director->setAnimationInterval(1.0 / 60.0);

#if CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID && CC_TARGET_PLATFORM != CC_PLATFORM_IOS
	glview->setFrameZoomFactor(1.0f);
	glview->setFrameSize(dr_w, dr_h);
#endif

	glview->setDesignResolutionSize(dr_w, dr_h, ResolutionPolicy::FIXED_HEIGHT);

	MainMenuScene* mainMenuScene = new MainMenuScene(nullptr); // нет автоматического init()
	SplashScreenScene* splashScreenScene = new SplashScreenScene();

	// делаем основной сценой - меню
	director->runWithScene(mainMenuScene);
	// ставим поверх всего SplashScreen
	director->pushScene(splashScreenScene);
	// готовимся выгрузить SplashScreen
	splashScreenScene->autorelease();

	// загружаем игровые данные
	WorldLoader::LoadGameInfo();
	WorldLoader::LoadGameState();

	// инициализируем графику уже после того как всё загрузилось
	mainMenuScene->init();

	// регистрируем сцены в сборщике мусора
	mainMenuScene->autorelease();

	director->setDisplayStats(true);

	return true;
}

void AppDelegate::applicationDidEnterBackground()
{
}

void AppDelegate::applicationWillEnterForeground()
{
}
