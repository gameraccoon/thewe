#include "SessionEndScreen.h"

CustomSessionEndScreen::CustomSessionEndScreen(const ConstructionInfo &ci)
	: _info(ci)
{
	init();
}

bool CustomSessionEndScreen::init(void)
{
	if (!cocos2d::Layer::init())
	{
		return false;
	}

	cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();
	cocos2d::Vec2 size = cocos2d::Director::getInstance()->getVisibleSize();

	cocos2d::Sprite *background = cocos2d::Sprite::create("custom-white-menu.png");
	background->setPositionX(origin.x + size.x / 2.0f);
	background->setPositionY(origin.y + size.y / 2.0f);
	background->setColor(_info.backgrndColor);

	cocos2d::TTFConfig ttfconf = cocos2d::TTFConfig("arial.ttf", 32);

	cocos2d::Label *bigMessage = cocos2d::Label::createWithTTF(ttfconf, _info.bigMessage, cocos2d::TextHAlignment::CENTER);
	bigMessage->setPositionX(background->getContentSize().width / 2.0f);
	bigMessage->setPositionY(background->getContentSize().height - bigMessage->getContentSize().height / 2.0f - 10.0f);
	bigMessage->setColor(_info.messageColor);

	cocos2d::Label *shortDesc = cocos2d::Label::createWithTTF(ttfconf, _info.shortDesc, cocos2d::TextHAlignment::CENTER, 250);
	shortDesc->setScale(0.5f);
	shortDesc->setPositionX(background->getContentSize().width / 2.0f);
	shortDesc->setPositionY(background->getContentSize().height - shortDesc->getContentSize().height / 2.0f - 35.0f);
	shortDesc->setColor(_info.messageColor);

	cocos2d::MenuItemImage *button = cocos2d::MenuItemImage::create("new-cell-button-active.png", "new-cell-button-pressed.png",
			"new-cell-button-disabled.png", _info.callback);
	button->setScale(0.5f);
	button->setPositionX(background->getContentSize().width / 2.0f);
	button->setPositionY(button->getContentSize().height / 2.0f - 15.0f);

	cocos2d::Label *buttonText = cocos2d::Label::createWithTTF(ttfconf, "Back to Main Menu", cocos2d::TextHAlignment::CENTER);
	buttonText->setPositionX(button->getContentSize().width / 2.0f);
	buttonText->setPositionY(button->getContentSize().height / 2.0f);
	buttonText->setColor(cocos2d::Color3B(0, 0, 0));
	button->addChild(buttonText, 0);
	
	cocos2d::Menu *menu = cocos2d::Menu::create(button, nullptr);
	menu->setPosition(0.0f, 0.0f);

	ApplyPulsationAnimation(button, 0.2f, 0.2f);

	background->addChild(bigMessage, 0);
	background->addChild(shortDesc, 0);
	background->addChild(menu, 0);

	addChild(background);

	return true;
}

void CustomSessionEndScreen::ApplyPulsationAnimation(cocos2d::Node *drawable, float amplX, float amplY)
{
	if (!drawable)
	{
		return;
	}

	float initialScaleX = drawable->getScaleX();
	float initialScaleY = drawable->getScaleY();
	float finalScaleX = initialScaleX + initialScaleX * amplX;
	float finalScaleY = initialScaleY + initialScaleY * amplY;

	cocos2d::ScaleTo *scaleFirstOut = cocos2d::ScaleTo::create(0.5f, finalScaleX, finalScaleY, 1.0f);
	cocos2d::ScaleTo *scaleFirstIn = cocos2d::ScaleTo::create(0.4f, initialScaleX, initialScaleY, 1.0f);

	cocos2d::EaseCubicActionOut *scaleFirstOutEase = cocos2d::EaseCubicActionOut::create(scaleFirstOut);
	cocos2d::EaseCubicActionOut *scaleFirstInEase = cocos2d::EaseCubicActionOut::create(scaleFirstIn);
	
	cocos2d::Sequence *sequnce = cocos2d::Sequence::create(
		cocos2d::DelayTime::create(0.5f)
		,scaleFirstOutEase
		,scaleFirstInEase
		,nullptr
		);

	drawable->runAction(cocos2d::RepeatForever::create(sequnce));
}