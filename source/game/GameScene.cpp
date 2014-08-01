#include "GameScene.h"

#include "WorldMapLayer.h"
#include "RegionInfoLayer.h"
#include "EditorLayer.h"
#include "CellScreenLayer.h"

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

	_worldMap = new WorldMapLayer(&_mapProjector);
	addChild(_worldMap);
	_worldMap->autorelease();

	// долгая операция
	WorldLoader::LoadWorld();

	return true;
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
	_cellScreen = new CellScreenLayer(cell);
	addChild(_cellScreen);
	_cellScreen->autorelease();
	_worldMap->SetMapInputEnabled(false);
	_worldMap->SetGuiEnabled(false);
}
