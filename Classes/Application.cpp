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

	MainMenuScene*		mainMenuScene = new MainMenuScene(nullptr); // there is no automatic init()
	SplashScreenScene*	splashScreenScene = new SplashScreenScene();

	director->runWithScene(mainMenuScene); // make Menu as the main scene	
	director->pushScene(splashScreenScene); // put SplashScreen onto the stack	

	splashScreenScene->autorelease(); // ready to unload the SplashScreen	

	mainMenuScene->init(); // initialize graphics after all data is loaded	
	mainMenuScene->autorelease(); // register scenes in the garbage collector	

	auto gameLoading = [&, splashScreenScene](){ // lambda-method for background loading
		
		GameInfo::Instance().ParseXml("gameInfo.xml"); // load game info		
		if (Utils::IsPlatformDesktop()){ // load localizations

			std::string languageCode = GameInfo::Instance().GetString("DESKTOP_LOCALE");
			LocalizationManager::Instance().InitWithLocale("content.xml", languageCode);
		}
		else{

			// use system language
			LocalizationManager::Instance().InitWithLocale("content.xml", getCurrentLanguageCode());
		}

		// load game data
		World::Instance().InitLuaContext();
		WorldLoader::LoadGameInfo();
		GameSavesManager::Instance().LoadGameState();	

		World::Instance().StartLogic(); // start calculation of game logic	

		std::this_thread::sleep_for(std::chrono::seconds(2)); // the illusion of long loading for very fast progress
		splashScreenScene->b_gameLoaded = true;
	};
	std::thread(gameLoading).detach();	

	return true;
}

void AppDelegate::applicationDidEnterBackground()
{
	GameSavesManager::Instance().SaveGameTime();
}

void AppDelegate::applicationWillEnterForeground()
{
}
