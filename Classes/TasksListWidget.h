#ifndef TASKS_LIST_WIDGET_H
#define TASKS_LIST_WIDGET_H

#include "GameInterface.h"
#include "Cell.h"
#include "TaskManager.h"

class TasksListItem : public cocos2d::ui::Button
{
public:
	static TasksListItem* create(void);
	
	void ReleaseToggle(void);
	bool IsPressed(void) const;

protected:
	TasksListItem(void);
	~TasksListItem(void);

	bool init(void) override;
	void OnPress(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType eventType);

private:
	void UpdateTexture(void);
	bool _state;
};

class TasksListWidget : public cocos2d::ui::ListView, public MessageReceiver
{
public:
	static TasksListWidget* create(Cell::WeakPtr cell);
	static const float VISIBLE_AREA_WIDTH;
	static const float VISIBLE_AREA_HEIGHT;

	void FillList(const TaskManager::Tasks &tasks);
	void AcceptMessage(const Message &message) override;

protected:
	TasksListWidget(Cell::WeakPtr cell);
	virtual ~TasksListWidget(void);

	bool init(void) override;
	void update(float dt) override;

private:
	Cell::WeakPtr _cell;
};

#endif