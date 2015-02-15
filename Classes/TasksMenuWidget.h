#ifndef TASKS_MENU_WIDGET_H
#define TASKS_MENU_WIDGET_H

#include "Cell.h"
#include "GameInterface.h"
#include "MemberWidget.h"
#include "MemberMover.h"
#include "MembersPage.h"
#include "TasksListWidget.h"
#include "TaskStartButton.h"

class TasksMenuWidget : public cocos2d::Node
{
public:
	static TasksMenuWidget* create(Cell::WeakPtr cell);

	void Show(void);
	void Hide(void);

protected:
	TasksMenuWidget(void);
	~TasksMenuWidget(void);

	bool init(Cell::WeakPtr cell);
	void update(float dt) override;

	void OnClosePressed(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType eventType);
	void OnPageLeft(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType eventType);
	void OnPageRight(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType eventType);

private:
	void StartTask(void);

	Cell::WeakPtr _cell;
	cocos2d::ui::Widget *_widget;
	cocos2d::ui::Button *_btnScrollLeft;
	cocos2d::ui::Button *_btnScrollRight;
	TasksListWidget *_tasksList;
	TaskStartButton *_startButton;
	MembersPage *_membersPage;
	MembersSlot *_membersSlot;
	MemberMover *_mover;
};

#endif