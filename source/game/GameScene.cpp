#include "GameScene.h"

#include "WorldMapLayer.h"
#include "RegionInfoLayer.h"
#include "EditorLayer.h"

GameScene::GameScene(void)
	: _mapProjector(cocos2d::CCPoint(0.0f, 0.0f), 3.0f)
	, _editor(nullptr)
	, _regionInfo(nullptr)
	, _worldMap(nullptr)
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
	if (_editor)
	{
		removeChild(_editor);
		_editor = nullptr;
	}

	if (_regionInfo)
	{
		removeChild(_regionInfo);
		_regionInfo = nullptr;
	}

	_worldMap->SetInputEnabled(true);
}

void GameScene::ToggleEditor()
{
	if (!_editor)
	{
		ShowMap(); // изменяет стостояние _editor
		_editor = new EditorLayer(&_mapProjector);
		addChild(_editor);
		_editor->autorelease();
		_worldMap->SetInputEnabled(false);
	}
	else
	{
		ShowMap();
	}
}

void GameScene::ShowRegionInfo(const std::string& regionName)
{
	ShowMap();
	_regionInfo = new RegionInfoLayer();
	addChild(_regionInfo);
	_regionInfo->autorelease();
	_worldMap->SetInputEnabled(false);
}