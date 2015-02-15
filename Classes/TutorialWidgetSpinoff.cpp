#include "TutorialWidgetSpinoff.h"

#include <luabind/luabind.hpp>

#include "MapProjector.h"
#include "WorldMapLayer.h"
#include "Vector2.h"
#include "Color.h"
#include "World.h"
#include "Log.h"

TutorialWidgetSpinoff::TutorialWidgetSpinoff(Tutorial::WeakPtr tutorial, WorldMapLayer *worldMapLayer, MapProjector *projector)
	: TutorialWidget(tutorial)
	, _worldMapLayer(worldMapLayer)
	, _projector(projector)
	, _state(State::FOCUS_ON_CELL)
	, _time(0.0f)
	, _viewFocusTime(0.0f)
	, _prevAlpha(0.8f)
	, _nextAlpha(0.8f)
	, _stateChangeTime(0.0f)
	, _viewFocused(false)
	, _mustChangeState(false)
{
}

TutorialWidgetSpinoff* TutorialWidgetSpinoff::create(Tutorial::WeakPtr tutorial, WorldMapLayer *worldMapLayer, MapProjector *projector)
{
	TutorialWidgetSpinoff* ret = new TutorialWidgetSpinoff(tutorial, worldMapLayer, projector);
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

bool TutorialWidgetSpinoff::init()
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

	_cell = World::Instance().GetCellsNetwork().GetRootCell();
	_viewStartLocation = _projector->GetLocation();
	_viewStartScale = _projector->GetScale();
	_spinoffTown = FindSuitableSpinoffTown();

	cocos2d::EventListenerTouchAllAtOnce *listener = cocos2d::EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(TutorialWidgetSpinoff::TouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(TutorialWidgetSpinoff::TouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(TutorialWidgetSpinoff::TouchesEnded, this);
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	cocos2d::Size view = cocos2d::Director::getInstance()->getVisibleSize();

	_text = cocos2d::ui::Text::create(_tutorial.lock()->text, "EuropeNormal.ttf", 28);
	_text->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
	_text->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
	_text->setPosition(cocos2d::Vec2(-view.width, 0.0f));
	_text->runAction(cocos2d::MoveTo::create(0.5f, cocos2d::Vec2(-220.0f, 0.0f)));

	_arrow = cocos2d::Sprite::create("tutorial/tutorial_arrow.png");
	_arrow->setScale(0.3f);
	_arrow->setRotation(-90.0f);
	_arrow->setVisible(false);

	_hand = cocos2d::Sprite::create("tutorial/tutorial_hand.png");
	_hand->setScale(0.5f);
	_hand->setPosition(0.0f, 0.0f);
	_hand->setAnchorPoint(cocos2d::Vec2(0.25f, 1.0f));
	_hand->setOpacity(0);
	_hand->setVisible(false);

	_roundSpot = cocos2d::Sprite::create("tutorial/cell_tutorial.png");
	_roundSpot->retain();

	_blackout = ScreenBlackoutWidget::create(cocos2d::Color4F(0.0f, 0.0f, 0.0f, 0.0f));
	_blackout->AddSpot(_roundSpot);

	addChild(_blackout, 0);
	addChild(_text, 1);
	addChild(_arrow, 2);
	addChild(_hand, 2);
	scheduleUpdate();

	MessageManager::Instance().PutMessage(Message("DisableMapScrolling"));

	return true;
}

void TutorialWidgetSpinoff::update(float dt)
{
	if (!_viewFocused)
	{
		float scale = 1.0f / 1.0f;
		_viewFocusTime += dt * scale;

		float t = Math::Clamp(1.0f, 0.0f, _viewFocusTime);
		Vector2 pos = _cell.lock()->GetLocation();
		_projector->SetLocation(Math::Lerp(_viewStartLocation, pos, t));
		_projector->SetScale(Math::Lerp(_viewStartScale, 1.0f, t));
		_blackout->SetBackgroundColor(cocos2d::Color4F(0.0f, 0.0f, 0.0f, Math::Lerp(0.0f, _nextAlpha, t)));

		_roundSpot->setPosition(_projector->ProjectOnScreen(pos));
		_roundSpot->setScale(_worldMapLayer->GetCellMapWidget(_cell)->getScale());

		if (_viewFocusTime > 1.0f)
		{
			_viewFocused = true;

			Vector2 view = cocos2d::Director::getInstance()->getVisibleSize();
			_arrow->setVisible(true);
			_arrow->setPosition(_projector->ProjectOnScreen(pos) + cocos2d::Vec2(0.0f, 65.0f) - view/2.0f);
			_arrow->runAction(TutorialWidget::Arrow(cocos2d::Vec2(0,-1), 40.0f, 0.9f));
		}
	}

	if (_mustChangeState) {
		float scale = 1.0f / 0.5f;
		_stateChangeTime += dt * scale;
		float time = Math::Clamp(1.0f, 0.0f, _stateChangeTime);
		float alpha = Math::Lerp(_prevAlpha, _nextAlpha, time);
		_blackout->SetBackgroundColor(cocos2d::Color4F(0.0f, 0.0f, 0.0f, alpha));
		if (_stateChangeTime > 1.0f) {
			_stateChangeTime = 0.0f;
			_mustChangeState = false;
		}
	}

	if (_worldMapLayer->IsCellMenuSpinoffMode() && _state == State::FOCUS_ON_BUTTON)
	{
		_state = State::FOCUS_ON_DRAG;

		Vector2 view = cocos2d::Director::getInstance()->getVisibleSize();
		Vector2 pos1 = _projector->ProjectOnScreen(_cell.lock()->GetLocation()) + cocos2d::Vec2(0.0f, 45.0f) - view/2.0f;
		Vector2 pos2 = _projector->ProjectOnScreen(_spinoffTown.lock()->GetInfo().location) - view/2.0f;

		_arrow->setVisible(false);
		_hand->setVisible(true);
		_hand->stopAllActions();
		_hand->runAction(MakeHandDragAction(pos1, pos2));

		_prevAlpha = _nextAlpha;
		_nextAlpha = 0.0f;
		_mustChangeState = true;
	}

	if (_worldMapLayer->IsCellMenuOpened() && _state == State::FOCUS_ON_CELL)
	{
		_state = State::FOCUS_ON_BUTTON;

		cocos2d::Size view = cocos2d::Director::getInstance()->getVisibleSize();
		Vector2 pos = _cell.lock()->GetLocation();

		_text->runAction(cocos2d::MoveTo::create(0.5f, cocos2d::Vec2(-view.width, 0.0f)));
		_roundSpot->setPosition(_roundSpot->getPosition() + cocos2d::Vec2(0.0f, 45.0f));
		_roundSpot->setScale(0.315f);
		_hand->setVisible(true);
		_hand->stopAllActions();
		_hand->runAction(MakeHandPressAction(_projector->ProjectOnScreen(pos) + cocos2d::Vec2(0.0f, 45.0f) - view/2.0f));
		_arrow->setVisible(false);

		_prevAlpha = _nextAlpha;
		_nextAlpha = 0.6f;
		_mustChangeState = true;
	}

	if ((!_worldMapLayer->IsCellMenuOpened() && _state == State::FOCUS_ON_BUTTON) ||
		(!_worldMapLayer->IsCellMenuSpinoffMode() && _state == State::FOCUS_ON_DRAG))
	{
		_state = State::FOCUS_ON_CELL;

		Vector2 view = cocos2d::Director::getInstance()->getVisibleSize();
		Vector2 pos = _cell.lock()->GetLocation();
		_text->runAction(cocos2d::MoveTo::create(0.5f, cocos2d::Vec2(-220.0f, 0.0f)));
		_roundSpot->setPosition(_projector->ProjectOnScreen(pos));
		_roundSpot->setScale(_worldMapLayer->GetCellMapWidget(_cell)->getScale());
		_arrow->stopAllActions();
		_arrow->setPosition(_projector->ProjectOnScreen(pos) + cocos2d::Vec2(0.0f, 65.0f) - view/2.0f);
		_arrow->runAction(TutorialWidget::Arrow(cocos2d::Vec2(0,-1), 40.0f, 0.9f));
		_arrow->setVisible(true);
		_hand->setVisible(false);

		_prevAlpha = _nextAlpha;
		_nextAlpha = 0.8f;
		_mustChangeState = true;

		MessageManager::Instance().PutMessage(Message("DisableMapScrolling"));
	}
}

void TutorialWidgetSpinoff::TouchesBegan(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
}

void TutorialWidgetSpinoff::TouchesMoved(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
}

void TutorialWidgetSpinoff::TouchesEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
}

Town::WeakPtr TutorialWidgetSpinoff::FindSuitableSpinoffTown(void) const
{
	float min_dist = 20.0f;
	float max_dist = 150.0f;

	cocos2d::Vec2 cell_pos = _cell.lock()->GetLocation();

	for (Town::Ptr town : World::Instance().GetTowns()) {
		cocos2d::Vec2 town_p = town->GetInfo().location;
		float distance = town_p.getDistance(cell_pos);
		if (distance > min_dist && distance < max_dist) {
			return town;
		}
	}

	// TODO: Write error to log.

	return Town::WeakPtr();
}

cocos2d::Action* TutorialWidgetSpinoff::MakeHandDragAction(cocos2d::Vec2 point1, cocos2d::Vec2 point2)
{
	cocos2d::FadeIn *fadein = cocos2d::FadeIn::create(0.3f);
	cocos2d::FadeOut *fadeout = cocos2d::FadeOut::create(0.3f);
	cocos2d::DelayTime *wait = cocos2d::DelayTime::create(0.2f);
	cocos2d::MoveTo *move0 = cocos2d::MoveTo::create(0.0f, point1 + cocos2d::Vec2(0.0f, 30.0f));
	cocos2d::MoveTo *move1 = cocos2d::MoveTo::create(0.6f, point1);
	cocos2d::MoveTo *move2 = cocos2d::MoveTo::create(1.1f, point2);

	cocos2d::Sequence *sequence = cocos2d::Sequence::create(move0, fadein, move1, wait, move2, fadeout, nullptr);

	return cocos2d::RepeatForever::create(sequence);
}

cocos2d::Action* TutorialWidgetSpinoff::MakeHandPressAction(cocos2d::Vec2 point)
{
	cocos2d::Vec2 point_from = point + cocos2d::Vec2(30.0f, 70.0f);

	cocos2d::MoveBy *shake1 = cocos2d::MoveBy::create(0.2f, cocos2d::Vec2(0.0f, 15.0f));
	cocos2d::MoveBy *shake2 = cocos2d::MoveBy::create(0.17f, cocos2d::Vec2(0.0f, -30.0f));
	cocos2d::MoveBy *shake3 = cocos2d::MoveBy::create(0.15f, cocos2d::Vec2(0.0f, 20.0f));
	cocos2d::MoveBy *shake4 = cocos2d::MoveBy::create(0.12f, cocos2d::Vec2(0.0f, -15.0f));
	cocos2d::MoveBy *shake5 = cocos2d::MoveBy::create(0.1f, cocos2d::Vec2(0.0f, 10.0f));
	cocos2d::Sequence *shaking = cocos2d::Sequence::create(shake1, shake2, shake3, shake4, shake5, nullptr);

	cocos2d::FadeIn *fadein = cocos2d::FadeIn::create(0.3f);
	cocos2d::FadeOut *fadeout = cocos2d::FadeOut::create(0.3f);
	cocos2d::MoveTo *move0 = cocos2d::MoveTo::create(0.0f, point_from);
	cocos2d::MoveTo *move1 = cocos2d::MoveTo::create(0.8f, point);

	cocos2d::Sequence *sequence = cocos2d::Sequence::create(move0, fadein, move1, shaking, fadeout, nullptr);

	return cocos2d::RepeatForever::create(sequence);
}
