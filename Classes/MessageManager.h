#ifndef MESSAGE_MANAGER_H
#define MESSAGE_MANAGER_H

#include <map>
#include <string>
#include "UserMessage.h"

class MessageManager
{
public:
	/**
	 * @return экземпляр класса
	 */
	static MessageManager& Instance();

	/**
	 * Отправить новое сообщение в мессенджер игрока
	 * @param message сообщение
	 */
	void SendMessage(UserMessage::Ptr message);
	void SendMessage(std::string text, float timeToShow = 5.0);

	void RemoveMessage(int key);

	const std::map<int, UserMessage::Ptr> GetMessages() const;
private:
	std::map<int, UserMessage::Ptr> _messages;

	int _maxSetKey;

	/*
	 * Отключаем ненужные операции
	 */
	MessageManager();
	~MessageManager();
	MessageManager(const MessageManager&);
	void operator=(const MessageManager&);
};

#endif // MESSAGE_MANAGER_H
