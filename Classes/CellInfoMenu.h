#ifndef CELL_INFO_MENU_H
#define CELL_INFO_MENU_H

#include "CellGameInterface.h"

class CellInfoMenu : public cocos2d::Layer
{
public:
	CellInfoMenu(Cell::WeakPtr cell, CellMenuSelector *selector);

	virtual bool init(void) override;
	virtual void update(float dt) override;

	void UpdateInfoBy(Cell::Ptr cell);

private:
	void OnCloseCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType eventType);
	void CloseMenu();
	void KeyReleased(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event *event);

private:
	cocos2d::ui::Layout *_widget;

	cocos2d::ui::TextBMFont *_txtMembers;
	cocos2d::ui::TextBMFont *_txtCach;
	cocos2d::ui::TextBMFont *_txtPursuedLevel;
	cocos2d::ui::TextBMFont *_txtMorale;
	cocos2d::ui::TextBMFont *_txtDevotion;
	cocos2d::ui::TextBMFont *_txtLevel;

	cocos2d::ui::LoadingBar  *_levelProgressBar;
	cocos2d::ui::LoadingBar  *_moraleProgressBar;
	cocos2d::ui::LoadingBar  *_devotionProgressBar;

	Cell::WeakPtr _cell;
	Task::WeakPtr _cellCurrentTask;
	CellMenuSelector *_cellMenuSelector;
};

#endif // CELL_INFO_MENU_H
