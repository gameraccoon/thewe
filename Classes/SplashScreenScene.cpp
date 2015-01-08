#include "SplashScreenScene.h"

#include "Vector2.h"
#include "Log.h"

SplashScreenScene::SplashScreenScene(void)
	: _timeElapsed(0.0f)
	, _timeToShow(2.0f)
{
	init();
}

SplashScreenScene::~SplashScreenScene(void)
{
	WRITE_LOG("SplashScreen unloaded sucessfully");
}

bool SplashScreenScene::init(void)
{
	if (!cocos2d::CCScene::init())
	{
		return false;
	}

	cocos2d::Director *director = cocos2d::Director::getInstance();

	_splashLogo = cocos2d::Sprite::create("TheWe-Splash.png");

	Vector2 client = director->getVisibleSize();
	Vector2 origin = director->getVisibleOrigin();
	Vector2 center(origin.x + client.x / 2.0f, origin.y + client.y / 2.0f);

	_splashLogo->setPosition(center);

	addChild(_splashLogo);

	scheduleUpdate();

	return true;
}

void SplashScreenScene::update(float delta)
{
	_timeElapsed += delta;

	if (_timeElapsed > _timeToShow)
	{
		cocos2d::Director::getInstance()->popScene();
	}
}
