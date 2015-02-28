#ifndef MEMBER_MOVER_H
#define MEMBER_MOVER_H

#include "GameInterface.h"
#include "MemberWidget.h"
#include "MembersPage.h"
#include "MiscUtils.h"

class MemberMover : public cocos2d::ui::Widget
{
public:
	static MemberMover* create(const cocos2d::Size &size, float scale,
		cocos2d::Vec2 v0, cocos2d::Vec2 v1, MembersSlot *slot, Member::WeakPtr member);
	
	enum class State {NONE, FLY, FINISH};

	virtual void setTouchEnabled(bool enable) override;

	bool IsState(MemberMover::State state) const;

protected:
	MemberMover(void);
	~MemberMover(void);

	bool init(const cocos2d::Size &size, float scale, cocos2d::Vec2 v0,
		cocos2d::Vec2 v1, MembersSlot *slot, Member::WeakPtr member);
	void update(float dt) override;

private:
	MemberWidget *_flyingItem;
	Member::WeakPtr _member;
	MembersSlot *_slot;
	Utils::Spline<cocos2d::Vec2> _path;
	State _state;
	float _scale;
	float _time;
};

#endif