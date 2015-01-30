#ifndef TASKS_MENU_WIDGET_H
#define TASKS_MENU_WIDGET_H

#include "Cell.h"
#include "GameInterface.h"

class TasksMenuWidget : public cocos2d::Node, public MessageReceiver
{
public:
	static TasksMenuWidget* create(void);

	void AcceptMessage(const Message &message);

	void Show(void);
	void Hide(void);

protected:
	TasksMenuWidget(void);
	~TasksMenuWidget(void);

	bool init(void) override;

	void OnClosePressed(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType eventType);

private:
	Cell::WeakPtr _cell;
	cocos2d::ui::Widget *_widget;
};

#endif