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

	director->setDisplayStats(true);
#endif

	glview->setDesignResolutionSize(dr_w, dr_h, ResolutionPolicy::FIXED_HEIGHT);

	MainMenuScene* mainMenuScene = new MainMenuScene(nullptr); // нет автоматического init()
	SplashScreenScene* splashScreenScene = new SplashScreenScene();

	// make Menu as the main scene
	director->runWithScene(mainMenuScene);
	// put SplashScreen onto the stack
	director->pushScene(splashScreenScene);
	// ready to unload the SplashScreen
	splashScreenScene->autorelease();

	// load game data
	WorldLoader::LoadGameInfo();
	WorldLoader::LoadGameState();

	// initialize graphics after all data is loaded
	mainMenuScene->init();

	// register scenes in the garbage collector
	mainMenuScene->autorelease();

	return true;
}

void AppDelegate::applicationDidEnterBackground()
{
}

void AppDelegate::applicationWillEnterForeground()
{
}
