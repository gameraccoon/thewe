#include "TutorialWidget.h"

#include "Vector2.h"
#include "Color.h"

TutorialWidget::TutorialWidget(Tutorial tutorial)
	: _tutorial(tutorial)
	, _isReadyToClose(false)
{
}

TutorialWidget* TutorialWidget::create(Tutorial tutorial)
{
	TutorialWidget* ret = new TutorialWidget(tutorial);
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

	cocos2d::Sprite *background = cocos2d::Sprite::create("custom-white-menu.png");
	background->setPosition(0.0f, 0.0f);
	background->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
	background->setColor(Color(0x000000));
	addChild(background);

	cocos2d::TTFConfig ttfConfig("arial.ttf", 18);
	cocos2d::Label *text = cocos2d::Label::createWithTTF(ttfConfig, _tutorial.text, cocos2d::TextHAlignment::CENTER);
	Vector2 center = Vector2(0.0f, 15.0f);
	text->setPosition(center);
	addChild(text);

	setContentSize(background->getContentSize());

	cocos2d::MenuItemImage *closeButton;
	{
		using namespace cocos2d;
		closeButton = MenuItemImage::create("new-cell-button-active.png", "new-cell-button-pressed.png",
			"new-cell-button-disabled.png", CC_CALLBACK_1(TutorialWidget::_OnCloseCallback, this));
		closeButton->setPosition(center * 2.0 + Vector2(0.0f, -getContentSize().height / 2));
		closeButton->setScale(0.3f);
	}

	cocos2d::Menu *menu = cocos2d::Menu::create(closeButton, nullptr);
	menu->setPosition(0.0, 0.0f);
	addChild(menu);

	cocos2d::TTFConfig ttfBtnConfig("arial.ttf", 12);
	cocos2d::Label *buttonLabel = cocos2d::Label::createWithTTF(ttfBtnConfig, _tutorial.buttonText, cocos2d::TextHAlignment::CENTER);
	buttonLabel->setPosition(closeButton->getPosition());
	buttonLabel->setScale(1.2f);
	addChild(buttonLabel);

	scheduleUpdate();

	return true;
}

bool TutorialWidget::IsReadyToClose() const
{
	return _isReadyToClose;
}

void TutorialWidget::_OnCloseCallback(cocos2d::Ref *sender)
{
	_isReadyToClose = true;
}
