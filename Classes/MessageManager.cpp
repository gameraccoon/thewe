#include "MessageManager.h"

#include <mutex>

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

static std::mutex InstanceMutex;
static std::recursive_mutex MessagesMutex;
static std::recursive_mutex ReceiversMutex;

MessageManager& MessageManager::Instance(void)
{
	std::lock_guard<std::mutex> lock(::InstanceMutex);
	static MessageManager instance;
	return instance;
}

void MessageManager::PutMessage(const Message &msg)
{
	std::lock_guard<std::recursive_mutex> lock(::MessagesMutex);
	_messages.push(msg);
}

void MessageManager::FlushMessages(void)
{
	std::lock_guard<std::recursive_mutex> lock(::MessagesMutex);
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

	std::lock_guard<std::recursive_mutex> lock(::ReceiversMutex);
	_receivers.insert(std::pair<const std::string, MessageReceiver*>(messageName, receiver));
}

void MessageManager::UnregisterReceiver(const MessageReceiver *receiver)
{
	std::lock_guard<std::recursive_mutex> lock(::ReceiversMutex);
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
	std::lock_guard<std::recursive_mutex> lock(::ReceiversMutex);
	auto receivers = _receivers.equal_range(messageName);

	std::list<Receivers::iterator> receiversToDelete;

	for (auto receiverIt = receivers.first, endRes = receivers.second; receiverIt != endRes; receiverIt++)
	{
		if (receiverIt->second == receiver)
		{
			receiversToDelete.push_back(receiverIt);
		}
	}

	for (Receivers::iterator& receiverToDelete : receiversToDelete)
	{
		_receivers.erase(receiverToDelete);
	}
}

void MessageManager::CallAcceptMessages(void)
{
	std::lock_guard<std::recursive_mutex> lock1(::MessagesMutex);
	std::lock_guard<std::recursive_mutex> lock2(::ReceiversMutex);

	while (!_messages.empty())
	{
		const Message &message = _messages.front();
		
		auto receivers = _receivers.equal_range(message.getName());

		for (Receivers::iterator& receiver = receivers.first, endRes = receivers.second; receiver != endRes; receiver++)
		{
			receiver->second->AcceptMessage(message);
		}
		
		_messages.pop();
	}
}
