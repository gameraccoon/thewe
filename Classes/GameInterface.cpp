#include "GameInterface.h"

#include "HudWidget.h"
#include "CellRadialMenu.h"
#include "TasksMenuWidget.h"

GameInterface* GameInterface::create(MapProjector *projector)
{
	GameInterface *ret = new GameInterface(projector);
	if (ret && ret->init())
	{
		ret->autorelease();
		return ret;
	}

	delete ret;
	return nullptr;
}

GameInterface::GameInterface(MapProjector *projector)
	: _projector(projector)
{
}

GameInterface::~GameInterface(void)
{
}

void GameInterface::AcceptMessage(const Message &message)
{
}

bool GameInterface::init(void)
{
	if (!cocos2d::Layer::init()) {
		return false;
	}

	_cellRadialMenu = CellRadialMenu::create(_projector);
	_tasksMenu = TasksMenuWidget::create();
	_hud = HudWidget::create();

	addChild(_hud, (int)DrawOrder::HUD);
	addChild(_cellRadialMenu, (int)DrawOrder::CELL_RADIAL_MENU);
	addChild(_tasksMenu, (int)DrawOrder::CELL_INGAME_MENU);

	return true;
}