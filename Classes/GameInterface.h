#ifndef GAME_INTERFACE_H
#define GAME_INTERFACE_H

#include <cocos2d.h>
#include <cocos-ext.h>
#include <cocostudio\CCSGUIReader.h>
#include <ui\CocosGUI.h>

#include "MessageManager.h"
#include "MapProjector.h"

class HudWidget;
class CellRadialMenu;

class GameInterface : public cocos2d::Layer, public MessageReceiver
{
public:
	static GameInterface* create(MapProjector *projector);

	enum class DrawOrder
	{
		CELL_RADIAL_MENU,
		CELL_INGAME_MENU,
		HUD
	};

	void AcceptMessage(const Message &message);

protected:
	GameInterface(MapProjector *projector);
	virtual ~GameInterface(void);

	bool init(void) override;

private:
	MapProjector *_projector;
	CellRadialMenu *_cellRadialMenu;
	HudWidget *_hud;
};

#endif
