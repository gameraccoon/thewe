#ifndef CELL_SPINOFF_MENU
#define CELL_SPINOFF_MENU

#include "CellGameInterface.h"

class CellSpinoffMenu : public cocos2d::Layer
{
public:
	CellSpinoffMenu(Cell::WeakPtr cell, CellMenuSelector *selector);

	virtual bool init(void) override;

private:
	void _OnCloseCallback(cocos2d::Ref *sender);

	Cell::WeakPtr _cell;
	CellMenuSelector *_cellMenuSelector;
};

#endif // CELL_INFO_MENU_H