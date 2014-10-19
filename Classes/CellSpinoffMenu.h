#ifndef CELL_SPINOFF_MENU
#define CELL_SPINOFF_MENU

#include "CellGameInterface.h"

class CellSpinoffMenu : public cocos2d::Layer
{
public:
	CellSpinoffMenu(Cell::WeakPtr cell, CellMenuSelector *selector, WorldMapLayer *worldMap);

	virtual bool init(void) override;

	void UpdateSpinoffState(const Cell::Info &info);

private:
	void _OnCloseCallback(cocos2d::Ref *sender);
	void _OnCreateNewCell(cocos2d::Ref *sender);
	void _SelfClose(void);
	void KeyReleased(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event *event);

private:
	bool _isCellCreationPossible;

	cocos2d::Label *_necessaryMembers;
	cocos2d::Label *_necessaryCash;
	cocos2d::MenuItemImage *_createCellButton;

	WorldMapLayer *_worldMapLayer;

	Cell::WeakPtr _cell;
	CellMenuSelector *_cellMenuSelector;
};

#endif // CELL_INFO_MENU_H
