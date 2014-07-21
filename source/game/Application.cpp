#include "Application.h"

#include "TestsScene.h"

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

	cocos2d::CCSize framesize = glview->getFrameSize();

	director->setDisplayStats(true);
	director->setAnimationInterval(1.0 / 60.0);

	cocos2d::CCScene *scene = HelloWorldLayer::scene();

	director->runWithScene(scene);

	return true;
}

void AppDelegate::applicationDidEnterBackground()
{
}

void AppDelegate::applicationWillEnterForeground()
{
}