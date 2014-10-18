#include "NotificationMessageLayer.h"

#include <math.h>

#include "World.h"
#include "GameScene.h"
#include "MessageManager.h"

NotificationMessageLayer::NotificationMessageLayer()
{
}

bool NotificationMessageLayer::init(void)
{
	if (!cocos2d::Layer::init())
	{
		return false;
	}

	cocos2d::Director *director = cocos2d::Director::getInstance();
	Vector2 screen = director->getVisibleSize();
	Vector2 origin = director->getVisibleOrigin();
	_messagesMargin = Vector2(5.0f, 5.0f);
	_messagesPosition = Vector2(origin + screen - _messagesMargin);

	setTouchEnabled(true);
    setKeypadEnabled(true);

	scheduleUpdate();

	return true;
}

NotificationMessageLayer* NotificationMessageLayer::create()
{
	NotificationMessageLayer* ret = new NotificationMessageLayer();
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

void NotificationMessageLayer::update(float delta)
{
	int changed = UpdateMessages();
	if (changed > 0)
	{
		UpdateMessagesPos();
	}
}

int NotificationMessageLayer::UpdateMessages()
{
	std::set<int> visibleKeys;
	for (auto message : _userMessages)
	{
		int key = message.first;

		visibleKeys.insert(key);

		if (message.second->IsOutdated())
		{
			World::Instance().GetMessageManager().RemoveMessage(key);
		}
	}

	const std::map<int, UserMessage::Ptr> messages = World::Instance().GetMessageManager().GetMessages();
	std::set<int> realKeys;
	for (auto message : messages)
	{
		realKeys.insert(message.first);
	}

	int changed = 0;
	std::set<int> newKeys;
	set_difference(realKeys.begin(), realKeys.end(),
				   visibleKeys.begin(), visibleKeys.end(),
				   inserter(newKeys, newKeys.begin()));

	if (newKeys.size() > 0)
	{
		for (int key : newKeys)
		{
			AddNewMessage(key, messages);
			changed++;
		}
	}

	std::set<int> keysToRemove;
	set_difference(visibleKeys.begin(), visibleKeys.end(),
				   realKeys.begin(), realKeys.end(),
				   inserter(keysToRemove, keysToRemove.begin()));

	if (keysToRemove.size() > 0)
	{
		for (int key : keysToRemove)
		{
			removeChild(_userMessages.at(key));
			_userMessages.erase(key);

			changed++;
		}
	}

	return changed;
}

void NotificationMessageLayer::UpdateMessagesPos()
{
	Vector2 position = _messagesPosition;

	for (auto message : _userMessages)
	{
		cocos2d::MoveTo *move = cocos2d::MoveTo::create(0.3f, position);
		message.second->stopAllActions();
		message.second->runAction(move);
		position.y += -message.second->getContentSize().height - _messagesMargin.y;
	}
}

void NotificationMessageLayer::AddNewMessage(int key, const std::map<int, UserMessage::Ptr>& messages)
{
	UserMessage::Ptr message = messages.at(key);
	NotificationMessageWidget* messageWidget = NotificationMessageWidget::create(message);

	Vector2 position = _messagesPosition;

	position.y += _userMessages.size() * (-messageWidget->getContentSize().height - _messagesMargin.y);

	messageWidget->setPosition(position);
	addChild(messageWidget);
	_userMessages.insert(std::pair<int, NotificationMessageWidget*>(key, messageWidget));
}
