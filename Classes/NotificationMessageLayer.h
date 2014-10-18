#ifndef NOTIFICATION_MESSAGE_LAYER
#define NOTIFICATION_MESSAGE_LAYER

#include <cocos2d.h>
#include "NotificationMessageWidget.h"
#include "Vector2.h"

class NotificationMessageLayer : public cocos2d::Layer
{
public:
	NotificationMessageLayer();
	static NotificationMessageLayer* create();

	virtual bool init(void) override;
	virtual void update(float delta);

private:
	int UpdateMessages();
	void UpdateMessagesPos();
	void AddNewMessage(int key, const std::map<int, UserMessage::Ptr>& messages);

private:
	std::map<int, NotificationMessageWidget*> _userMessages;

	Vector2 _messagesMargin;
	Vector2 _messagesPosition;
};

#endif // NOTIFICATION_MESSAGE_LAYER
