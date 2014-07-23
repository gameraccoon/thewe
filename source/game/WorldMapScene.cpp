#include "WorldMapScene.h"

WorldMapScene::WorldMapScene(void)
{
	init();
}

bool WorldMapScene::init(void)
{
	if (!cocos2d::CCScene::init())
	{
		return false;
	}

	WorldMapLayer *layer = new WorldMapLayer();

	addChild(layer);

	return true;
}