#include "TutorialWidget.h"

#include "Vector2.h"

TutorialWidget::TutorialWidget(std::string text)
	: _text(text)
{
}

TutorialWidget* TutorialWidget::create(std::string text)
{
	TutorialWidget* ret = new TutorialWidget(text);
	if (ret && ret->init())
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

bool TutorialWidget::init()
{
	if (!cocos2d::Node::init())
	{
		return false;
	}

	cocos2d::Sprite *background = cocos2d::Sprite::create("message_background.png");
	background->setPosition(0.0f, 0.0f);
	background->setAnchorPoint(cocos2d::Vec2(1.0f, 1.0f));
	addChild(background);

	cocos2d::TTFConfig ttfConfig("arial.ttf", 18);
	cocos2d::Label *text = cocos2d::Label::createWithTTF(ttfConfig, _text, cocos2d::TextHAlignment::CENTER);
	cocos2d::Rect rect = background->getBoundingBox();
	Vector2 center = Vector2(rect.getMinX(), rect.getMinY()) / 2;
	text->setPosition(center);
	addChild(text);

	setContentSize(background->getContentSize());

	scheduleUpdate();

	return true;
}
