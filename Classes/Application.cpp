#include "Application.h"

#include "GameScene.h"
#include "MainMenuScene.h"
#include "Vector2.h"
#include "PlayersProfiles.h"
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

	director->setOpenGLView(glview);
	director->setDisplayStats(true);
	director->setAnimationInterval(1.0 / 60.0);

#if CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID && CC_TARGET_PLATFORM != CC_PLATFORM_IOS
	glview->setFrameZoomFactor(1.0f);
	glview->setFrameSize(dr_w, dr_h);
#endif

	glview->setDesignResolutionSize(dr_w, dr_h, ResolutionPolicy::FIXED_HEIGHT);

	GameScene* gameScene = new GameScene();
	MainMenuScene* mainMenuScene = new MainMenuScene();

	// делаем основной - игровую сцену
	director->runWithScene(gameScene);
	// ставим поверх главное меню
	director->pushScene(mainMenuScene);
	// ToDo: поверх нужно будет добавлять SplashScreen

	// загружаем игровые данные
	ProfilesManager::Instance().LoadProfiles();
	WorldLoader::LoadGameInfo();
	WorldLoader::LoadGameState();

	// инициализируем графику
	mainMenuScene->init();
	gameScene->init();

	// регистрируем сцены в сборщике мусора
	mainMenuScene->autorelease();
	gameScene->autorelease();

	return true;
}

void AppDelegate::applicationDidEnterBackground()
{
}

void AppDelegate::applicationWillEnterForeground()
{
}
