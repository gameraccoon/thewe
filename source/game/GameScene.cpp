#include "GameScene.h"

#include "WorldMapLayer.h"
#include "RegionInfoLayer.h"
#include "EditorLayer.h"
#include "CellScreenLayer.h"
#include "World.h"

GameScene::GameScene(void)
	: _mapProjector(Point(1390.0f, 1003.0f))
	, _RegionEditor(nullptr)
	, _regionInfo(nullptr)
	, _worldMap(nullptr)
	, _cellScreen(nullptr)
{
	init();
}

bool GameScene::init(void)
{
	if (!cocos2d::CCScene::init())
	{
		return false;
	}

	// долгая операция
	GameLoader::LoadGameInfo();
	GameLoader::LoadGameState();

	_worldMap = new WorldMapLayer(this, &_mapProjector);
	_townInfo = new TownInfoLayer();
	addChild(_worldMap);
	addChild(_townInfo);
	_worldMap->autorelease();
	_townInfo->autorelease();
	_townInfo->setVisible(false);

	scheduleUpdate();

	return true;
}

void GameScene::update(float delta)
{
	World::Instance().Update(delta);
}

void GameScene::ShowMap()
{
	if (_RegionEditor)
	{
		removeChild(_RegionEditor);
		_RegionEditor = nullptr;
	}

	if (_regionInfo)
	{
		removeChild(_regionInfo);
		_regionInfo = nullptr;
	}

	if (_cellScreen)
	{
		removeChild(_cellScreen);
		_cellScreen = nullptr;
	}

	_worldMap->SetMapInputEnabled(true);
	_worldMap->SetGuiEnabled(true);
}

void GameScene::ToggleEditor()
{
	if (!_RegionEditor)
	{
		ShowMap(); // изменяет стостояние _editor
		_RegionEditor = new EditorLayer(&_mapProjector);
		addChild(_RegionEditor);
		_RegionEditor->autorelease();
		_worldMap->SetMapInputEnabled(false);
	}
	else
	{
		ShowMap();
	}
}

void GameScene::ShowRegionInfo(Region::Ptr region)
{
	ShowMap();
	_regionInfo = new RegionInfoLayer(region->GetInfo());
	addChild(_regionInfo);
	_regionInfo->autorelease();
	_worldMap->SetMapInputEnabled(false);
	_worldMap->SetGuiEnabled(false);
}

void GameScene::ShowCellScreen(Cell::Ptr cell)
{
	ShowMap();
	_cellScreen = new CellScreenLayer(cell, _worldMap);
	addChild(_cellScreen);
	_cellScreen->autorelease();
	_worldMap->SetMapInputEnabled(false);
	_worldMap->SetGuiEnabled(false);
}

void GameScene::ShowTownInfo(Town::Ptr town)
{
	if (town)
	{
		dynamic_cast<TownInfoLayer *>(_townInfo)->SelectTown(town);
		_townInfo->setVisible(true);
	}
	else
	{
		_townInfo->setVisible(false);
	}
}
