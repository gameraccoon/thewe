#include "TutorialWidgetAfterFirstCell.h"

#include <luabind/luabind.hpp>

#include "WorldMapLayer.h"
#include "MapProjector.h"
#include "Vector2.h"
#include "Color.h"
#include "World.h"
#include "Log.h"

TutorialWidgetAfterFirstCell::TutorialWidgetAfterFirstCell(Tutorial::WeakPtr tutorial, WorldMapLayer *worldMapLayer, MapProjector *projector)
	: TutorialWidget(tutorial)
	, _worldMapLayer(worldMapLayer)
	, _projector(projector)
	, _state(State::APPEAR)
	, _time(0.0f)
{
}

TutorialWidgetAfterFirstCell* TutorialWidgetAfterFirstCell::create(Tutorial::WeakPtr tutorial, WorldMapLayer *worldMapLayer, MapProjector *projector)
{
	TutorialWidgetAfterFirstCell* ret = new TutorialWidgetAfterFirstCell(tutorial, worldMapLayer, projector);
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

bool TutorialWidgetAfterFirstCell::init()
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

	_firstCell = World::Instance().GetCellsNetwork().GetRootCell();
	_startViewLocation = _projector->GetLocation();
	_targetViewLocation = _firstCell.lock()->GetLocation();

	cocos2d::EventListenerTouchAllAtOnce *listener = cocos2d::EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(TutorialWidgetAfterFirstCell::TouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(TutorialWidgetAfterFirstCell::TouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(TutorialWidgetAfterFirstCell::TouchesEnded, this);
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	cocos2d::Size view = cocos2d::Director::getInstance()->getVisibleSize();
	_tutorialText = cocos2d::ui::Text::create(_tutorial.lock()->text, "EuropeNormal.ttf", 28);
	_tutorialText->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
	_tutorialText->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
	_tutorialText->setPosition(cocos2d::Vec2(-view.width, 0.0f));

	_arrow = cocos2d::Sprite::create("tutorial_arrow.png");
	_arrow->setScale(0.35f);
	_arrow->setRotation(-90.0f);
	_arrow->setVisible(false);
	_arrow->runAction(TutorialWidget::Arrow(cocos2d::Vec2(0,-1), 40.0f, 0.9f));

	_spot = cocos2d::Sprite::create("cell_tutorial.png");
	_spot->setPosition(0.0f, 0.0f);
	_spot->retain();

	_blackout = ScreenBlackoutWidget::create(cocos2d::Color4F(0.0f, 0.0f, 0.0f, 0.85f));
	_blackout->AddSpot(_spot);

	addChild(_blackout, 0);
	addChild(_tutorialText, 1);
	addChild(_arrow, 1);
	scheduleUpdate();

	return true;
}

void TutorialWidgetAfterFirstCell::update(float dt)
{
	if (_state == State::APPEAR)
	{
		float scale = 1.0f / 0.8f;
		_time += dt * scale;
		_projector->SetLocation(Math::Lerp(_startViewLocation, _targetViewLocation, _time));

		float t = Math::Clamp(1.0f, 0.0f, _time*2.5f);
		float alpha = Math::Lerp(0.0f, 0.85f, t);
		_blackout->SetBackgroundColor(cocos2d::Color4F(0.0f, 0.0f, 0.0f, alpha));

		_spot->setPosition(_projector->ProjectOnScreen(_targetViewLocation));
		_spot->setScale(_worldMapLayer->GetCellMapWidget(_firstCell)->getScale());

		if (_time > 1.0f)
		{
			_time = 0.0f;
			_state = State::FOCUS_ON_CELL;
			MessageManager::Instance().PutMessage(Message("DisableMapScrolling"));

			cocos2d::Size view = cocos2d::Director::getInstance()->getVisibleSize();
			cocos2d::Vec2 pos = _projector->ProjectOnScreen(_targetViewLocation) + cocos2d::Vec2(0.0f, 100.0f);
			_arrow->setPosition(pos - view / 2.0f); // compensate node position (in the middle of the screen)
			_arrow->setVisible(true);

			cocos2d::MoveTo *move = cocos2d::MoveTo::create(0.5f, cocos2d::Vec2(-215.0f, 0.0f));
			_tutorialText->runAction(move);
		}
	}

	if (_state == State::DISAPPEAR) {
		float scale = 1.0f / 0.25f;
		_time += dt * scale;
		float alpha = Math::Lerp(0.85f, 0.0f, _time);
		_blackout->SetBackgroundColor(cocos2d::Color4F(0.0f, 0.0f, 0.0f, alpha));
		if (_time > 1.0f) {
			_time = 0.0f;
			_state = State::FINISH;
			MessageManager::Instance().PutMessage(Message("EnableMapScrolling"));
		}
	}

	if (_state == State::FOCUS_ON_CELL && _worldMapLayer->IsCellMenuOpened())
	{
		_state = State::FOCUS_ON_BUTTON;

		cocos2d::Size view = cocos2d::Director::getInstance()->getVisibleSize();
		cocos2d::Vec2 pos = _spot->getPosition() + cocos2d::Vec2(38.97f, -22.5f);

		_spot->setPosition(pos);
		_spot->setScale(0.315f);

		_arrow->stopAllActions();
		_arrow->setRotation(0.0f);
		_arrow->setPosition((pos + cocos2d::Vec2(120.0f, 0.0f)) - view/2.0f);
		_arrow->runAction(TutorialWidget::Arrow(cocos2d::Vec2(1,0), 40.0f, 0.9f));
	}

	if (_state == State::FOCUS_ON_BUTTON && !_worldMapLayer->IsCellMenuOpened())
	{
		_state = State::FOCUS_ON_CELL;

		cocos2d::Size view = cocos2d::Director::getInstance()->getVisibleSize();
		cocos2d::Vec2 pos = _projector->ProjectOnScreen(_targetViewLocation);

		_spot->setPosition(pos);
		_spot->setScale(_worldMapLayer->GetCellMapWidget(_firstCell)->getScale());

		_arrow->stopAllActions();
		_arrow->setRotation(-90.0f);
		_arrow->setPosition((pos + cocos2d::Vec2(0.0f, 100.0f)) - view/2.0f);
		_arrow->runAction(TutorialWidget::Arrow(cocos2d::Vec2(0,-1), 40.0f, 0.9f));
	}

	if (_state == State::FOCUS_ON_BUTTON && _tutorial.expired())
	{
		_state = State::DISAPPEAR;
		cocos2d::Size view = cocos2d::Director::getInstance()->getVisibleSize();
		_tutorialText->runAction(cocos2d::MoveBy::create(0.2f, cocos2d::Vec2(-view.width, 0.0f)));
	}
}

void TutorialWidgetAfterFirstCell::TouchesBegan(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
	if (_state != State::FOCUS_ON_CELL && _state != State::FOCUS_ON_BUTTON) {
		event->stopPropagation();
	}
}

void TutorialWidgetAfterFirstCell::TouchesMoved(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
	if (_state != State::FOCUS_ON_CELL && _state != State::FOCUS_ON_BUTTON) {
		event->stopPropagation();
	}
}

void TutorialWidgetAfterFirstCell::TouchesEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
	if (_state != State::FOCUS_ON_CELL && _state != State::FOCUS_ON_BUTTON) {
		event->stopPropagation();
	}
}

bool TutorialWidgetAfterFirstCell::IsReadyToClose(void) const
{
	return _tutorial.expired() && _state == State::FINISH;
}
