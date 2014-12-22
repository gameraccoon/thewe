#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include <string>

#include <cocos2d.h>

#include "MapProjector.h"
#include "WorldMapLayer.h"
#include "MainMenuScene.h"
#include "GameInfo.h"
#include "Cell.h"
#include "Town.h"

class GameScene : public cocos2d::Scene
{
public:
	GameScene(MainMenuScene *mainMenuScene);
	virtual ~GameScene();

	virtual bool init() override;
	virtual void update(float delta) override;
	
	void ShowMap();
	void GoToMainMenu(void);
	void ToggleEditor();
	void SetInputEnabled(bool enabled);

	void MoveViewToPoint(const Vector2& worldPoint);

private:
	MapProjector *_mapProjector;

	WorldMapLayer *_worldMap;
	MainMenuScene *_mainMenuScene;
	cocos2d::Layer *_regionEditor;
	cocos2d::Layer *_cellScreen;
};

#endif
