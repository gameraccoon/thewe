#include "UserMessage.h"

UserMessage::UserMessage(std::string text, float timeToShow)
	: _text(text)
	, _timeToShow(timeToShow)
{
}

const std::string UserMessage::GetText() const
{
	return _text;
}

const bool UserMessage::GetIsReaden() const
{
	return _isReaden;
}

const float UserMessage::GetTimeToShow() const
{
	return _timeToShow;
}
