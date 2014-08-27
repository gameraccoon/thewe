#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include <string>

#include <cocos2d.h>

#include "MapProjector.h"
#include "WorldMapLayer.h"
#include "Cell.h"
#include "Town.h"

class GameScene : public cocos2d::Scene
{
public:
	GameScene();

	virtual bool init() override;
	virtual void update(float delta) override;
	
	void ShowMap();

	void ToggleEditor();
	void ShowRegionInfo(Region::WeakPtr region);
	void ShowCellScreen(Cell::WeakPtr cell);
	void ShowTownInfo(Town::WeakPtr town);

	MapProjector _mapProjector;

private:
	WorldMapLayer *_worldMap;
	cocos2d::Layer *_regionInfo;
	cocos2d::Layer *_RegionEditor;
	cocos2d::Layer *_cellScreen;
	cocos2d::Layer *_townInfo;
};

#endif