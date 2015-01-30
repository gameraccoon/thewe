#include "MessageManager.h"

#include "Log.h"

MessageManager::MessageManager(void)
{
}

MessageManager::~MessageManager(void)
{
}

MessageReceiver::~MessageReceiver()
{
	MessageManager::Instance().UnregisterReceiver(this);
}

MessageManager& MessageManager::Instance(void)
{
	static MessageManager instance;
	return instance;
}

void MessageManager::PutMessage(const Message &msg)
{
	_messages.push(msg);
}

void MessageManager::FlushMessages(void)
{
	while (!_messages.empty()) {
		_messages.pop();
	}
}

void MessageManager::RegisterReceiver(MessageReceiver *receiver, const std::string& messageName)
{
	if (!receiver)
	{
		WRITE_ERR("Trying to add nullptr as message receiver.");
		return;
	}

	_receivers.insert(std::pair<const std::string, MessageReceiver*>(messageName, receiver));
}

void MessageManager::UnregisterReceiver(const MessageReceiver *receiver)
{
	std::list<Receivers::iterator> receiversToDelete;

	for (auto receiverIt = _receivers.begin(), endRes = _receivers.end(); receiverIt != endRes; receiverIt++)
	{
		if (receiverIt->second == receiver)
		{
			receiversToDelete.push_back(receiverIt);
		}
	}

	for (Receivers::iterator& receiverTodelete : receiversToDelete)
	{
		_receivers.erase(receiverTodelete);
	}
}

void MessageManager::UnregisterReceiver(const MessageReceiver *receiver, const std::string& messageName)
{
	auto receivers = _receivers.equal_range(messageName);

	std::list<Receivers::iterator> receiversToDelete;

	for (auto receiverIt = receivers.first, endRes = receivers.second; receiverIt != endRes; receiverIt++)
	{
		if (receiverIt->second == receiver)
		{
			receiversToDelete.push_back(receiverIt);
		}
	}

	for (Receivers::iterator& receiverTodelete : receiversToDelete)
	{
		_receivers.erase(receiverTodelete);
	}
}

void MessageManager::CallAcceptMessages(void)
{
	while (!_messages.empty())
	{
		Message &message = _messages.front();
		
		auto receivers = _receivers.equal_range(message.getName());

		for (Receivers::iterator& receiver = receivers.first, endRes = receivers.second; receiver != endRes; receiver++)
		{
			receiver->second->AcceptMessage(message);
		}
		
		_messages.pop();
	}
}
