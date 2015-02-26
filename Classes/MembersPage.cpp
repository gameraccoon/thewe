#include "MembersPage.h"

const int MembersPage::MEMBERS_PAGE_SIZE = 7;
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

void MembersPage::Fill(int number)
{
	// test code
	std::vector<MemberWidget *> members;
	for (int k=0;k<number;++k) {
		MemberWidget *widget = MemberWidget::createWithMember(Member::create("geek", 3));
		widget->setTag(k);
		members.push_back(widget);
	}

	if (members.size() <= MEMBERS_PAGE_SIZE) {
		setContentSize(cocos2d::Size(SLOT_SIZE*number + SPACING*(number+1), SLOT_SIZE + SPACING*2.0f));
		cocos2d::Vec2 pos = cocos2d::Vec2(SPACING, SPACING);
		for (auto member : members) {
			member->setPosition(pos);
			member->setScale(SLOT_SCALE);
			addWidgetToPage(member, 0, true);
			pos.x += SLOT_SIZE+SPACING;
		}
	} else {
		setContentSize(cocos2d::Size(SLOT_SIZE*MEMBERS_PAGE_SIZE + SPACING*(MEMBERS_PAGE_SIZE+1), SLOT_SIZE + SPACING*2.0f));
		int page=0;
		for (std::size_t index = 0; index < members.size(); ++index) {
			MemberWidget *widget = members[index];
			int place = index % MEMBERS_PAGE_SIZE;
			widget->setPosition(cocos2d::Vec2(SLOT_SIZE*place + ((place+1)*SPACING), SPACING));
			widget->setScale(SLOT_SCALE);
			addWidgetToPage(widget, page, true);
			if (place+1 == MEMBERS_PAGE_SIZE) {
				++page;
			}
		}
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

// MembersSlot implementation

MembersSlot* MembersSlot::create(void)
{
	MembersSlot *ret = new MembersSlot();
	if (ret && ret->init())
	{
		ret->autorelease();
		return ret;
	}

	delete ret;
	return nullptr;
}

MembersSlot::MembersSlot(void)
{
}

MembersSlot::~MembersSlot(void)
{
}

bool MembersSlot::init(void)
{
	if (!MembersPage::init()) {
		return false;
	}

	return true;
}

bool MembersSlot::IsAbleToAddMember(Member::Ptr member)
{
	if (!HaveFreeSlots()) {
		return false;
	}

	return true;
}

MembersSlot::SlotInfo MembersSlot::FindPlace(Member::Ptr member)
{
	SlotInfo info;

	for (auto page : getPages()) {
		for (auto item : page->getChildren()) {
			MemberWidget *widget;
			widget = dynamic_cast<MemberWidget *>(item);
			if (widget && widget->IsEmptyMemberWidget()) {
				bool alreadyCreated = false;
				for (SlotInfo s : _slotsUnderConstruction) {
					if (s.index == widget->getTag()) {
						alreadyCreated = true;
						break;
					}
				}
				if (!alreadyCreated) {
					info.page = 0;
					info.index = widget->getTag();
					info.member = member;
					info.worldPos = widget->getWorldPosition();
					_slotsUnderConstruction.push_back(info);
					return info;
				}
			}
		}
	}

	return info;
}

void MembersSlot::AddMember(const MembersSlot::SlotInfo &info)
{
	cocos2d::Node *page = getPage(info.page);
	if (!page) {Log::Instance().writeError("Failed to get page"); return;}
	cocos2d::Node *item = page->getChildByTag(info.index);
	if (!item) {Log::Instance().writeError("Faield to get item"); return;}

	MemberWidget *widget;
	widget = MemberWidget::createWithMember(info.member, true);
	widget->setPosition(item->getPosition());
	widget->setScale(MembersSlot::SLOT_SCALE);
	widget->setTag(item->getTag());

	page->removeChild(item);
	addWidgetToPage(widget, 0, true);

	for (std::vector<SlotInfo>::iterator it = _slotsUnderConstruction.begin(); it != _slotsUnderConstruction.end();) {
		if ((*it).member == info.member) {
			it = _slotsUnderConstruction.erase(it);
			break;
		} else ++it;
	}
}

void MembersSlot::Fill(int number)
{
	// test code
	std::vector<MemberWidget *> members;
	for (int k=0;k<number;++k) {
		MemberWidget *widget = MemberWidget::createEmpty("geek");
		widget->setTag(k);
		members.push_back(widget);

	}

	if (members.size() <= MEMBERS_PAGE_SIZE) {
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

bool MembersSlot::HaveFreeSlots(void)
{
	int emptySlotsNumber = 0;

	for (auto widget : getPages().at(0)->getChildren()) {
		MemberWidget *member = dynamic_cast<MemberWidget *>(widget);
				
		bool deffered = false;
		for (SlotInfo s : _slotsUnderConstruction) {
			if (s.index == widget->getTag()) {
				deffered = true;
				break;
			}
		}

		if (!deffered && member && member->IsEmptyMemberWidget()) {
			++emptySlotsNumber;
		}
	}
	return emptySlotsNumber > 0;
}
