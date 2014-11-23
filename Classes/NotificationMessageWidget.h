#ifndef NOTIFICATION_MESSAGE_WIDGET_H
#define NOTIFICATION_MESSAGE_WIDGET_H

#include <cocos2d.h>
#include "UserMessage.h"

class NotificationMessageWidget : public cocos2d::Node
{
public:
	explicit NotificationMessageWidget(UserMessage::Ptr message);

	static NotificationMessageWidget* create(UserMessage::Ptr message);

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
#endif // NOTIFICATION_MESSAGE_WIDGET_H
