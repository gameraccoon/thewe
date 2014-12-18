#ifndef MESSAGE_MANAGER_H
#define MESSAGE_MANAGER_H

#include "MiscUtils.h"

struct Message
{
private:
	const std::string name;
public:
	Utils::VariablesSet variables;
public:
	Message(const std::string &n)
		: name(n)
	{}
	bool is(const std::string &n) const {
		return name == n;
	}
};

class MessageReceiver
{
public:
	MessageReceiver();
	virtual ~MessageReceiver(void);

	virtual void AcceptMessage(const Message &msg) = 0;
};

class MessageManager
{
public:
	static MessageManager& Instance(void);

	void PutMessage(const Message &msg);
	void FlushMessages(void);

	void RegisterReceiver(MessageReceiver *receiver);
	void UnregisterReceiver(MessageReceiver *receiver);

	/**
	 * Send all scheduled messages to the receivers
	 */
	void CallAcceptMessages(void);

private:
	std::list<MessageReceiver *> _receivers;
	std::queue<Message> _messages;

private:
	~MessageManager(void);
	MessageManager(void);
	MessageManager(const MessageManager &);
	void operator=(const MessageManager &);
};

#endif
