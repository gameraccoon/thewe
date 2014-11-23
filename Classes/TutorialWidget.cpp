#include "TutorialWidget.h"

#include <luabind/luabind.hpp>

#include "Vector2.h"
#include "Color.h"
#include "World.h"
#include "Log.h"

TutorialWidget::TutorialWidget(Tutorial::WeakPtr tutorial)
	: _tutorial(tutorial)
{
}

TutorialWidget* TutorialWidget::create(Tutorial::WeakPtr tutorial)
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

	Tutorial::Ptr tutorial = _tutorial.lock();
	if (!tutorial)
	{
		Log::Instance().writeWarning("Tutorial was removed before use");
		return true;
	}

	cocos2d::ui::Widget *widget = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("tutorial/tutorial.ExportJson");
	widget->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
	widget->setPosition(cocos2d::Vec2(0.0f, 0.0f));

	cocos2d::ui::Button *btnContinue = dynamic_cast<cocos2d::ui::Button *>(widget->getChildByName("ContinueBtn"));
	cocos2d::ui::Text *tutorialText = dynamic_cast<cocos2d::ui::Text *>(widget->getChildByName("Text"));
	
	if (!btnContinue) {Log::Instance().writeWarning("Failed to get element with name ContinueBtn from tutorial widget."); return false;}
	if (!tutorialText) {Log::Instance().writeWarning("Failed to get element with name Text from tutorial widget."); return false;}
	
	btnContinue->addTouchEventListener(CC_CALLBACK_2(TutorialWidget::OnContinueCallback, this));
	btnContinue->setTitleText(_tutorial.lock()->buttonText);
	tutorialText->setString(_tutorial.lock()->text);

	addChild(widget);
	scheduleUpdate();

	return true;
}

bool TutorialWidget::IsReadyToClose() const
{
	return _tutorial.expired();
}

void TutorialWidget::OnContinueCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType eventType)
{
	if (eventType == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		World::Instance().RemoveCurrentTutorial();
	}
}
