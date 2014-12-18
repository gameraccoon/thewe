#include "MessageManager.h"

#include "Log.h"

MessageReceiver::MessageReceiver()
{
	MessageManager::Instance().RegisterReceiver(this);
}

MessageReceiver::~MessageReceiver()
{
	MessageManager::Instance().UnregisterReceiver(this);
}

MessageManager::MessageManager(void)
{
}

MessageManager::~MessageManager(void)
{
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

void MessageManager::RegisterReceiver(MessageReceiver *receiver)
{
	if (!receiver)
	{
		Log::Instance().writeError("Trying to add nullptr message receiver.");
		return;
	}

	for (MessageReceiver *rec : _receivers) {
		if (rec == receiver) {
			Log::Instance().writeWarning("Trying to put existed message.");
			return;
		}
	}
	_receivers.push_back(receiver);
}

void MessageManager::UnregisterReceiver(MessageReceiver *receiver)
{
	for (std::list<MessageReceiver *>::iterator iter = _receivers.begin(); iter != _receivers.end(); ++iter)
	{
		if ((*iter) == receiver)
		{
			iter = _receivers.erase(iter);
			break;
		}
	}
}

void MessageManager::CallAcceptMessages(void)
{
	while (!_messages.empty())
	{
		Message &message = _messages.front();
		
		for (MessageReceiver *rec : _receivers) {
			rec->AcceptMessage(message);
		}
		
		_messages.pop();
	}
}
