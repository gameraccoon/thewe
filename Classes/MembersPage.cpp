#include "MembersPage.h"

const int MembersPage::PAGE_SIZE = 7;
const float MembersPage::SPACING = 5.0f;
const float MembersPage::SLOT_SIZE = 95.0f;
const float MembersPage::SLOT_SCALE = MembersPage::SLOT_SIZE / 120.0f;

MembersPage* MembersPage::create(void)
{
	MembersPage *ret = new MembersPage();
	if (ret && ret->init())
	{
		ret->autorelease();
		return ret;
	}

	delete ret;
	return nullptr;
}

MembersPage::MembersPage(void)
{
}

MembersPage::~MembersPage(void)
{
}

void MembersPage::FillInWithRealMembers(int number)
{
	// test code
	std::vector<MemberWidget *> members;
	for (int k=0;k<number;++k) {
		members.push_back(MemberWidget::createWithMember());
	}

	if (members.size() <= PAGE_SIZE) {
		setContentSize(cocos2d::Size(SLOT_SIZE*number + SPACING*(number+1), SLOT_SIZE + SPACING*2.0f));
		cocos2d::Vec2 pos = cocos2d::Vec2(SPACING, SPACING);
		for (auto member : members) {
			member->setPosition(pos);
			member->setScale(SLOT_SCALE);
			addWidgetToPage(member, 0, true);
			pos.x += SLOT_SIZE+SPACING;
		}
	} else {
		setContentSize(cocos2d::Size(SLOT_SIZE*PAGE_SIZE + SPACING*(PAGE_SIZE+1), SLOT_SIZE + SPACING*2.0f));
		int page=0;
		for (std::size_t index = 0; index < members.size(); ++index) {
			MemberWidget *widget = members[index];
			int place = index % PAGE_SIZE;
			widget->setPosition(cocos2d::Vec2(SLOT_SIZE*place + ((place+1)*SPACING), SPACING));
			widget->setScale(SLOT_SCALE);
			addWidgetToPage(widget, page, true);
			if (place+1 == PAGE_SIZE) {
				++page;
			}
		}
	}
}

void MembersPage::FillInWithEmptyMembers(int number)
{
		// test code
	std::vector<MemberWidget *> members;
	for (int k=0;k<number;++k) {
		members.push_back(MemberWidget::createEmpty(true));
	}

	if (members.size() <= PAGE_SIZE) {
		setContentSize(cocos2d::Size(SLOT_SIZE*number + SPACING*(number+1), SLOT_SIZE + SPACING*2.0f));
		cocos2d::Vec2 pos = cocos2d::Vec2(SPACING, SPACING);
		for (auto member : members) {
			member->setPosition(pos);
			member->setScale(SLOT_SCALE);
			addWidgetToPage(member, 0, true);
			pos.x += SLOT_SIZE+SPACING;
		}
	} else {
		Log::Instance().writeWarning("Empty slots can not scroll");
	}
}

void MembersPage::handleMoveLogic(cocos2d::Touch *touch)
{
}

bool MembersPage::init(void)
{
	if (!cocos2d::ui::PageView::init()) {
		return false;
	}

	setBackGroundImage("ui/members_page.png");
	setBackGroundImageOpacity(200);
	setBackGroundImageScale9Enabled(true);
	setBackGroundImageCapInsets(cocos2d::Rect(5,5,648,139));

	return true;
}
