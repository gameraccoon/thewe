#include "NotificationMessageManager.h"

NotificationMessageManager::NotificationMessageManager()
	: _maxSetKey(0)
{
}

void NotificationMessageManager::SendGameMessage(std::string text)
{
	_messages.insert(std::pair<int, UserMessage::Ptr>(++_maxSetKey
		,std::make_shared<UserMessage>(text)));
}

const std::map<int, UserMessage::Ptr> NotificationMessageManager::GetMessages() const
{
	return _messages;
}

void NotificationMessageManager::RemoveMessage(int key)
{
	_messages.erase(key);
}
