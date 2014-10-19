#include "TutorialLayer.h"

#include <math.h>

#include "World.h"
#include "GameScene.h"

TutorialLayer::TutorialLayer()
	: _currentTutorial(nullptr)
{
}

TutorialLayer* TutorialLayer::create()
{
	TutorialLayer* ret = new TutorialLayer();
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

bool TutorialLayer::init(void)
{
	if (!cocos2d::Layer::init())
	{
		return false;
	}

	scheduleUpdate();

	return true;
}

void TutorialLayer::update(float delta)
{
	if (_currentTutorial == nullptr)
	{
		if (World::Instance().IsHaveTutorial())
		{
			cocos2d::Director *director = cocos2d::Director::getInstance();
			Vector2 screen = director->getVisibleSize();
			Vector2 origin = director->getVisibleOrigin();

			_currentTutorial = TutorialWidget::create(World::Instance().GetCurrentTutorial());
			addChild(_currentTutorial);
			_currentTutorial->setPosition(origin + screen/2);

			dynamic_cast<GameScene*>(getParent())->SetInputEnabled(false);
		}
	}
	else
	{
		if (_currentTutorial->IsReadyToClose())
		{
			removeChild(_currentTutorial);
			_currentTutorial = nullptr;

			dynamic_cast<GameScene*>(getParent())->SetInputEnabled(true);
		}
	}
}
