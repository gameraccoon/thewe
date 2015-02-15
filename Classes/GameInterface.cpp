#include "GameInterface.h"

#include "World.h"
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
	, _tasksMenu(nullptr)
{
	MessageManager::Instance().RegisterReceiver(this, "OpenTasksMenu");
	MessageManager::Instance().RegisterReceiver(this, "CloseTasksMenu");
}

GameInterface::~GameInterface(void)
{
	MessageManager::Instance().UnregisterReceiver(this, "OpenTasksMenu");
	MessageManager::Instance().UnregisterReceiver(this, "CloseTasksMenu");
}

void GameInterface::AcceptMessage(const Message &message)
{
	if (message.is("OpenTasksMenu"))
	{
		if (!_tasksMenu) {
			Cell::WeakPtr cell = World::Instance().GetCellsNetwork().GetCellByUid(message.variables.GetInt("UID"));
			_tasksMenu = TasksMenuWidget::create(cell);
			_tasksMenu->Show();
			addChild(_tasksMenu, DrawOrder::CELL_INGAME_MENU);
		}
	}
	if (message.is("CloseTasksMenu"))
	{
		if (_tasksMenu) {
			removeChild(_tasksMenu);
			_tasksMenu = nullptr;
		}
	}
}

bool GameInterface::init(void)
{
	if (!cocos2d::Layer::init()) {
		return false;
	}

	_cellRadialMenu = CellRadialMenu::create(_projector);
	_hud = HudWidget::create();

	addChild(_hud, DrawOrder::HUD);
	addChild(_cellRadialMenu, DrawOrder::CELL_RADIAL_MENU);

	return true;
}