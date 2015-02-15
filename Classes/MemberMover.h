#ifndef MEMBER_MOVER_H
#define MEMBER_MOVER_H

#include "GameInterface.h"
#include "MemberWidget.h"
#include "MembersPage.h"

class MemberMover : public cocos2d::ui::Widget, public MessageReceiver
{
public:
	static MemberMover* create(const cocos2d::Size &size, MembersPage *transmitter, MembersSlot *receiver);
	
	virtual void AcceptMessage(const Message &message) override;
	virtual void setTouchEnabled(bool enabled) override;

protected:
	MemberMover(void);
	~MemberMover(void);

	bool initWithSize(const cocos2d::Size &size, MembersPage *transmitter, MembersSlot *receiver);
	void update(float dt) override;

	void OnTouch(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType eventType);

private:
	enum class State
	{
		NONE,
		ADJUST_FINGER,
		MOVE,
		ADJUST_SLOT
	};

	MemberWidget *_moveable;
	MembersPage *_pageTransmitter;
	MembersSlot *_pageReceiver;
	State _state;
	bool _release;
	float _time;
	float _dragBeginScale;
	cocos2d::Vec2 _dragBeginPos;
	cocos2d::ui::Widget *_dragedPageItem;

	static const cocos2d::Vec2 OFFSET_FROM_FINGER;
};

#endif