#ifndef TASKS_LIST_WIDGET_H
#define TASKS_LIST_WIDGET_H

#include "GameInterface.h"
#include "Cell.h"
#include "TaskManager.h"

class TasksListItem : public cocos2d::ui::Button
{
public:
	static TasksListItem* create(void);

protected:
	TasksListItem(void);
	~TasksListItem(void);

	bool init(void) override;

	void OnPress(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType eventType);

private:
	bool _state;
};

class TasksListWidget : public cocos2d::ui::ListView
{
public:
	static TasksListWidget* create(Cell::WeakPtr cell);
	static const float VISIBLE_AREA_WIDTH;
	static const float VISIBLE_AREA_HEIGHT;

	void FillList(const TaskManager::Tasks &tasks);

protected:
	TasksListWidget(Cell::WeakPtr cell);
	virtual ~TasksListWidget(void);

	bool init(void) override;
	void update(float dt) override;

private:
	Cell::WeakPtr _cell;
};

#endif