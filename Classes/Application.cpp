#include "Application.h"

#include "GameScene.h"
#include "MainMenuScene.h"
#include "SplashScreenScene.h"
#include "Vector2.h"
#include "MiscUtils.h"
#include "Log.h"
#include "WorldLoader.h"
#include "GameSavesManager.h"
#include "Localization.h"
#include "ThreadUtils.h"

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
        glview = cocos2d::GLView::create("The We");
    }

	// Take Samsung Galaxy S2 resolution as reference
	float dr_w = 800;
	float dr_h = 480;

	std::string resourcesPath = Utils::GetResourcesPath();

	cocos2d::FileUtils::getInstance()->addSearchPath(resourcesPath + "textures");
	cocos2d::FileUtils::getInstance()->addSearchPath(resourcesPath + "gui");
	cocos2d::FileUtils::getInstance()->addSearchPath(resourcesPath + "worldinfo");
	cocos2d::FileUtils::getInstance()->addSearchPath(resourcesPath + "saves");
	cocos2d::FileUtils::getInstance()->addSearchPath(resourcesPath + "scripts");
	cocos2d::FileUtils::getInstance()->addSearchPath(resourcesPath + "shaders");
	cocos2d::FileUtils::getInstance()->addSearchPath(resourcesPath + "fonts");
	cocos2d::FileUtils::getInstance()->addSearchPath(resourcesPath + "texts");
	
	director->setOpenGLView(glview);
	director->setAnimationInterval(1.0 / 60.0);

#if CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID && CC_TARGET_PLATFORM != CC_PLATFORM_IOS && CC_TARGET_PLATFORM != CC_PLATFORM_WP8
	glview->setFrameZoomFactor(1.0f);
	glview->setFrameSize(dr_w, dr_h);

	director->setDisplayStats(true);
#endif

	glview->setDesignResolutionSize(dr_w, dr_h, ResolutionPolicy::FIXED_HEIGHT);

	srand(time(nullptr));

	MainMenuScene* mainMenuScene = new MainMenuScene(); // there is no automatic init()
	SplashScreenScene* splashScreenScene = new SplashScreenScene();
	// make Menu as the main scene
	director->runWithScene(mainMenuScene);
	// put SplashScreen onto the stack
	director->pushScene(splashScreenScene);
	// ready to unload the SplashScreen
	splashScreenScene->autorelease();

	CreateAllShaders();

	std::string systemLanguageCode = getCurrentLanguageCode();

	auto dataLoading([systemLanguageCode](){ // lambda-method for background loading
		GameInfo::Instance().ParseXml("gameInfo.xml"); // load game info

		if (Utils::IsPlatformDesktop()){ // load localizations
			std::string languageCode = GameInfo::Instance().GetString("DESKTOP_LOCALE");
			LocalizationManager::Instance().InitWithLocale("content.xml", languageCode);
		}
		else{
			// use system language
			LocalizationManager::Instance().InitWithLocale("content.xml", systemLanguageCode);
		}
	});

	auto onFinishDataLoading([mainMenuScene, splashScreenScene](){
		// load game data
		World::Instance().InitLuaContext();
		WorldLoader::LoadGameInfo();
		GameSavesManager::Instance().LoadGameState();

		// initialize graphics after all data is loaded
		mainMenuScene->init();

		// register scenes in the garbage collector
		mainMenuScene->autorelease();

		// start calculation of game logic
		World::Instance().StartLogic();

		splashScreenScene->SetLoadingFinished();
	});

	Utils::RunInBackgroundThread(dataLoading, onFinishDataLoading);

	return true;
}

void AppDelegate::applicationDidEnterBackground()
{
	GameSavesManager::Instance().SaveGameTime();
}

void AppDelegate::applicationWillEnterForeground()
{
}
