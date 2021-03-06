#include "MemberWidget.h"

MemberWidget* MemberWidget::createWithMember(Member::Ptr member, bool withRemoveButton, bool moveable)
{
	MemberWidget *ret = new MemberWidget();
	if (ret && ret->initWithMember(member, withRemoveButton, moveable))
	{
		ret->autorelease();
		return ret;
	}

	delete ret;
	return nullptr;
}

MemberWidget* MemberWidget::createEmpty(const std::string &specialType)
{
	MemberWidget *ret = new MemberWidget();
	if (ret && ret->initEmpty(specialType))
	{
		ret->autorelease();
		return ret;
	}

	delete ret;
	return nullptr;
}

MemberWidget::MemberWidget(void)
	: _removeButton(nullptr)
{
}

MemberWidget::~MemberWidget(void)
{
}

bool MemberWidget::initWithMember(Member::Ptr member, bool withRemoveButton, bool moveable)
{
	if (!cocos2d::ui::Layout::init()) {
		return false;
	}

	_member = member;
	_isMoveable = moveable;
	_isEmpty = false;
	
	std::string icon = GetIconForSpecial(_member->getSpecialization());
	_special = cocos2d::Sprite::create(icon);
	_special->setPosition(80.0f, 95.0f);

	_face = cocos2d::Sprite::create("ui/human_face.png");
	_face->setPosition((cocos2d::Vec2)_face->getContentSize()*0.5f);
	
	_background = cocos2d::Sprite::create("ui/human_bg.png");
	_background->setPosition((cocos2d::Vec2)_background->getContentSize() * 0.5f);

	float star_x = 13.0f;
	for (int i = 0; i < 5; ++i) {
		cocos2d::Sprite *star;
		star = cocos2d::Sprite::create("ui/star_1.png");
		star->setPosition(star_x, 16.0f);
		addChild(star, 3);
		_stars.push_back(star);
		star_x += 24.0f;
	}

	if (withRemoveButton) {
		_removeButton = cocos2d::ui::Button::create("ui/member_remove_normal.png", "ui/member_remove_pressed.png");
		_removeButton->addTouchEventListener(CC_CALLBACK_2(MemberWidget::OnRemovePressed, this));
		_removeButton->setPosition(cocos2d::Vec2(19.0f, 100.0f));
		addChild(_removeButton, 3);
	}

	addChild(_background, 0);
	addChild(_face, 1);
	addChild(_special, 2);

	setAnchorPoint(cocos2d::Vec2(0.0f, 0.0f));
	setContentSize(_background->getContentSize());
	addTouchEventListener(CC_CALLBACK_2(MemberWidget::TouchListener, this));
	setTouchEnabled(true);

	return true;
}

bool MemberWidget::initEmpty(const std::string &specialType)
{
	if (!cocos2d::ui::Layout::init()) {
		return false;
	}

	_isEmpty = true;
	_isMoveable = false;

	_background = cocos2d::Sprite::create("ui/human_slot.png");
	_background->setPosition((cocos2d::Vec2)_background->getContentSize() * 0.5f);

	_face = cocos2d::Sprite::create("ui/human_face.png");
	_face->setPosition((cocos2d::Vec2)_face->getContentSize()*0.5f);
	_face->setOpacity(128);

	if (!specialType.empty()) {
		std::string icon = GetIconForSpecial(specialType);
		_special = cocos2d::Sprite::create(icon);
		_special->setPosition(80.0f, 95.0f);
		addChild(_special, 2);
	}

	addChild(_background, 0);
	addChild(_face, 1);

	setAnchorPoint(cocos2d::Vec2(0.0f, 0.0f));
	setContentSize(_background->getContentSize());
	addTouchEventListener(CC_CALLBACK_2(MemberWidget::TouchListener, this));
	setTouchEnabled(true);

	return true;
}

Member::Ptr MemberWidget::GetMemberPtr(void) const
{
	return _member;
}

bool MemberWidget::IsEmpty(void) const
{
	return _isEmpty;
}

bool MemberWidget::IsMoveable(void) const
{
	return _isMoveable;
}

void MemberWidget::OnRemovePressed(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType eventType)
{
	if (eventType == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		std::function<void()> func = [&]()
		{
			Message message("RemoveMemberFromSlot");
			message.variables.SetInt("Tag", getTag());
			MessageManager::Instance().PutMessage(message);
		};

		cocos2d::FadeOut *fade = cocos2d::FadeOut::create(0.5f);
		cocos2d::CallFunc *call = cocos2d::CallFunc::create(func);
		cocos2d::Sequence *effect = cocos2d::Sequence::create(fade, call, nullptr);
		runAction(effect);
	}
}

void MemberWidget::TouchListener(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType eventType)
{
	if (eventType == cocos2d::ui::Widget::TouchEventType::BEGAN) {
		if (_isMoveable) {
			Message message("BeginMemberMove");
			message.variables.SetInt("Index", getTag());
			MessageManager::Instance().PutMessage(message);
			for (auto star : _stars) {
				cocos2d::Vec2 location = convertToNodeSpace(getTouchBeganPosition());
				if (star->getBoundingBox().containsPoint(location)) {
					star->setScale(0.9f);
				}
			}
		}
	} else {
		for (auto star : _stars) {
			star->setScale(1.0f);
		}
	}
}

std::string MemberWidget::GetIconForSpecial(const std::string &special)
{
	std::string filename;
	if (special == "thug") {
		filename = "ui/icon_spec_thug.png";
	} else if (special == "geek") {
		filename = "ui/icon_spec_geek.png";
	} else {
		Log::Instance().writeError("Unknown member special.");
	}
	return filename;
}