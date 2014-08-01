#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include <string>

#include <cocos2d.h>

#include "MapProjector.h"
#include "WorldMapLayer.h"
#include "Cell.h"

class GameScene : public cocos2d::CCScene
{
public:
	GameScene();

	bool init();
	
	void ShowMap();

	void ToggleEditor();
	void ShowRegionInfo(Region::Ptr region);
	void ShowCellScreen(Cell::Ptr cell);

	MapProjector _mapProjector;
private:
	WorldMapLayer *_worldMap;
	cocos2d::CCLayer *_regionInfo;
	cocos2d::CCLayer *_RegionEditor;
	cocos2d::CCLayer *_cellScreen;
};

#endif