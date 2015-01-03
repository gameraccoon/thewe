#include "TutorialWidgetFirstCell.h"

#include <luabind/luabind.hpp>

#include "MapProjector.h"
#include "WorldMapLayer.h"
#include "Vector2.h"
#include "Color.h"
#include "World.h"
#include "Log.h"

TutorialWidgetFirstCell::TutorialWidgetFirstCell(Tutorial::WeakPtr tutorial, WorldMapLayer *worldMapLayer, MapProjector *projector)
	: TutorialWidget(tutorial)
	, _worldMapLayer(worldMapLayer)
	, _projector(projector)
	, _state(State::FOCUS)
	, _time(0.0f)
{
}

TutorialWidgetFirstCell* TutorialWidgetFirstCell::create(Tutorial::WeakPtr tutorial, WorldMapLayer *worldMapLayer, MapProjector *projector)
{
	TutorialWidgetFirstCell* ret = new TutorialWidgetFirstCell(tutorial, worldMapLayer, projector);
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

bool TutorialWidgetFirstCell::init()
{
	if (!cocos2d::Node::init())
	{
		return false;
	}

	Tutorial::Ptr tutorial = _tutorial.lock();
	if (!tutorial)
	{
		Log::Instance().writeWarning("Tutorial was removed before use");
		return true;
	}

	cocos2d::EventListenerTouchAllAtOnce *listener = cocos2d::EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(TutorialWidgetFirstCell::TouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(TutorialWidgetFirstCell::TouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(TutorialWidgetFirstCell::TouchesEnded, this);
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	cocos2d::Size view = cocos2d::Director::getInstance()->getVisibleSize();

	_tutorialText = cocos2d::ui::Text::create(LocalizationManager::Instance().getText(_tutorial.lock()->text), "EuropeNormal.ttf", 32);
	_tutorialText->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
	_tutorialText->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
	_tutorialText->setTextAreaSize(cocos2d::Size(300.0f, 500.0f));
	_tutorialText->setPosition(cocos2d::Vec2(view.width, -100.0f));

	_startViewScale = _projector->GetScale();
	_startViewPos = _projector->GetLocation();

	addChild(_tutorialText);
	scheduleUpdate();

	return true;
}

void TutorialWidgetFirstCell::update(float dt)
{
	if (_state == State::FOCUS) {
		float scale = 1.0f / 1.5f;
		_time += dt * scale;

		_projector->SetScale(Utils::Lerp(_startViewScale, 1.2f, _time));
		_projector->SetLocation(Utils::Lerp(_startViewPos, Vector2(300.0f, 280.0f), _time));
		_worldMapLayer->UpdateMapElements();

		if (_time > 1.0f) {
			_time = 0.0f;
			_state = State::WAIT;

			MessageManager::Instance().PutMessage(Message("DisableMapScrolling"));

			cocos2d::Size view = cocos2d::Director::getInstance()->getVisibleSize();
			cocos2d::MoveTo *move = cocos2d::MoveTo::create(0.5f, cocos2d::Vec2(200.0f, -100.0f));
			
			_tutorialText->runAction(cocos2d::Sequence::create(move, nullptr));
		}
	}

	if (_tutorial.expired() && _state == State::WAIT) {
		// new cell already created.
		_state = State::DISAPPEAR;
		
		cocos2d::Size view = cocos2d::Director::getInstance()->getVisibleSize();
		cocos2d::MoveTo *move = cocos2d::MoveTo::create(0.3f, cocos2d::Vec2(-view.width, -100.0f));
		cocos2d::CallFunc *func = cocos2d::CallFunc::create([&](){
			_state = State::FINISH;
			MessageManager::Instance().PutMessage(Message("EnableMapScrolling"));
		});
		
		_tutorialText->runAction(cocos2d::Sequence::create(move, func, nullptr));
	}
}

void TutorialWidgetFirstCell::TouchesBegan(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
	if (_state != State::WAIT) {
		event->stopPropagation();
	}
}

void TutorialWidgetFirstCell::TouchesMoved(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
	if (_state != State::WAIT) {
		event->stopPropagation();
	}
}

void TutorialWidgetFirstCell::TouchesEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
	if (_state != State::WAIT) {
		event->stopPropagation();
	}
}

bool TutorialWidgetFirstCell::IsReadyToClose(void) const
{
	return _tutorial.expired() && _state == State::FINISH;
}