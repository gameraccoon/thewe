#ifndef MEMBERS_PAGE_H
#define MEMBERS_PAGE_H

#include "GameInterface.h"
#include "MemberWidget.h"

class MembersPage : public cocos2d::ui::PageView
{
public:
	static MembersPage* create(void);
	static const int MEMBERS_PAGE_SIZE;
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
	struct SlotInfo {
		int index;
		int page;
		cocos2d::Vec2 worldPos;
		Member::Ptr member;
	};

public:
	static MembersSlot* create(void);

	SlotInfo FindPlace(Member::Ptr member);

	void AddMember(const MembersSlot::SlotInfo &info);
	void Fill(int number) override;
	bool HaveFreeSlots(void);
	bool IsAbleToAddMember(Member::Ptr member);

protected:
	MembersSlot(void);
	virtual ~MembersSlot(void);

	bool init(void) override;

private:
	std::vector<SlotInfo> _slotsUnderConstruction;
};

#endif
