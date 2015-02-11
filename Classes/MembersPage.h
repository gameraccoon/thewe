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

	void FillInWithRealMembers(int number);
	void FillInWithEmptyMembers(int number);

	virtual void handleMoveLogic(cocos2d::Touch *touch) override;

protected:
	MembersPage(void);
	~MembersPage(void);

	bool init(void) override;

private:
};

#endif