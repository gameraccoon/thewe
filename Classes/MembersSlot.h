#ifndef MEMBERS_SLOT_H
#define MEMBERS_SLOT_H

#include "GameInterface.h"
#include "MemberWidget.h"

class MembersSlot : public cocos2d::ui::Layout
{
public:
	static MembersSlot* create(float slotScale = 1.0f);

	void RenewSlots(int membersNumber);

protected:
	MembersSlot(float slotScale);
	~MembersSlot(void);

	bool init(void) override;

private:
	struct Slot {
		cocos2d::ui::Widget *widget;
		cocos2d::Vec2 center;
		bool empty;
	};

private:
	float _scale;
};

#endif
