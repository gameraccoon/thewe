#ifndef CELL_GAME_INTERFACE_H
#define CELL_GAME_INTERFACE_H

#include <cocos2d.h>
#include <cocos-ext.h>
#include <cocostudio/CCSGUIReader.h>
#include <ui/CocosGUI.h>

#include "Cell.h"
#include "TaskManager.h"
#include "ProgressBar.h"
#include "World.h"

class WorldMapLayer;
class CellMapPopupButton;

class CellMenuSelector : public cocos2d::Layer
{
public:
	CellMenuSelector(MapProjector *proj, WorldMapLayer *map);

	static CellMenuSelector* create(MapProjector *proj, WorldMapLayer *map);

	virtual bool init() override;
	virtual void update(float dt) override;

	bool IsCursorOnMenu(const Vector2 &cursorPos) const;
	
	void AppearImmediately(Cell::WeakPtr cell, const Vector2 &position);
	void AppearWithAnimation(Cell::WeakPtr cell, const Vector2 &position);
	void DisappearImmedaitely(void);
	void DisappearWithAnimation(void);
	void InitButtons(Cell::Ptr cell);

	void OnMenuClosed(void);

private:
	typedef cocos2d::Vector<cocos2d::MenuItem*> Buttons;

private:
	void _PrepareButtonToAppear(cocos2d::MenuItem* item, Vector2 pos);
	void _PrepareButtonToDisappear(cocos2d::MenuItem* item);
	bool _IsAnimationFinished(void);
	void CreateMenu(cocos2d::Layer* menu);
	// listeners
	void OnKillButtonPressed(cocos2d::Ref *sender);
	void OnCellInfoButtonPressed(cocos2d::Ref *sender);
	void OnTasksButtonPressed(cocos2d::Ref *sender);
	void OnSpinoffButtonPressed(cocos2d::Ref *sender);

private:
	WorldMapLayer *_worldMapLayer;
	MapProjector *_projector;

	cocos2d::Menu *_menu;

	Cell::WeakPtr _cell;
	Vector2 _position;

	bool _isDisappearing;
	const std::string _menuNodeName;

	Buttons _buttons;
};

#endif
