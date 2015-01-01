#include "TutorialWidgetWelcome.h"

#include <luabind/luabind.hpp>

#include "Vector2.h"
#include "Color.h"
#include "World.h"
#include "Log.h"

TutorialWidgetWelcome::TutorialWidgetWelcome(Tutorial::WeakPtr tutorial)
	: TutorialWidget(tutorial)
	, _time(0.0f)
	, _alpha(1.0f)
	, _state(State::FADE_IN)
{
}

TutorialWidgetWelcome* TutorialWidgetWelcome::create(Tutorial::WeakPtr tutorial)
{
	TutorialWidgetWelcome* ret = new TutorialWidgetWelcome(tutorial);
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

bool TutorialWidgetWelcome::init()
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
	listener->onTouchesBegan = CC_CALLBACK_2(TutorialWidgetWelcome::TouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(TutorialWidgetWelcome::TouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(TutorialWidgetWelcome::TouchesEnded, this);
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	
	cocos2d::Size view = cocos2d::Director::getInstance()->getVisibleSize();
	cocos2d::Vec2 window = cocos2d::Director::getInstance()->getWinSize();

	_quad[0] = cocos2d::Vec2(0.0f, 0.0f);
	_quad[1] = cocos2d::Vec2(window.x, 0.0f);
	_quad[2] = cocos2d::Vec2(window.x, window.y);
	_quad[3] = cocos2d::Vec2(0.0f, window.y);

	_fsQuad = cocos2d::DrawNode::create();
	_fsQuad->drawPolygon(_quad, 4, cocos2d::Color4F(0.0f, 0.0f, 0.0f, 0.5f), 0.0f, cocos2d::Color4F::WHITE);
	_fsQuad->setPosition(-window * 0.5f);
	
	cocos2d::MoveTo *ttc_move = cocos2d::MoveTo::create(0.8f, cocos2d::Vec2(0.0f, -170.0f));
	cocos2d::DelayTime *ttc_delay = cocos2d::DelayTime::create(2.2f);

	_tapToContinue = cocos2d::ui::Text::create(LocalizationManager::Instance().getText("TapToContinue"), "EuropeNormal.ttf", 18);
	_tapToContinue->setPosition(cocos2d::Vec2(0.0f, -view.height));

	cocos2d::CallFunc *func = cocos2d::CallFunc::create([&]() {
		cocos2d::ScaleTo *ttc_scale1 = cocos2d::ScaleTo::create(0.5f, 0.95f, 0.95f, 1.0f);
		cocos2d::ScaleTo *ttc_scale2 = cocos2d::ScaleTo::create(0.6f, 1.05f, 1.05f, 1.0f);
		cocos2d::Sequence *ttc_scale = cocos2d::Sequence::create(ttc_scale1, ttc_scale2, nullptr);
		cocos2d::RepeatForever *pulsation = cocos2d::RepeatForever::create(ttc_scale);
		_tapToContinue->runAction(pulsation);
	});

	_tapToContinue->runAction(cocos2d::Sequence::create(ttc_delay, ttc_move, func, nullptr));

	cocos2d::MoveTo *welcome_move = cocos2d::MoveTo::create(1.0f, cocos2d::Vec2(0.0f, 0.0f));
	cocos2d::DelayTime *welcome_delay = cocos2d::DelayTime::create(1.0f);

	cocos2d::ui::Text *welcomeText = cocos2d::ui::Text::create(_tutorial.lock()->text, "EuropeNormal.ttf", 30);
	welcomeText->setPosition(cocos2d::Vec2(0.0f, view.width));
	welcomeText->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
	welcomeText->runAction(cocos2d::Sequence::create(welcome_delay, welcome_move, nullptr));

	addChild(_fsQuad, 0);
	addChild(_tapToContinue, 1);
	addChild(welcomeText, 1);
	scheduleUpdate();

	return true;
}

void TutorialWidgetWelcome::update(float dt)
{
	if (_state == State::FADE_IN) {
		float scale = 1.0f / 3.0f;
		if (_time < 1.0f) {
			_time += dt * scale;
			_alpha = 1.0f - 0.5f * _time;
			if (_time >= 1.0f) {
				_time = 0.0f;
				_state = State::WAIT;
			}
		}
	}
	if (_state == State::FADE_OUT) {
		float scale = 1.0f / 0.5f;
		if (_time < 1.0f) {
			_time += dt * scale;
			_alpha = 0.5f * abs(1.0f - _time);
			if (_time >= 1.0f) {
				_time = 0.0f;
				_state = State::FINISH;
				TutorialWidgetWelcome::OnContinueCallback(nullptr, cocos2d::ui::Widget::TouchEventType::ENDED);
			}
		}
	}

	_fsQuad->clear();
	_fsQuad->drawPolygon(_quad, 4,
						cocos2d::Color4F(0.0f, 0.0f, 0.0f, _alpha),
						0.0f, cocos2d::Color4F::WHITE);
}

void TutorialWidgetWelcome::TouchesBegan(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
	event->stopPropagation();
}

void TutorialWidgetWelcome::TouchesMoved(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
	event->stopPropagation();
}

void TutorialWidgetWelcome::TouchesEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
	if (_state == State::WAIT) {
		_state = State::FADE_OUT;
	}

	event->stopPropagation();
}

bool TutorialWidgetWelcome::IsReadyToClose(void) const
{
	return _state == State::FINISH;
}