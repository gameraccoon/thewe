#include "MembersSlot.h"

MembersSlot* MembersSlot::create(float slotScale)
{
	MembersSlot *ret = new MembersSlot(slotScale);
	if (ret && ret->init())
	{
		ret->autorelease();
		return ret;
	}

	delete ret;
	return nullptr;
}

MembersSlot::MembersSlot(float slotScale)
	: _scale(slotScale)
{
}

MembersSlot::~MembersSlot(void)
{
}

bool MembersSlot::init(void)
{
	if (!cocos2d::ui::Layout::init()) {
		return false;
	}

	//setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
	//setBackGroundColor(cocos2d::Color3B::BLUE);

	return true;
}

void MembersSlot::RenewSlots(int membersNumber)
{
	float initialSizeX = 120.0f;
	float initialSizeY = 120.0f;

	setContentSize(cocos2d::Size(initialSizeX*_scale*membersNumber, initialSizeY*_scale));

	for (int k=0;k<membersNumber;++k) {
		MemberWidget *empty = MemberWidget::createEmpty(true);
		empty->setScale(_scale);
		empty->setPositionX(k*empty->getContentSize().width*_scale);
		addChild(empty);
	}
}
