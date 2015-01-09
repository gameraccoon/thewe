#include "TutorialWidgetTextual.h"

#include <luabind/luabind.hpp>

#include "Vector2.h"
#include "Color.h"
#include "World.h"
#include "Log.h"

TutorialWidgetTextual::TutorialWidgetTextual(Tutorial::WeakPtr tutorial)
	: TutorialWidget(tutorial)
	, _state(State::SHOW)
	, _time(0.0f)
{
}

TutorialWidgetTextual* TutorialWidgetTextual::create(Tutorial::WeakPtr tutorial)
{
	TutorialWidgetTextual* ret = new TutorialWidgetTextual(tutorial);
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

bool TutorialWidgetTextual::init()
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
	listener->onTouchesBegan = CC_CALLBACK_2(TutorialWidgetTextual::TouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(TutorialWidgetTextual::TouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(TutorialWidgetTextual::TouchesEnded, this);
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
	//LocalizationManager::Instance().getText("TapToContinue")
	addChild(_fsQuad, 0);
	addChild(_tutorialText, 1);
	addChild(_tapToContinue, 1);
	scheduleUpdate();

	return true;
}

void TutorialWidgetTextual::update(float dt)
{
	float alpha = 0.5f;

	if (_state == State::SHOW) {
		float scale = 1.0f / 0.6f;
		_time += dt * scale;
		alpha = Utils::Lerp(0.0f, 0.5f, _time);
		_tapToContinue->setOpacity(Utils::Clamp(255, 0, (int)(255 * _time)));
		if (_time > 1.0f) {
			_time = 0.0f;
			_state = State::STAND;
		}
	}
	if (_state == State::HIDE) {
		float scale = 1.0f / 0.6f;
		_time += dt * scale;
		alpha = Utils::Lerp(0.5f, 0.0f, _time);
		_tapToContinue->setOpacity((Utils::Clamp(255, 0, (int)(255 - 255*_time))));
		if (_time > 1.0f) {
			_time = 0.0f;
			_state = State::FINISH;
			World::Instance().GetTutorialManager().RemoveCurrentTutorial();
		}
	}

	_fsQuad->clear();
	_fsQuad->drawPolygon(_quad, 4,
						cocos2d::Color4F(0.0f, 0.0f, 0.0f, alpha),
						0.0f, cocos2d::Color4F::WHITE);
}

void TutorialWidgetTextual::TouchesBegan(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
	if (_state == State::STAND) {
		_state = State::HIDE;
		cocos2d::Size view = cocos2d::Director::getInstance()->getVisibleSize();
		_tutorialText->runAction(cocos2d::MoveBy::create(0.5f, cocos2d::Vec2(view.width, 0.0f)));
	}

	event->stopPropagation();
}

void TutorialWidgetTextual::TouchesMoved(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
	event->stopPropagation();
}

void TutorialWidgetTextual::TouchesEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
	event->stopPropagation();
}
