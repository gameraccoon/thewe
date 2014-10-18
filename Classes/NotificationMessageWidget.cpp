#include "NotificationMessageWidget.h"

#include "Vector2.h"

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

	cocos2d::Sprite *background = cocos2d::Sprite::create("message_background.png");
	background->setPosition(0.0f, 0.0f);
	background->setAnchorPoint(cocos2d::Vec2(1.0f, 1.0f));
	addChild(background);

	if (_message)
	{
		cocos2d::TTFConfig ttfConfig("arial.ttf", 18);
		cocos2d::Label *text = cocos2d::Label::createWithTTF(ttfConfig, _message->GetText(), cocos2d::TextHAlignment::CENTER);
		cocos2d::Rect rect = background->getBoundingBox();
		Vector2 center = Vector2(rect.getMinX(), rect.getMinY()) / 2;
		text->setPosition(center);
		addChild(text);
	}

	setContentSize(background->getContentSize());

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
