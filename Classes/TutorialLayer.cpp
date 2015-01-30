#include "TutorialLayer.h"

#include <math.h>

#include "World.h"
#include "WorldMapLayer.h"
#include "GameScene.h"

TutorialLayer::TutorialLayer(WorldMapLayer *worldMapLayer, MapProjector *projector)
	: _currentTutorial(nullptr)
	, _worldMapLayer(worldMapLayer)
	, _projector(projector)
{
}

TutorialLayer* TutorialLayer::create(WorldMapLayer *worldMapLayer, MapProjector *projector)
{
	TutorialLayer* ret = new TutorialLayer(worldMapLayer, projector);
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
		if (World::Instance().GetTutorialManager().IsHaveTutorial())
		{
			cocos2d::Director *director = cocos2d::Director::getInstance();
			Vector2 screen = director->getVisibleSize();
			Vector2 origin = director->getVisibleOrigin();

			_currentTutorial = TutorialWidget::Make(World::Instance().GetTutorialManager().GetCurrentTutorial(),
				                                    _worldMapLayer,
													_projector);
			addChild(_currentTutorial);
			_currentTutorial->setPosition(origin + screen/2);
		}
	}
	else
	{
		if (_currentTutorial->IsReadyToClose())
		{
			removeChild(_currentTutorial);
			_currentTutorial = nullptr;
		}
	}
}
