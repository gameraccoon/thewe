#include "SplashScreenScene.h"

#include "Vector2.h"
#include "Log.h"

SplashScreenScene::SplashScreenScene(void)
	: _timeElapsed(0.0f)
	, _timeToShow(1.5f)
	, _isLoadingComplete(false)
{
}

SplashScreenScene::~SplashScreenScene(void)
{
	WRITE_LOG("SplashScreen unloaded sucessfully");
}

SplashScreenScene* SplashScreenScene::create()
{
	SplashScreenScene* ret = new SplashScreenScene();
	if (ret && ret->init())
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

bool SplashScreenScene::init(void)
{
	if (!cocos2d::Scene::init())
	{
		return false;
	}

	cocos2d::Director *director = cocos2d::Director::getInstance();

	_splashLogo = cocos2d::Sprite::create("ui/TheWe-Splash.png");

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

	if (_isLoadingComplete && _timeElapsed > _timeToShow)
	{
		cocos2d::Director::getInstance()->popScene();
	}
}

void SplashScreenScene::SetLoadingFinished()
{
	_isLoadingComplete = true;
}
