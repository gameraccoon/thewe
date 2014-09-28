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

void MessageManager::SendGameMessage(std::string text)
{
	_messages.insert(std::pair<int, UserMessage::Ptr>(++_maxSetKey
		,std::make_shared<UserMessage>(text)));
}

const std::map<int, UserMessage::Ptr> MessageManager::GetMessages() const
{
	return _messages;
}

void MessageManager::RemoveMessage(int key)
{
	_messages.erase(key);
}
