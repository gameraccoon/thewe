#include "GameScene.h"

#include "WorldMapLayer.h"
#include "RegionInfoLayer.h"
#include "RegionDrawLayer.h"

GameScene::GameScene(void)
	: _editor(nullptr)
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

	_worldMap = new WorldMapLayer();
	addChild(_worldMap);
	_worldMap->autorelease();

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
}

void GameScene::ShowEditor()
{
	ShowMap();
	_editor = new RegionDrawLayer();
	addChild(_editor);
	_editor->autorelease();
}

void GameScene::ShowRegionInfo(const std::string& regionName)
{
	ShowMap();
	_regionInfo = new RegionInfoLayer();
	addChild(_regionInfo);
	_regionInfo->autorelease();
}