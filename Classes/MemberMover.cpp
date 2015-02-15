#include "MemberMover.h"

const cocos2d::Vec2 MemberMover::OFFSET_FROM_FINGER = cocos2d::Vec2(60.0f, 20.0f);

MemberMover* MemberMover::create(const cocos2d::Size &size, MembersPage *transmitter, MembersSlot *receiver)
{
	MemberMover *ret = new MemberMover();
	if (ret && ret->initWithSize(size, transmitter, receiver))
	{
		ret->autorelease();
		return ret;
	}

	delete ret;
	return nullptr;
}

MemberMover::MemberMover(void)
	: _moveable(nullptr)
	, _pageTransmitter(nullptr)
	, _pageReceiver(nullptr)
	, _dragedPageItem(nullptr)
	, _state(State::NONE)
	, _time(0.0f)
	, _release(false)
{
	MessageManager::Instance().RegisterReceiver(this, "BeginMemberMove");
}

MemberMover::~MemberMover(void)
{
	MessageManager::Instance().UnregisterReceiver(this, "BeginMemberMove");
}

bool MemberMover::initWithSize(const cocos2d::Size &size, MembersPage *transmitter, MembersSlot *receiver)
{
	if (!cocos2d::ui::Widget::init()) {
		return false;
	}

	_pageTransmitter = transmitter;
	_pageReceiver = receiver;
	
	scheduleUpdate();
	setAnchorPoint(cocos2d::Vec2(0.0f,0.0f));
	setTouchEnabled(true);
	setContentSize(size);
	addTouchEventListener(CC_CALLBACK_2(MemberMover::OnTouch, this));
	
	return true;
}

void MemberMover::AcceptMessage(const Message &message)
{
	if (message.is("BeginMemberMove"))
	{
		for (auto page : _pageTransmitter->getPages()) {
			for (auto child : page->getChildren()) {
				if (child->getTag() == message.variables.GetInt("Index")) {
					_dragedPageItem = dynamic_cast<cocos2d::ui::Widget *>(child);
					_dragBeginPos = _dragedPageItem->getWorldPosition();
					_dragBeginScale = _dragedPageItem->getScale();
					_moveable = MemberWidget::createWithMember();
					_moveable->setPosition(_dragBeginPos);
					_moveable->setScale(_dragBeginScale);
					addChild(_moveable);
					_dragedPageItem->setVisible(false);
					return;
				}
			}
		}
	}
}

void MemberMover::setTouchEnabled(bool enable)
{
    if (enable == _touchEnabled)
    {
        return;
    }
    _touchEnabled = enable;
    if (_touchEnabled)
    {
        _touchListener = cocos2d::EventListenerTouchOneByOne::create();
        CC_SAFE_RETAIN(_touchListener);
        _touchListener->setSwallowTouches(false);
        _touchListener->onTouchBegan = CC_CALLBACK_2(Widget::onTouchBegan, this);
        _touchListener->onTouchMoved = CC_CALLBACK_2(Widget::onTouchMoved, this);
        _touchListener->onTouchEnded = CC_CALLBACK_2(Widget::onTouchEnded, this);
        _touchListener->onTouchCancelled = CC_CALLBACK_2(Widget::onTouchCancelled, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener, this);
    }
    else
    {
        _eventDispatcher->removeEventListener(_touchListener);
        CC_SAFE_RELEASE_NULL(_touchListener);
    }
}

void MemberMover::update(float dt)
{
	if (_release) {
		removeChild(_moveable, true);
		_moveable = nullptr;
		_dragedPageItem = nullptr;
		_release = false;
		return;
	}
	if (!_moveable) {
		return;
	}

	if (_state == State::ADJUST_FINGER) {
		float scale = 1.0f/0.15f;
		_time += dt * scale;
		cocos2d::Vec2 pos;
		cocos2d::Vec2 offset = OFFSET_FROM_FINGER * _moveable->getScale();
		pos = Math::Lerp(_dragBeginPos, getTouchMovePosition() - offset, _time);
		_moveable->setPosition(pos);
		_moveable->setScale(Math::Lerp(_dragBeginScale, 1.0f, _time));
		if (_time >= 1.0f) {
			_time = 0.0f;
			_state = State::MOVE;
		}
	}
	if (_state == State::MOVE) {
		_moveable->setPosition(getTouchMovePosition() - OFFSET_FROM_FINGER);
	}
}

void MemberMover::OnTouch(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType eventType)
{
	if (eventType == cocos2d::ui::Widget::TouchEventType::MOVED)
	{
		if (_state == State::NONE) {
			_state = State::ADJUST_FINGER;
		}
		/*
		cocos2d::Vec2 pos;
		pos = _moveable->getPosition();
		pos += OFFSET_FROM_FINGER * _moveable->getScale();
		if ( _pageReceiver->hitTest(pos)) {
			if (_pageReceiver->IsSlotsEmpty()) {

			} else {
			}
		}
		*/
	}

	if (eventType == cocos2d::ui::Widget::TouchEventType::ENDED ||
		eventType == cocos2d::ui::Widget::TouchEventType::CANCELED)
	{
		if (_dragedPageItem && (_state == State::MOVE || _state == State::ADJUST_FINGER))
		{
			cocos2d::Vec2 pos;
			pos = _moveable->getPosition();
			pos += OFFSET_FROM_FINGER * _moveable->getScale();
			cocos2d::Vec2 endPos = cocos2d::Vec2::ZERO;
			cocos2d::CallFunc *func;
			if (_pageReceiver->hitTest(pos) && _pageReceiver->HaveFreeSlots()) {
				endPos = _pageReceiver->GetFreeSlotPos();
				func = cocos2d::CallFunc::create([&](){
					_pageReceiver->AddMember(_moveable);
					_state = State::NONE;
					_dragedPageItem->setVisible(true);
					_release = true;
				});
			} else {
				endPos = _dragedPageItem->getWorldPosition();
				func = cocos2d::CallFunc::create([&](){
					_state = State::NONE;
					_dragedPageItem->setVisible(true);
					_release = true;
				});
			}

			_state = State::ADJUST_SLOT;
			_time = 0.0f;
			cocos2d::MoveTo *move = cocos2d::MoveTo::create(0.15f, endPos);
			cocos2d::ScaleTo *scale = cocos2d::ScaleTo::create(0.15f, MembersPage::SLOT_SCALE);
			cocos2d::Spawn *motion = cocos2d::Spawn::create(move, scale, nullptr);
			cocos2d::Sequence *seq = cocos2d::Sequence::create(motion, func, nullptr);
			_moveable->runAction(seq);
		}
	}
}
