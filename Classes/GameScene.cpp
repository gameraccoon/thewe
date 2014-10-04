#include "GameScene.h"

#include "WorldMapLayer.h"
#include "EditorLayer.h"
#include "World.h"
#include "Log.h"

GameScene::GameScene(void)
	: _mapProjector(Vector2(1390.0f, 1003.0f))
	, _RegionEditor(nullptr)
	, _worldMap(nullptr)
	, _cellScreen(nullptr)
{
}

GameScene::~GameScene(void)
{
	Log::Instance().writeLog("GameScene unloaded sucessfully");
}

bool GameScene::init(void)
{
	if (!cocos2d::CCScene::init())
	{
		return false;
	}

	_worldMap = new WorldMapLayer(this, &_mapProjector);
	addChild(_worldMap);
	_worldMap->autorelease();

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

void GameScene::ShowCellScreen(Cell::WeakPtr cell)
{
}
