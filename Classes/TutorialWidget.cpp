#include "TutorialWidget.h"

#include "Vector2.h"
/*
TutorialWidget::TutorialWidget(UserMessage::Ptr message)
	: _message(message)
	, _timeToShow(message->GetTimeToShow())
	, _showedTime(0.0f)
	, _isOutdated(false)
{
}

TutorialWidget* TutorialWidget::create(UserMessage::Ptr message)
{
	TutorialWidget* ret = new TutorialWidget(message);
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

void TutorialWidget::update(float delta)
{
	_showedTime += delta;

	if (!_isOutdated && _showedTime > _timeToShow)
	{
		_isOutdated = true;
	}
}

bool TutorialWidget::IsOutdated()
{
	return _isOutdated;
}
*/
