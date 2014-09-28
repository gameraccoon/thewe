#ifndef USER_MESSAGE_H
#define USER_MESSAGE_H

#include <memory>
#include <string>

class UserMessage
{
public:
	typedef std::shared_ptr<UserMessage> Ptr;
	typedef std::weak_ptr<UserMessage> WeakPtr;

	enum class Type
	{
		Notification
		,Primary
		,Important
	};

public:
	UserMessage(std::string text);

	const std::string GetText() const;
	const bool GetIsReaden() const;
	const float GetTimeToShow() const;

private:
	std::string _text;
	bool _isReaden;
	float _timeToShow;
};

#endif // USER_MESSAGE_H
