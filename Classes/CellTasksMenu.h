#ifndef CELL_TASKS_MENU_H
#define CELL_TASKS_MENU_H

#include "CellGameInterface.h"

class CellTasksScreen : public cocos2d::Layer
{
public:
	CellTasksScreen(Cell::WeakPtr cell, CellMenuSelector *cellMenu);
	static CellTasksScreen* create(Cell::WeakPtr cell, CellMenuSelector *cellMenu);

	virtual bool init(void) override;

	void CloseMenu(void);
	
private:
	void OnCloseCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType eventType);
	void OnStartTaskCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType eventType);
	void KeyReleased(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event *event);
	cocos2d::ui::Widget* CreateScrollerItem(const Task::Info *info);

	cocos2d::ui::Layout *_widget;

	TaskManager::Tasks _avaliableTasks;
	Cell::WeakPtr _cell;
	CellMenuSelector *_cellMenu;
};

#endif // CELL_TASKS_MENU_H
