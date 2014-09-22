#include "MessageManager.h"

MessageManager::MessageManager()
	: _maxSetKey(0)
{
}

MessageManager::~MessageManager()
{
}

MessageManager& MessageManager::Instance()
{
	static MessageManager singleInstance;
	return singleInstance;
}

void MessageManager::SendGameMessage(UserMessage::Ptr message)
{
	_messages.insert(std::pair<int, UserMessage::Ptr>(++_maxSetKey, message));
}

void MessageManager::SendGameMessage(std::string text, float timeToShow)
{
	_messages.insert(std::pair<int, UserMessage::Ptr>(++_maxSetKey
		,std::make_shared<UserMessage>(UserMessage(text, timeToShow))));
}

const std::map<int, UserMessage::Ptr> MessageManager::GetMessages() const
{
	return _messages;
}

void MessageManager::RemoveMessage(int key)
{
	_messages.erase(key);
}
