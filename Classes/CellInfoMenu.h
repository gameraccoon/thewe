#ifndef CELL_INFO_MENU_H
#define CELL_INFO_MENU_H

#include "CellGameInterface.h"

class CellInfoMenu : public cocos2d::Layer
{
public:
	CellInfoMenu(Cell::WeakPtr cell, CellMenuSelector *selector);

	virtual bool init(void) override;

	void UpdateInfoBy(Cell::Ptr cell);

private:
	void _OnCloseCallback(cocos2d::Ref *sender);

	cocos2d::Label *_labelCachInfo;
	cocos2d::Label *_labelMembersInfo;
	cocos2d::Label *_labelContentmentInfo;
	cocos2d::Label *_labelMoraleInfo;
	cocos2d::Label *_labelChildrensInfo;

	Cell::WeakPtr _cell;
	CellMenuSelector *_cellMenuSelector;
};

#endif // CELL_INFO_MENU_H