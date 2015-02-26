#include "MemberMover.h"

MemberMover* MemberMover::create(const cocos2d::Size &size, float scale, cocos2d::Vec2 start,
								 MembersSlot *slot, MembersSlot::SlotInfo slotInfo)
{
	MemberMover *ret = new MemberMover();
	if (ret && ret->init(size, scale, start, slot, slotInfo))
	{
		ret->autorelease();
		return ret;
	}

	delete ret;
	return nullptr;
}

MemberMover::MemberMover(void)
	: _flyingItem(nullptr)
	, _state(State::NONE)
	, _time(0.0f)
{
}

MemberMover::~MemberMover(void)
{
}

bool MemberMover::init(const cocos2d::Size &size, float scale, cocos2d::Vec2 start,
		MembersSlot *slot, MembersSlot::SlotInfo slotInfo)
{
	if (!cocos2d::ui::Widget::init()) {
		return false;
	}

	_scale = scale;
	_slot = slot;
	_slotInfo = slotInfo;
	
	cocos2d::Vec2 v0 = start;
	cocos2d::Vec2 v1 = _slotInfo.worldPos;

	cocos2d::Vec2 d0 = (v1-v0).rotateByAngle(cocos2d::Vec2::ZERO, Math::PI*0.5f);
	_path.Clear();
	_path.AddKey(v0);
	_path.AddKey(v0*0.5f + v1*0.5f + d0.getNormalized()*-100.0f);
	_path.AddKey(v1);
	_path.CalculateGradient();
	_state = State::FLY;
	_flyingItem = MemberWidget::createWithMember(_slotInfo.member);
	_flyingItem->setPosition(v0);
	_flyingItem->setScale(scale);
	
	addChild(_flyingItem, 0);
	scheduleUpdate();
	setAnchorPoint(cocos2d::Vec2(0.0f,0.0f));
	setTouchEnabled(true);
	setContentSize(size);
	
	return true;
}

void MemberMover::update(float dt)
{
	if (_state == State::FLY)
	{
		float t_scale = 1.0f / 1.0f;
		_time += dt * t_scale;
		float t = _time;
		cocos2d::Vec2 p = _path.GetGlobalFrame(t);
		float scale = _scale + 0.5f*sinf(Math::PI * t);
		_flyingItem->setPosition(p);
		_flyingItem->setScale(scale);
		if (_time > 1.0f) {
			_time = 0.0f;
			_state = State::FINISH;
			_slot->AddMember(_slotInfo);
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

bool MemberMover::IsState(MemberMover::State state) const
{
	return _state == state;
}
