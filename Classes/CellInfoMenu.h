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

	SquareProgressBar *_taskProgressBar;

	cocos2d::Label *_labelCachInfo;
	cocos2d::Label *_labelMembersInfo;
	cocos2d::Label *_labelContentmentInfo;
	cocos2d::Label *_labelMoraleInfo;
	cocos2d::Label *_labelChildrensInfo;
	cocos2d::Label *_currentTaskLabel;

	Cell::WeakPtr _cell;
	Task::Ptr _cellCurrentTask;
	CellMenuSelector *_cellMenuSelector;
};

#endif // CELL_INFO_MENU_H