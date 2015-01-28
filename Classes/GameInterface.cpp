#include "GameInterface.h"

#include "CellRadialMenu.h"

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

	addChild(_cellRadialMenu, (int)DrawOrder::CELL_RADIAL_MENU);

	return true;
}