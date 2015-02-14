#include "MemberWidget.h"

MemberWidget* MemberWidget::createWithMember(void)
{
	MemberWidget *ret = new MemberWidget();
	if (ret && ret->initWithMember())
	{
		ret->autorelease();
		return ret;
	}

	delete ret;
	return nullptr;
}

MemberWidget* MemberWidget::createEmpty(bool drawSpecial)
{
	MemberWidget *ret = new MemberWidget();
	if (ret && ret->initEmpty(drawSpecial))
	{
		ret->autorelease();
		return ret;
	}

	delete ret;
	return nullptr;
}

MemberWidget::MemberWidget(void)
{
}

MemberWidget::~MemberWidget(void)
{
}

bool MemberWidget::initWithMember(void)
{
	if (!cocos2d::ui::Layout::init()) {
		return false;
	}

	_isEmptyMemberWidget = false;
	
	_special = cocos2d::Sprite::create("ui/icon_spec_inform.png");
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

	addChild(_background, 0);
	addChild(_face, 1);
	addChild(_special, 2);

	setAnchorPoint(cocos2d::Vec2(0.0f, 0.0f));
	setContentSize(_background->getContentSize());
	addTouchEventListener(CC_CALLBACK_2(MemberWidget::TouchListener, this));
	setTouchEnabled(true);

	return true;
}

bool MemberWidget::initEmpty(bool drawSpecial)
{
	if (!cocos2d::ui::Layout::init()) {
		return false;
	}

	_isEmptyMemberWidget = true;

	_background = cocos2d::Sprite::create("ui/human_slot.png");
	_background->setPosition((cocos2d::Vec2)_background->getContentSize() * 0.5f);

	_face = cocos2d::Sprite::create("ui/human_face.png");
	_face->setPosition((cocos2d::Vec2)_face->getContentSize()*0.5f);
	_face->setOpacity(128);

	if (drawSpecial) {
		_special = cocos2d::Sprite::create("ui/icon_spec_inform.png");
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

bool MemberWidget::IsEmptyMemberWidget(void) const
{
	return _isEmptyMemberWidget;
}

void MemberWidget::TouchListener(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType eventType)
{
	if (eventType == cocos2d::ui::Widget::TouchEventType::BEGAN) {
		Message message("BeginMemberMove");
		message.variables.SetInt("Index", getTag());
		MessageManager::Instance().PutMessage(message);
		for (auto star : _stars) {
			cocos2d::Vec2 location = convertToNodeSpace(getTouchBeganPosition());
			if (star->getBoundingBox().containsPoint(location)) {
				star->setScale(0.9f);
			}
		}
	} else {
		for (auto star : _stars) {
			star->setScale(1.0f);
		}
	}
}
