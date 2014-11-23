#include "NotificationMessageWidget.h"

#include "Vector2.h"
#include "Log.h"

NotificationMessageWidget::NotificationMessageWidget(UserMessage::Ptr message)
	: _message(message)
	, _timeToShow(message->GetTimeToShow())
	, _showedTime(0.0f)
	, _isOutdated(false)
{
}

NotificationMessageWidget* NotificationMessageWidget::create(UserMessage::Ptr message)
{
	NotificationMessageWidget* ret = new NotificationMessageWidget(message);
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

bool NotificationMessageWidget::init()
{
	if (!cocos2d::Node::init())
	{
		return false;
	}

	cocos2d::ui::Widget *widget = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("ui_hud/ui_message.ExportJson");
	widget->setAnchorPoint(cocos2d::Vec2(1.0f, 1.0f));
	widget->setPosition(cocos2d::Vec2(0.0f, 0.0f));

	if (_message)
	{
		cocos2d::ui::Text *text = dynamic_cast<cocos2d::ui::Text *>(widget->getChildByName("Text"));

		if (!text)
		{
			Log::Instance().writeError("Failed to get element with name Text from ui_message widget.");
			return false;
		}

		text->setString(_message->GetText());
	}

	addChild(widget);
	setContentSize(widget->getContentSize());
	scheduleUpdate();

	return true;
}

void NotificationMessageWidget::update(float delta)
{
	_showedTime += delta;

	if (!_isOutdated && _showedTime > _timeToShow)
	{
		_isOutdated = true;
	}
}

bool NotificationMessageWidget::IsOutdated()
{
	return _isOutdated;
}
