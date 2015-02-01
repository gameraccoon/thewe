#include "MemberWidget.h"

MemberWidget* MemberWidget::create(void)
{
	MemberWidget *ret = new MemberWidget();
	if (ret && ret->init())
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

bool MemberWidget::init(void)
{
	if (!cocos2d::ui::Layout::init()) {
		return false;
	}
	
	_specialIcon = cocos2d::Sprite::create("ui/icon_spec_inform.png");
	_specialIcon->setPosition(80.0f, 95.0f);
	
	_bgRect = cocos2d::Sprite::create("ui/human_bg.png");
	_bgRect->setPosition((cocos2d::Vec2)_bgRect->getContentSize() * 0.5f);

	float star_x = 13.0f;
	for (int i = 0; i < 5; ++i) {
		cocos2d::Sprite *star;
		star = cocos2d::Sprite::create("ui/star_1.png");
		star->setPosition(star_x, 16.0f);
		addChild(star, 1);
		_stars.push_back(star);
		star_x += 24.0f;
	}

	setAnchorPoint(cocos2d::Vec2(0.0f, 0.0f));
	setContentSize(_bgRect->getContentSize());
	setTouchEnabled(true);
	addTouchEventListener(CC_CALLBACK_2(MemberWidget::TouchListener, this));
	addChild(_bgRect, 0);
	addChild(_specialIcon, 2);

	return true;
}

void MemberWidget::TouchListener(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType eventType)
{
	if (eventType == cocos2d::ui::Widget::TouchEventType::BEGAN) {
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
