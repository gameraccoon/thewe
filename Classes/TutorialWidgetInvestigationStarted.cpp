#include "TutorialWidgetInvestigationStarted.h"

#include <luabind/luabind.hpp>

#include "WorldMapLayer.h"
#include "MapProjector.h"
#include "Vector2.h"
#include "Color.h"
#include "World.h"
#include "Log.h"

TutorialWidgetInvestigationStarted::TutorialWidgetInvestigationStarted(Tutorial::WeakPtr tutorial,
																	   WorldMapLayer *worldMapLayer,
																	   MapProjector *projector)
	: TutorialWidget(tutorial)
	, _worldMapLayer(worldMapLayer)
	, _projector(projector)
	, _state(State::MOVE)
	, _time(0.0f)
{
}

TutorialWidgetInvestigationStarted* TutorialWidgetInvestigationStarted::create(Tutorial::WeakPtr tutorial,
																			   WorldMapLayer *worldMapLayer,
																			   MapProjector *projector)
{
	TutorialWidgetInvestigationStarted* ret = new TutorialWidgetInvestigationStarted(tutorial, worldMapLayer, projector);
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

bool TutorialWidgetInvestigationStarted::init()
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

	if (!World::Instance().GetInvestigators().empty()) {
		Investigator::WeakPtr inves = World::Instance().GetInvestigators().at(0);
		_cell = inves.lock()->GetInvestigationRoot();
	} else {
		Log::Instance().writeError("No investigators avaliable");
	}

	_startViewPos = _projector->GetLocation();
	_startViewScale = _projector->GetScale();

	cocos2d::Size view = cocos2d::Director::getInstance()->getVisibleSize();

	cocos2d::EventListenerTouchAllAtOnce *listener = cocos2d::EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(TutorialWidgetInvestigationStarted::TouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(TutorialWidgetInvestigationStarted::TouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(TutorialWidgetInvestigationStarted::TouchesEnded, this);
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	_text = cocos2d::ui::Text::create(_tutorial.lock()->text, "EuropeNormal.ttf", 28);
	_text->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
	_text->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
	_text->setPosition(cocos2d::Vec2(view.width, 0.0f));
	_text->runAction(cocos2d::MoveTo::create(0.5f, cocos2d::Vec2(220.0f, 0.0f)));

	_finger = cocos2d::Sprite::create("tutorial_hand.png");
	_finger->setScale(0.5f);
	_finger->setPosition(0.0f, 0.0f);
	_finger->setAnchorPoint(cocos2d::Vec2(0.25f, 1.0f));
	_finger->setVisible(false);

	addChild(_text);
	addChild(_finger);
	scheduleUpdate();

	MessageManager::Instance().PutMessage(Message("DisableMapScrolling"));

	return true;
}

void TutorialWidgetInvestigationStarted::update(float dt)
{
	if (_state == State::MOVE) {
		float scale = 1.0f / 0.5f;
		_time += dt * scale;
		float t = Math::Clamp(1.0f, 0.0f, _time);
		_projector->SetLocation(Math::Lerp(_startViewPos, _cell.lock()->GetLocation(), t));
		_projector->SetScale(Math::Lerp(_startViewScale, 1.2f, t));
		_worldMapLayer->UpdateMapElements();

		if (_time > 1.0f) {
			_time = 0.0f;
			_state = State::SHOW_FINGER;

			cocos2d::Vec2 view_size = cocos2d::Director::getInstance()->getVisibleSize();
			cocos2d::Vec2 cell_pos = _projector->ProjectOnScreen(_cell.lock()->GetLocation());
			cocos2d::Vec2 p1 = Vector2(0.0f, 45.0f);
			cocos2d::Vec2 p2 = Vector2(20.0f, 120.0f);
			cocos2d::Vec2 dir = (p2-p1).getNormalized();
			_finger->setPosition(cell_pos + cocos2d::Vec2(25.0f, 180.0f) - view_size/2.0f);
			_finger->runAction(TutorialWidget::Arrow(dir, 70.0f, 0.6f));
			_finger->setVisible(true);
		}
	}

	if (_state == State::SHOW_FINGER && _tutorial.expired())
	{
		_state = State::HIDE;

		cocos2d::Size view = cocos2d::Director::getInstance()->getVisibleSize();
		cocos2d::MoveTo *move = cocos2d::MoveTo::create(0.5f, cocos2d::Vec2(view.width, 0.0f));
		cocos2d::CallFunc *func = cocos2d::CallFunc::create([&](){_state = State::FINISH;});
		_text->runAction(cocos2d::Sequence::create(move, func, nullptr));
		_finger->stopAllActions();
		_finger->runAction(cocos2d::FadeOut::create(0.5f));

		MessageManager::Instance().PutMessage(Message("EnableMapScrolling"));
	}
}

void TutorialWidgetInvestigationStarted::TouchesBegan(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
	if (_state == State::MOVE) {
		event->stopPropagation();
	}
}

void TutorialWidgetInvestigationStarted::TouchesMoved(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
	if (_state == State::MOVE) {
		event->stopPropagation();
	}
}

void TutorialWidgetInvestigationStarted::TouchesEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
	if (_state == State::MOVE) {
		event->stopPropagation();
	}
}

bool TutorialWidgetInvestigationStarted::IsReadyToClose(void) const
{
	return _tutorial.expired() && _state == State::FINISH;
}
