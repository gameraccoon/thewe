#ifndef CELL_TASKS_MENU_H
#define CELL_TASKS_MENU_H

#include "CellGameInterface.h"

class CellTasksScreen : public cocos2d::Layer
{
public:
	CellTasksScreen(Cell::WeakPtr cell, CellMenuSelector *cellMenu);

	virtual bool init(void) override;

	void CloseMenu(void);
	
private:
	void _OnCloseCallback(cocos2d::Ref *sender);
	void _CreateTasksScrollViewMenu(const TaskManager::Tasks &tasks, const cocos2d::Vec2 &pos, const cocos2d::Size &size);
	void KeyReleased(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event *event);

	Cell::WeakPtr _cell;
	CellMenuSelector *_cellMenu;
	cocos2d::extension::ScrollView *_scrollTasksView;
};

#endif // CELL_TASKS_MENU_H
