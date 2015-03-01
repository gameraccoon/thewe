#ifndef MEMBERS_PAGE_H
#define MEMBERS_PAGE_H

#include "GameInterface.h"
#include "MemberWidget.h"
#include "Task.h"

class MembersPage : public cocos2d::ui::PageView
{
public:
	static MembersPage* create(void);
	static const int MEMBERS_PAGE_SIZE;
	static const float SPACING;
	static const float SLOT_SIZE;
	static const float SLOT_SCALE;

	virtual void FillWithMembers(const Member::Vector &members);

	virtual void handleMoveLogic(cocos2d::Touch *touch) override;

protected:
	MembersPage(void);
	virtual ~MembersPage(void);

	bool init(void) override;
};

class MembersSlot : public MembersPage
{
public:
	static MembersSlot* create(void);

	cocos2d::Vec2 FindPlace(Member::Ptr member);
	void AddMember(Member::Ptr membe);
	void RemoveMember(int tag);
	void FillByTaskRequire(Task::WeakPtr task);
	bool IsAbleToAddMember(Member::Ptr member);
	bool HaveFreeSlots(void) const;

protected:
	MembersSlot(void);
	virtual ~MembersSlot(void);

	bool init(void) override;

private:
	struct SlotInfo {
		cocos2d::Vec2 worldPos;
		MemberWidget *widget;
		Member::WeakPtr member;
		std::string sepcial;
		bool free;
		bool empty;
	};

private:
	std::vector<SlotInfo> _slots;
	Task::WeakPtr _task;
};

#endif
