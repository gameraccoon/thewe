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

void MembersPage::FillWithMembers(const Member::Vector &members)
{
	removeAllPages();

	std::vector<MemberWidget *> membersWidgets;
	std::size_t amount = members.size();
	for (std::size_t k = 0; k < amount; ++k) {
		Member::Ptr member = members[k];
		if (member->IsState(Member::State::NORMAL)) {
			MemberWidget *widget = MemberWidget::createWithMember(member);
			widget->setTag(k);
			membersWidgets.push_back(widget);
		}
	}

	if (membersWidgets.size() <= MEMBERS_PAGE_SIZE) {
		setContentSize(cocos2d::Size(SLOT_SIZE*amount + SPACING*(amount+1), SLOT_SIZE + SPACING*2.0f));
		cocos2d::Vec2 pos = cocos2d::Vec2(SPACING, SPACING);
		for (auto member : membersWidgets) {
			member->setPosition(pos);
			member->setScale(SLOT_SCALE);
			addWidgetToPage(member, 0, true);
			pos.x += SLOT_SIZE+SPACING;
		}
	} else {
		setContentSize(cocos2d::Size(SLOT_SIZE*MEMBERS_PAGE_SIZE + SPACING*(MEMBERS_PAGE_SIZE+1), SLOT_SIZE + SPACING*2.0f));
		int page=0;
		for (std::size_t index = 0; index < membersWidgets.size(); ++index) {
			MemberWidget *widget = membersWidgets[index];
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

cocos2d::Vec2 MembersSlot::FindPlace(Member::Ptr member)
{
	for (SlotInfo &slot : _slots) {
		if (slot.widget && slot.free && slot.empty) {
			slot.free = false;
			slot.member = member;
			return slot.widget->getWorldPosition();
		}
	}
	return cocos2d::Vec2::ZERO;
}

void MembersSlot::AddMember(Member::Ptr member)
{
	for (SlotInfo &slot : _slots) {
		if (slot.member.lock() == member && !slot.free) {
			MemberWidget *widget;
			widget = MemberWidget::createWithMember(member, true, false);
			widget->setPosition(slot.widget->getPosition());
			widget->setScale(MembersSlot::SLOT_SCALE);
			widget->setTag(slot.widget->getTag());

			cocos2d::ui::Layout *page = getPage(0);
			if (page) {
				page->removeChild(slot.widget);
				slot.widget = nullptr;
			}

			addWidgetToPage(widget, 0, true);

			slot.widget = widget;
			slot.worldPos = widget->getWorldPosition();
			slot.free = true;
			slot.empty = false;

			break;
		}
	}
}

void MembersSlot::RemoveMember(int tag)
{
	cocos2d::Node *page = getPage(0);
	if (!page) {Log::Instance().writeError("Failed to get page."); return;}
	cocos2d::Node *item = page->getChildByTag(tag);
	if (!item) {Log::Instance().writeError("Failed to get item."); return;}

	MemberWidget *widget = dynamic_cast<MemberWidget *>(item);
	MemberWidget *empty = MemberWidget::createEmpty("geek");

	for (SlotInfo &slot : _slots) {
		if (slot.widget == widget) {
			slot.widget = empty;
			slot.member = Member::Ptr();
			slot.empty = true;
			slot.free = true;
			break;
		}
	}

	empty->setPosition(widget->getPosition());
	empty->setScale(MembersSlot::SLOT_SCALE);
	empty->setTag(widget->getTag());

	getPage(0)->removeChild(widget);
	addWidgetToPage(empty, 0, true);
}

void MembersSlot::FillByTaskRequire(Task::Ptr task)
{
	removeAllPages();
	_slots.clear();

	Task::Info taskInfo = task->GetInfo();
	
	int k = 0;
	for (auto executants : taskInfo.members) {
		for (int i = 0; i < executants.count; i++) {
			MemberWidget *widget = MemberWidget::createEmpty(executants.special);
			widget->setTag(k);
			++k;
			MembersSlot::SlotInfo slot;
			slot.free = true;
			slot.empty = true;
			slot.member = Member::Ptr();
			slot.widget = widget;
			_slots.push_back(slot);
		}
	}

	if (_slots.size() <= MEMBERS_PAGE_SIZE) {
		std::size_t amount = _slots.size();
		setContentSize(cocos2d::Size(SLOT_SIZE*amount + SPACING*(amount+1), SLOT_SIZE + SPACING*2.0f));
		cocos2d::Vec2 pos = cocos2d::Vec2(SPACING, SPACING);
		for (auto &slot : _slots) {
			slot.widget->setPosition(pos);
			slot.widget->setScale(SLOT_SCALE);
			slot.worldPos = slot.widget->getWorldPosition();
			addWidgetToPage(slot.widget, 0, true);
			pos.x += SLOT_SIZE+SPACING;
		}
	} else {
		Log::Instance().writeWarning("Empty slots can not scroll");
	}
}

bool MembersSlot::HaveFreeSlots(void) const
{
	for (SlotInfo slot : _slots) {
		if (slot.empty && slot.free) {
			return true;
		}
	}
	return false;
}
