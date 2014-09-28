#include "UserMessage.h"

UserMessage::UserMessage(std::string text)
	: _text(text)
	, _timeToShow(5.0f)
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
