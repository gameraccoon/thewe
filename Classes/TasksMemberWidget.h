#ifndef TASKS_HUMAN_TASK_WIDGET_H
#define TASKS_HUMAN_TASK_WIDGET_H

#include "GameInterface.h"

class TasksMemberWidget : public cocos2d::ui::Layout
{
public:
	static TasksMemberWidget* create(void);

	void TouchListener(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType eventType);

protected:
	TasksMemberWidget(void);
	virtual ~TasksMemberWidget(void);

	bool init(void) override;

private:
	typedef std::vector<cocos2d::Sprite *> Stars;

private:
	cocos2d::Sprite *_bgRect;
	cocos2d::Sprite *_specialIcon;
	Stars _stars;
};

#endif
