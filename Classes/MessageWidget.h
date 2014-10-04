#ifndef MESSAGE_WIDGET_H
#define MESSAGE_WIDGET_H

#include <cocos2d.h>
#include "UserMessage.h"

class MessageWidget : public cocos2d::Node
{
public:
	explicit MessageWidget(UserMessage::Ptr message);

	static MessageWidget* create(UserMessage::Ptr message);

	bool IsOutdated();

private:
	virtual bool init() override;

	virtual void update(float delta) override;

private:
	UserMessage::Ptr _message;

	float _timeToShow;
	float _showedTime;
	bool _isOutdated;
};
#endif // MESSAGE_WIDGET_H
