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
#include "CircleMenu.h"

class WorldMapLayer;
class CellMapPopupButton;

class CellMenuSelector : public cocos2d::Layer
{
public:
	CellMenuSelector(MapProjector *proj, WorldMapLayer *map);

	static CellMenuSelector* create(MapProjector *proj, WorldMapLayer *map);

	virtual bool init() override;
	virtual void update(float dt) override;
	
	void AppearImmediately(Cell::WeakPtr cell, const Vector2 &position);
	void AppearWithAnimation(Cell::WeakPtr cell, const Vector2 &position);
	void DisappearImmedaitely(void);
	void DisappearWithAnimation(void);
	void InitButtons(Cell::Ptr cell);

	void OnMenuClosed(void);

	bool isOpened() const;

private:
	void PrepareButtonToAppear(cocos2d::Node* item, Vector2 pos);
	void PrepareButtonToDisappear(cocos2d::Node* item);
	void CreateMenu(cocos2d::Layer* menu);
	void RemoveMenu();
	// listeners
	void OnKillButtonPressed(cocos2d::Ref *sender);
	void OnCellInfoButtonPressed(cocos2d::Ref *sender);
	void OnTasksButtonPressed(cocos2d::Ref *sender);
	void OnSpinoffButtonPressed(cocos2d::Ref *sender);

private:
	WorldMapLayer *_worldMapLayer;
	MapProjector *_projector;

	Cell::WeakPtr _cell;

	CircleMenu *_circleMenu;
	const std::string _menuNodeName;
};

#endif
