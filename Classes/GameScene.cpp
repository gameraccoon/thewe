#include "GameScene.h"

#include "WorldMapLayer.h"
#include "EditorLayer.h"
#include "World.h"
#include "Log.h"
#include "TransitionZoomFade.h"
#include "TutorialLayer.h"
#include "NotificationMessageLayer.h"
#include "ThreadUtils.h"

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
	WRITE_LOG("GameScene unloaded sucessfully");
}

bool GameScene::init(void)
{
	if (!cocos2d::Scene::init())
	{
		return false;
	}

	_worldMap = new WorldMapLayer(this, &_mapProjector);
	addChild(_worldMap);
	_worldMap->autorelease();

	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("bg-gameplay.mp3", true);

	scheduleUpdate();

	addChild(TutorialLayer::create(_worldMap, &_mapProjector));
	addChild(NotificationMessageLayer::create());

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
}

void GameScene::GoToMainMenu(void)
{
	MainMenuScene* mainMenuScene = new MainMenuScene();
	mainMenuScene->init();
	mainMenuScene->autorelease();
	
	cocos2d::TransitionScene* transition = cocos2d::TransitionSlideInL::create(0.5f, mainMenuScene);
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
	}
	else
	{
		ShowMap();
	}
}

void GameScene::MoveViewToPoint(const Vector2& worldPoint)
{
	_mapProjector.SetLocation(worldPoint);
}
