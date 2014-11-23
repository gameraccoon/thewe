#ifndef CELL_TASK_INFO_MENU
#define CELL_TASK_INFO_MENU

#include "CellGameInterface.h"

class CellTaskInfoMenu : public cocos2d::Layer
{
public:
	CellTaskInfoMenu(Cell::WeakPtr cell, CellMenuSelector *selector);

	virtual bool init(void) override;
	virtual void update(float dt) override;

private:
	void CloseMenu(void);
	void KeyReleased(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event *event);
	void OnCloseCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType eventType);
	void OnAbortCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType eventType);

private:
	cocos2d::ui::Layout *_widget;
	cocos2d::ui::LoadingBar *_progressBar;
	cocos2d::ui::TextBMFont *_textPercentage;

	Cell::WeakPtr _cell;
	Task::WeakPtr _cellCurrentTask;
	CellMenuSelector *_cellMenuSelector;
};

#endif // CELL_TASK_INFO_MENU
