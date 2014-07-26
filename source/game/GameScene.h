#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include <string>

#include <cocos2d.h>

#include "MapProjector.h"

class GameScene : public cocos2d::CCScene
{
public:
	GameScene();

	bool init();
	
	void ShowMap();

	void ShowEditor();
	void ShowRegionInfo(const std::string& regionName);

	MapProjector _mapProjector;
private:
	cocos2d::CCLayer *_worldMap;
	cocos2d::CCLayer *_regionInfo;
	cocos2d::CCLayer *_editor;
};

#endif