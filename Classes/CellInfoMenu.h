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
	void _OnCloseCallback(cocos2d::Ref *sender);
	void _CloseMenu();
	void KeyReleased(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event *event);

private:
	SquareProgressBar *_taskProgressBar;
	SquareProgressBar *_levelProgressBar;
	SquareProgressBar *_moraleProgressBar;
	SquareProgressBar *_devotionProgressBar;
	SquareProgressBar *_townInfluenceProgressBar;

	cocos2d::Label *_labelLevelInfo;
	cocos2d::Label *_labelCashInfo;
	cocos2d::Label *_labelMembersInfo;
	cocos2d::Label *_labelPursuedLevelInfo;
	cocos2d::Label *_currentTaskLabel;

	Cell::WeakPtr _cell;
	Task::WeakPtr _cellCurrentTask;
	CellMenuSelector *_cellMenuSelector;
};

#endif // CELL_INFO_MENU_H
