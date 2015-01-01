#include "TutorialWidgetFirstCell.h"

#include <luabind/luabind.hpp>

#include "Vector2.h"
#include "Color.h"
#include "World.h"
#include "Log.h"

TutorialWidgetFirstCell::TutorialWidgetFirstCell(Tutorial::WeakPtr tutorial)
	: TutorialWidget(tutorial)
{
}

TutorialWidgetFirstCell* TutorialWidgetFirstCell::create(Tutorial::WeakPtr tutorial)
{
	TutorialWidgetFirstCell* ret = new TutorialWidgetFirstCell(tutorial);
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

bool TutorialWidgetFirstCell::init()
{
	if (!cocos2d::Node::init())
	{
		return false;
	}

	Tutorial::Ptr tutorial = _tutorial.lock();
	if (!tutorial)
	{
		Log::Instance().writeWarning("Tutorial was removed before use");
		return true;
	}

	cocos2d::ui::Widget *widget = cocostudio::GUIReader::getInstance()->widgetFromJsonFile(
				std::string("tutorial/").append(tutorial->widgetName).append(".ExportJson").c_str());
	widget->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
	widget->setPosition(cocos2d::Vec2(0.0f, 0.0f));

	cocos2d::ui::Button *btnContinue = dynamic_cast<cocos2d::ui::Button *>(widget->getChildByName("ContinueBtn"));
	cocos2d::ui::Text *tutorialText = dynamic_cast<cocos2d::ui::Text *>(widget->getChildByName("TutorialText"));

	if (btnContinue)
	{
		btnContinue->addTouchEventListener(CC_CALLBACK_2(TutorialWidgetFirstCell::OnContinueCallback, this));
		btnContinue->setTitleText(_tutorial.lock()->buttonText);
	}

	if (tutorialText)
	{
		tutorialText->setString(_tutorial.lock()->text);
	}

	cocos2d::Sprite *spot1 = cocos2d::Sprite::create("tutorial_spot.png");
	spot1->setPosition(cocos2d::Director::getInstance()->getVisibleSize() * 0.5f);
	spot1->setScale(3.0f);
	spot1->retain();

	cocos2d::Sprite *spot2 = cocos2d::Sprite::create("tutorial_spot.png");
	spot2->setPosition(0.0f, 0.0f);
	spot2->setAnchorPoint(cocos2d::Vec2(0.0f, 0.0f));
	spot2->setScale(1.5f);
	spot2->retain();

	ScreenBlackoutWidget *blackout = ScreenBlackoutWidget::create(cocos2d::Color4F(0.0f, 0.0f, 0.0f, 0.9f));
	blackout->AddSpot(spot1);
	blackout->AddSpot(spot2);

	addChild(widget, 0);
	addChild(blackout, 1);

	return true;
}