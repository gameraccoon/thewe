#ifndef MESSAGE_MANAGER_H
#define MESSAGE_MANAGER_H

#include <map>
#include <string>
#include "UserMessage.h"

class MessageManager
{
public:
	/**
	 * @return new exemplar of the class
	 */
	static MessageManager& Instance();

	/**
	 * Send new message to the player messages box
	 * @param text message text
	 */
	void SendGameMessage(std::string text);

	void RemoveMessage(int key);

	const std::map<int, UserMessage::Ptr> GetMessages() const;
private:
	std::map<int, UserMessage::Ptr> _messages;

	int _maxSetKey;

	/*
	 * Turn off unusable operations
	 */
	MessageManager();
	~MessageManager();
	MessageManager(const MessageManager&);
	void operator=(const MessageManager&);
};

#endif // MESSAGE_MANAGER_H
