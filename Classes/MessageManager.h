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

	inline bool is(const std::string &n) const {
		return name == n;
	}

	inline const std::string& getName() const
	{
		return name;
	}
};

class MessageReceiver
{
public:
	virtual ~MessageReceiver();
	virtual void AcceptMessage(const Message &msg) = 0;
};

class MessageManager
{
private:
	typedef std::multimap<const std::string, MessageReceiver *> Receivers;
public:
	static MessageManager& Instance(void);

	void PutMessage(const Message &msg);
	void FlushMessages(void);

	void RegisterReceiver(MessageReceiver *receiver, const std::string& messageName);
	void UnregisterReceiver(const MessageReceiver *receiver);
	void UnregisterReceiver(const MessageReceiver *receiver, const std::string& messageName);

	/**
	 * Send all scheduled messages to the receivers
	 */
	void CallAcceptMessages(void);

private:
	Receivers _receivers;
	std::queue<Message> _messages;

private:
	~MessageManager(void);
	MessageManager(void);
	MessageManager(const MessageManager &);
	void operator=(const MessageManager &);
};

#endif
