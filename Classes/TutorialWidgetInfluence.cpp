#include "TutorialWidgetInfluence.h"

#include <luabind/luabind.hpp>

#include "Vector2.h"
#include "Color.h"
#include "World.h"
#include "Log.h"

TutorialWidgetInfluence::TutorialWidgetInfluence(Tutorial::WeakPtr tutorial)
	: TutorialWidget(tutorial)
	, _state(State::SHOW)
	, _time(0.0f)
{
}

TutorialWidgetInfluence* TutorialWidgetInfluence::create(Tutorial::WeakPtr tutorial)
{
	TutorialWidgetInfluence* ret = new TutorialWidgetInfluence(tutorial);
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

bool TutorialWidgetInfluence::init()
{
	if (!cocos2d::Node::init())
	{
		return false;
	}

	Tutorial::Ptr tutorial = _tutorial.lock();
	if (!tutorial)
	{
		WRITE_WARN("Tutorial was removed before use");
		return true;
	}

	cocos2d::EventListenerTouchAllAtOnce *listener = cocos2d::EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(TutorialWidgetInfluence::TouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(TutorialWidgetInfluence::TouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(TutorialWidgetInfluence::TouchesEnded, this);
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	cocos2d::Size view = cocos2d::Director::getInstance()->getVisibleSize();
	cocos2d::Vec2 window = cocos2d::Director::getInstance()->getWinSize();

	_spot = cocos2d::Sprite::create("tutorial_influence.png");
	_spot->setScale(0.85f, 0.2f);
	_spot->setPosition(cocos2d::Vec2(view.width/2.0f, 14.0f));
	_spot->retain();

	_blackout = ScreenBlackoutWidget::create(cocos2d::Color4F(0.0f, 0.0f, 0.0f, 0.0f));
	_blackout->AddSpot(_spot);

	_arrow = cocos2d::Sprite::create("tutorial_arrow.png");
	_arrow->setPosition(cocos2d::Vec2(200.0f, 75.0f) - view/2.0f);
	_arrow->setScale(0.35f);
	_arrow->setRotation(-90.0f);
	_arrow->runAction(TutorialWidget::Arrow(cocos2d::Vec2(0,-1), 40.0f, 0.9f));

	_tutorialText = cocos2d::ui::Text::create(_tutorial.lock()->text, "EuropeNormal.ttf", 28);
	_tutorialText->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
	_tutorialText->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
	_tutorialText->setTextAreaSize(cocos2d::Size(300.0f, 500.0f));
	_tutorialText->setPosition(cocos2d::Vec2(view.width, 0.0f));
	_tutorialText->runAction(cocos2d::MoveTo::create(0.5f, cocos2d::Vec2(220.0f, 0.0f)));

	cocos2d::ScaleTo *ttc_scale1 = cocos2d::ScaleTo::create(0.5f, 0.95f, 0.95f, 1.0f);
	cocos2d::ScaleTo *ttc_scale2 = cocos2d::ScaleTo::create(0.6f, 1.05f, 1.05f, 1.0f);
	cocos2d::Sequence *ttc_scale = cocos2d::Sequence::create(ttc_scale1, ttc_scale2, nullptr);
	cocos2d::RepeatForever *pulsation = cocos2d::RepeatForever::create(ttc_scale);
	_tapToContinue = cocos2d::ui::Text::create(_tutorial.lock()->buttonText, "EuropeNormal.ttf", 18);
	_tapToContinue->setPosition(cocos2d::Vec2(0.0f, -170.0f));
	_tapToContinue->runAction(pulsation);
	
	addChild(_blackout, 0);
	addChild(_arrow, 1);
	addChild(_tutorialText, 2);
	addChild(_tapToContinue, 2);
	scheduleUpdate();

	return true;
}

void TutorialWidgetInfluence::update(float dt)
{
	float alpha = 0.5f;

	if (_state == State::SHOW) {
		float scale = 1.0f / 0.6f;
		_time += dt * scale;
		alpha = Math::Lerp(0.0f, 0.5f, _time);
		_tapToContinue->setOpacity(Math::Clamp(255, 0, (int)(255 * _time)));
		if (_time > 1.0f) {
			_time = 0.0f;
			_state = State::STAND;
		}
	}
	if (_state == State::HIDE) {
		float scale = 1.0f / 0.6f;
		_time += dt * scale;
		alpha = Math::Lerp(0.5f, 0.0f, _time);
		_tapToContinue->setOpacity((Math::Clamp(255, 0, (int)(255 - 255*_time))));
		if (_time > 1.0f) {
			_time = 0.0f;
			_state = State::FINISH;
			World::Instance().GetTutorialManager().RemoveCurrentTutorial();
		}
	}

	_blackout->SetBackgroundColor(cocos2d::Color4F(0.0f, 0.0f, 0.0f, alpha));
}

void TutorialWidgetInfluence::TouchesBegan(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
	if (_state == State::STAND) {
		_state = State::HIDE;
		cocos2d::Size view = cocos2d::Director::getInstance()->getVisibleSize();
		_tutorialText->runAction(cocos2d::MoveBy::create(0.5f, cocos2d::Vec2(view.width, 0.0f)));
	}

	event->stopPropagation();
}

void TutorialWidgetInfluence::TouchesMoved(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
	event->stopPropagation();
}

void TutorialWidgetInfluence::TouchesEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
	event->stopPropagation();
}
