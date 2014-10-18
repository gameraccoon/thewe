#include "GameScene.h"

#include "WorldMapLayer.h"
#include "EditorLayer.h"
#include "World.h"
#include "Log.h"
#include "TransitionZoomFade.h"
#include "TutorialLayer.h"
#include "NotificationMessageLayer.h"

GameScene::GameScene(MainMenuScene *mainMenuScene)
	: _mapProjector()
	, _mainMenuScene(mainMenuScene)
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

	addChild(TutorialLayer::create());
	addChild(NotificationMessageLayer::create());

	return true;
}

void GameScene::update(float delta)
{
	(void)delta;
	World::Instance().Update();
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

void GameScene::GoToMainMenu(void)
{
	MainMenuScene* mainMenuScene = new MainMenuScene(nullptr);
	mainMenuScene->init();
	mainMenuScene->autorelease();

	cocos2d::Scene *scene = dynamic_cast<cocos2d::Scene *>(mainMenuScene);
	
	cocos2d::TransitionScene* transition = cocos2d::TransitionSlideInL::create(0.5f, scene);
	cocos2d::Director::getInstance()->replaceScene(transition);
}

void GameScene::ToggleEditor()
{
	if (!_RegionEditor)
	{
		ShowMap(); // changes state of _editor
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

void GameScene::SetInputEnabled(bool enabled)
{
	_worldMap->SetMapInputEnabled(enabled);
}
