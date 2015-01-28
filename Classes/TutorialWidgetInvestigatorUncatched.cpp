#include "TutorialWidgetInvestigatorUncatched.h"

#include <luabind/luabind.hpp>

#include "WorldMapLayer.h"
#include "Vector2.h"
#include "Color.h"
#include "World.h"
#include "Log.h"

TutorialWidgetInvestigatorUncatched::TutorialWidgetInvestigatorUncatched(Tutorial::WeakPtr tutorial,
																		 WorldMapLayer *worldMapLayer,
																		 MapProjector *projector)
    : TutorialWidget(tutorial)
	, _worldMapLayer(worldMapLayer)
	, _projector(projector)
	, _state(State::MOVE)
	, _prevAlpha(0.0f)
	, _nextAlpha(0.0f)
	, _localTime(0.0f)
	, _time(0.0f)
	, _alphaTime(0.0f)
	, _mustChangeAlpha(false)
	, _textHiden(false)
{
}

TutorialWidgetInvestigatorUncatched* TutorialWidgetInvestigatorUncatched::create(Tutorial::WeakPtr tutorial,
																				 WorldMapLayer *worldMapLayer,
																				 MapProjector *projector)
{
	TutorialWidgetInvestigatorUncatched* ret = new TutorialWidgetInvestigatorUncatched(tutorial, worldMapLayer, projector);
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

bool TutorialWidgetInvestigatorUncatched::init()
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

	_viewStartPos = _projector->GetLocation();
	_viewStartScale = _projector->GetScale();

	if (!World::Instance().GetInvestigators().empty()) {
		Investigator::WeakPtr inves = World::Instance().GetInvestigators().at(0);
		_cell = inves.lock()->GetInvestigationRoot();
	} else {
		Log::Instance().writeError("No investigators avaliable");
	}

	cocos2d::EventListenerTouchAllAtOnce *listener = cocos2d::EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(TutorialWidgetInvestigatorUncatched::TouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(TutorialWidgetInvestigatorUncatched::TouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(TutorialWidgetInvestigatorUncatched::TouchesEnded, this);
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	Cell::Ptr cell_ptr = _cell.lock();

	if (!cell_ptr->GetParent().expired() && cell_ptr->GetParent().lock()->IsState(Cell::State::READY))
	{
		Cell::Ptr parent_ptr = cell_ptr->GetParent().lock();
		cocos2d::Sprite *arrow;
		arrow = cocos2d::Sprite::create("tutorial/tutorial_arrow.png");
		arrow->setRotation(-90.0f);
		arrow->retain();
		_arrows.push_back(Arrow(parent_ptr, arrow));
		addChild(arrow, 1);
	}

	for (auto cell : cell_ptr->GetChildren()) {
		cocos2d::Sprite *arrow;
		arrow = cocos2d::Sprite::create("tutorial/tutorial_arrow.png");
		arrow->setRotation(-90.0f);
		arrow->retain();
		_arrows.push_back(Arrow(cell, arrow));
		addChild(arrow, 1);
	}

	_spot = cocos2d::Sprite::create("tutorial/cell_tutorial.png");
	_spot->setPosition(100.0f, 100.0f);
	_spot->setScale(3.0f);
	_spot->retain();

	_blackout = ScreenBlackoutWidget::create(cocos2d::Color4F(0.0f, 0.0f, 0.0f, 0.0f));
	_blackout->AddSpot(_spot);
	
	cocos2d::Vec2 view = cocos2d::Director::getInstance()->getVisibleSize();
	_text = cocos2d::ui::Text::create(_tutorial.lock()->text, "EuropeNormal.ttf", 28);
	_text->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
	_text->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
	_text->setTextAreaSize(cocos2d::Size(300.0f, 500.0f));
	_text->setPosition(cocos2d::Vec2(view.x, 0.0f));
	_text->runAction(cocos2d::MoveTo::create(0.5f, cocos2d::Vec2(220.0f, 0.0f)));

	addChild(_blackout, 0);
	addChild(_text, 2);
	scheduleUpdate();

	MessageManager::Instance().PutMessage(Message("DisableMapScrolling"));

    return true;
}

void TutorialWidgetInvestigatorUncatched::update(float dt)
{
	if (_state == State::MOVE) {
		float scale = 1.0f / 0.5f;
		_time += dt * scale;
		float t = Math::Clamp(1.0f, 0.0f, _time);
		_projector->SetLocation(Math::Lerp(_viewStartPos, _cell.lock()->GetInfo().location, t));
		_projector->SetScale(Math::Lerp(_viewStartScale, 1.2f, t));
		_worldMapLayer->UpdateMapElements();
		if (_time > 1.0f) {
			_time = 0.0f;
			_state = State::WAIT;
			MessageManager::Instance().PutMessage(Message("EnableMapScrolling"));
		}
	}

	if (_mustChangeAlpha) {
		float scale = 1.0f / 0.5f;
		_alphaTime += dt * scale;
		float time = Math::Clamp(1.0f, 0.0f, _alphaTime);
		float alpha = Math::Lerp(_prevAlpha, _nextAlpha, time);
		_blackout->SetBackgroundColor(cocos2d::Color4F(0.0f, 0.0f, 0.0f, alpha));
		if (_alphaTime > 1.0f) {
			_alphaTime = 0.0f;
			_mustChangeAlpha = false;
		}
	}

	cocos2d::Vec2 view = cocos2d::Director::getInstance()->getVisibleSize();

	for (std::vector<Arrow>::iterator it = _arrows.begin(); it != _arrows.end();)
	{
		if (!(*it).first.expired())
		{
			Cell::Ptr cell_ptr = (*it).first.lock();
			cocos2d::Sprite *sprite = (*it).second;

			float y = 12.0f * sinf(Math::PI*2.0f * _localTime*1.5f);
			if (_worldMapLayer->IsCellMenuOpenedFor(cell_ptr)) {
				y += 60.0f;
			}

			cocos2d::Vec2 cell_pos = _projector->ProjectOnScreen(cell_ptr->GetInfo().location + Vector2(0.0f, 45.0f));
			const float scale = _worldMapLayer->GetCellMapWidget(cell_ptr)->getScale();
			sprite->setPosition(cell_pos + cocos2d::Vec2(0,y) - view*0.5f);
			sprite->setScale(scale);

			++it;
		}
		else
		{
			removeChild((*it).second);
			it = _arrows.erase(it);
		}
	}

	cocos2d::Vec2 spot_pos;
	for (auto arrow : _arrows) {
		if (_worldMapLayer->IsCellMenuOpenedFor(arrow.first)) {
			spot_pos = _projector->ProjectOnScreen(arrow.first.lock()->GetInfo().location);
		}
	}
	_spot->setPosition(spot_pos + cocos2d::Vec2(0.0f, 50.0f));
	_spot->setScale(0.29f);

	if (_localTime > 7.0f && !_textHiden) {
		cocos2d::Vec2 view = cocos2d::Director::getInstance()->getVisibleSize();
		_text->runAction(cocos2d::MoveTo::create(0.5f, cocos2d::Vec2(view.x, 0.0f)));
		_textHiden = true;
	}

	if (_worldMapLayer->IsCellMenuOpened() && _state == State::WAIT) {
		_state = State::FOCUS;
		_prevAlpha = _nextAlpha;
		_nextAlpha = 0.9f;
		_mustChangeAlpha = true;
	}

	if (!_worldMapLayer->IsCellMenuOpened() && _state == State::FOCUS) {
		_state = State::WAIT;
		_prevAlpha = _nextAlpha;
		_nextAlpha = 0.0f;
		_mustChangeAlpha = true;
	}

	_localTime += dt;
}

void TutorialWidgetInvestigatorUncatched::TouchesBegan(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
	if (_state == State::MOVE) {
		event->stopPropagation();
	}
}

void TutorialWidgetInvestigatorUncatched::TouchesMoved(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
	if (_state == State::MOVE) {
		event->stopPropagation();
	}
}

void TutorialWidgetInvestigatorUncatched::TouchesEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
	if (_state == State::MOVE) {
		event->stopPropagation();
	}
}

bool TutorialWidgetInvestigatorUncatched::IsReadyToClose(void) const
{
	return _tutorial.expired();
}