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
	void OnCloseCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType eventType);
	void OnSpinoffCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType eventType);
	void SelfClose(void);
	void KeyReleased(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event *event);

private:
	bool _isCellCreationPossible;

	cocos2d::ui::Layout *_widget;
	cocos2d::ui::Text *_cachTextWidget;
	cocos2d::ui::Text *_membersTextWidget;
	WorldMapLayer *_worldMapLayer;

	Cell::WeakPtr _cell;
	CellMenuSelector *_cellMenuSelector;
};

#endif // CELL_INFO_MENU_H
