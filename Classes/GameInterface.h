#ifndef GAME_INTERFACE_H
#define GAME_INTERFACE_H

#include <cocos2d.h>
#include <cocos-ext.h>
#include <cocostudio\CCSGUIReader.h>
#include <ui\CocosGUI.h>

#include "MessageManager.h"
#include "MapProjector.h"

class HudWidget;
class TasksMenuWidget;
class CellRadialMenu;

class GameInterface : public cocos2d::Layer, public MessageReceiver
{
public:
	static GameInterface* create(MapProjector *projector);

	enum DrawOrder
	{
		CELL_RADIAL_MENU,
		HUD,
		CELL_INGAME_MENU
	};

	void AcceptMessage(const Message &message);

protected:
	GameInterface(MapProjector *projector);
	virtual ~GameInterface(void);

	bool init(void) override;

private:
	MapProjector *_projector;
	CellRadialMenu *_cellRadialMenu;
	TasksMenuWidget *_tasksMenu;
	HudWidget *_hud;
};

#endif
