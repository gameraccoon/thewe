#ifndef CELL_GAME_INTERFACE_H
#define CELL_GAME_INTERFACE_H

#include <cocos2d.h>
#include <cocos-ext.h>

#include "Cell.h"
#include "TaskManager.h"
#include "ProgressBar.h"
#include "World.h"

class WorldMapLayer;

class CellMenuSelector : public cocos2d::Layer
{
public:
	CellMenuSelector(MapProjector *proj, WorldMapLayer *map);
		
	virtual bool init() override;
	virtual void update(float dt) override;

	bool IsCursorOnMenu(const Vector2 &cursorPos) const;
	
	void AppearImmediately(Cell::WeakPtr cell, const Vector2 &position);
	void AppearWithAnimation(Cell::WeakPtr cell, const Vector2 &position);
	void DisappearImmedaitely(void);
	void DisappearWithAnimation(void);

	void OnCellMenuClosed(void);

private:
	enum CELL_MENU_TAGS
	{
		CELL_OPEN_TASKS = 0,
		CELL_OPEN_INFO,
		CELL_OPEN_SPINOFF,
		CELL_NUM_TAGS
	};

	typedef std::vector<cocos2d::MenuItemImage *> ButtonsList;

private:
	void _PrepearButtonToAppear(cocos2d::MenuItemImage *item, Vector2 pos);
	void _PrepearButtonToDisappear(cocos2d::MenuItemImage *item);
	void _MenuInputListener(cocos2d::Ref *sender);
	bool _IsAnimationFinished(void);

	WorldMapLayer *_worldMapLayer;
	MapProjector *_projector;

	cocos2d::Menu *_menu;

	Cell::WeakPtr _cell;
	Vector2 _position;
	ButtonsList _button;

	bool _isDisappearing;
	const std::string _menuNodeName;
};

#endif
