#include "TutorialWidgetStartFirstTask.h"

#include <luabind/luabind.hpp>

#include "Vector2.h"
#include "Color.h"
#include "World.h"
#include "Log.h"

TutorialWidgetStartFirstTask::TutorialWidgetStartFirstTask(Tutorial::WeakPtr tutorial)
	: TutorialWidget(tutorial)
	, _state(State::APPEAR)
	, _time(0.0f)
{
}

TutorialWidgetStartFirstTask* TutorialWidgetStartFirstTask::create(Tutorial::WeakPtr tutorial)
{
	TutorialWidgetStartFirstTask* ret = new TutorialWidgetStartFirstTask(tutorial);
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

bool TutorialWidgetStartFirstTask::init()
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
	listener->onTouchesBegan = CC_CALLBACK_2(TutorialWidgetStartFirstTask::TouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(TutorialWidgetStartFirstTask::TouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(TutorialWidgetStartFirstTask::TouchesEnded, this);
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	cocos2d::Size view = cocos2d::Director::getInstance()->getVisibleSize();

	_spotStartPos = view / 2.0f;
	_spot = cocos2d::Sprite::create("tutorial_menu.png");
	_spot->setPosition(_spotStartPos);
	_spot->retain();

	_blackout = ScreenBlackoutWidget::create(cocos2d::Color4F(0.0f, 0.0f, 0.0f, 0.0f));
	_blackout->AddSpot(_spot);

	_arrow = cocos2d::Sprite::create("tutorial_arrow.png");
	_arrow->setRotation(-90.0f);
	_arrow->setPosition((view/2.0f + cocos2d::Vec2(-160.0f, 130.0f)) - view/2.0f);
	_arrow->setScale(0.3f);
	_arrow->setVisible(false);

	_text = cocos2d::ui::Text::create(LocalizationManager::Instance().getText(_tutorial.lock()->text), "EuropeNormal.ttf", 30);
	_text->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
	_text->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
	_text->setPosition(cocos2d::Vec2(view.width, -100.0f));

	addChild(_blackout, 0);
	addChild(_arrow, 1);
	addChild(_text, 1);
	scheduleUpdate();

	return true;
}

void TutorialWidgetStartFirstTask::update(float dt)
{
	if (_state == State::APPEAR) {
		float scale = 1.0f / 1.0f;
		_time += dt * scale;
		float alpha = Utils::Lerp(0.0f, 0.9f, _time);
		_blackout->SetBackgroundColor(cocos2d::Color4F(0.0f, 0.0f, 0.0f, alpha));
		if (_time > 1.0f) {
			_time = 0.0f;
			_state = State::FOCUS;
			_text->runAction(cocos2d::MoveTo::create(0.5f, cocos2d::Vec2(0.0, -100.0f)));
		}
	} else if (_state == State::FOCUS) {
		float scale = 1.0f / 0.5f;
		_time += dt * scale;
		_spot->setScaleY(Utils::Lerp(1.0f, 0.27f, _time));
		_spot->setPositionY(_spotStartPos.y + Utils::Lerp(0.0f, 73.0f, _time)); 
		if (_time > 1.0f) {
			_time = 0.0f;
			_state = State::WAIT;
			_arrow->setVisible(true);
			_arrow->runAction(TutorialWidget::Arrow(cocos2d::Vec2(0,-1), 40.0f, 0.9f));
		}
	} else if (_state == State::DISAPPEAR) {
		float scale = 1.0f / 0.6f;
		_time += dt * scale;
		float alpha = Utils::Lerp(0.9f, 0.0f, _time);
		_blackout->SetBackgroundColor(cocos2d::Color4F(0.0f, 0.0f, 0.0f, alpha));
		if (_time > 1.0f) {
			_time = 0.0f;
			_state = State::FINISH;
		}
	}

	if ((_state != State::DISAPPEAR && _state != State::FINISH) && _tutorial.expired()) {
		_state = State::DISAPPEAR;
		cocos2d::Size view = cocos2d::Director::getInstance()->getVisibleSize();
		_arrow->runAction(cocos2d::FadeOut::create(0.2f));
		_text->runAction(cocos2d::MoveTo::create(0.3f, cocos2d::Vec2(-view.width, -100.0f)));
	}
}

void TutorialWidgetStartFirstTask::TouchesBegan(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
	event->stopPropagation();
}

void TutorialWidgetStartFirstTask::TouchesMoved(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
	event->stopPropagation();
}

void TutorialWidgetStartFirstTask::TouchesEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
	event->stopPropagation();
}

bool TutorialWidgetStartFirstTask::IsReadyToClose(void) const
{
	return _tutorial.expired() && _state == State::FINISH;
}