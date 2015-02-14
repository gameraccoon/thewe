#ifndef MEMBERS_PAGE_H
#define MEMBERS_PAGE_H

#include "GameInterface.h"
#include "MemberWidget.h"

class MembersPage : public cocos2d::ui::PageView
{
public:
	static MembersPage* create(void);
	static const int PAGE_SIZE;
	static const float SPACING;
	static const float SLOT_SIZE;
	static const float SLOT_SCALE;

	virtual void Fill(int number);

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

	cocos2d::Vec2 GetFreeSlotPos(void);
	void AddMember(MemberWidget *memberWidget);
	void SwapMember(MemberWidget *memberWidget, int spawnIndex);
	void RemoveMember(MemberWidget *memberWidget);
	void Fill(int number) override;
	bool HaveFreeSlots(void);

protected:
	MembersSlot(void);
	virtual ~MembersSlot(void);

	bool init(void) override;
};

#endif
