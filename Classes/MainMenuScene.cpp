#include "MainMenuScene.h"

#include "WorldMapLayer.h"
#include "RegionInfoLayer.h"
#include "EditorLayer.h"
#include "World.h"
#include "Log.h"

MainMenuScene::MainMenuScene(void)
{
}

MainMenuScene::~MainMenuScene(void)
{
	Log::Instance().writeLog("Main menu unloaded sucessfully");
}

bool MainMenuScene::init(void)
{
	if (!cocos2d::CCScene::init())
	{
		return false;
	}

	cocos2d::Director *director = cocos2d::Director::getInstance();

	{
		using namespace cocos2d;
		_btnRunGame = MenuItemImage::create("btn-start-game-normal.png", "btn-start-game-selected.png",
			CC_CALLBACK_1(MainMenuScene::_MenuInputListener, this));
		_btnTestScene1 = MenuItemImage::create("btn-test1-normal.png", "btn-test1-selected.png",
			CC_CALLBACK_1(MainMenuScene::_MenuInputListener, this));
		_btnExitGame = MenuItemImage::create("btn-exit-normal.png", "btn-exit-selected.png",
			CC_CALLBACK_1(MainMenuScene::_MenuInputListener, this));
	}

	Vector2 client = director->getVisibleSize();
	Vector2 origin = director->getVisibleOrigin();
	Vector2 center(origin.x + client.x / 2.0f, origin.y + client.y - 100.0f);

	_btnRunGame->setPosition(center - Vector2(0.0f, 0.0f));
	_btnRunGame->setTag(MENU_ITEM_RUN_GAME);
	_btnRunGame->setScale(3.0f);
	_btnTestScene1->setPosition(center - Vector2(0.0f, 135.0f));
	_btnTestScene1->setTag(MENU_ITEM_TEST_SCENE_1);
	_btnTestScene1->setScale(3.0f);
	_btnExitGame->setPosition(center - Vector2(0.0f, 270.0f));
	_btnExitGame->setTag(MENU_ITEM_EXIT);
	_btnExitGame->setScale(3.0f);

	_mainMenu = cocos2d::Menu::create(_btnRunGame, _btnTestScene1, _btnExitGame, NULL);
	_mainMenu->setPosition(0.0f, 0.0f);

	addChild(_mainMenu);

	return true;
}

void MainMenuScene::_MenuInputListener(cocos2d::Ref *sender)
{
	cocos2d::Director *director = cocos2d::Director::getInstance();
	cocos2d::Scene *scene = NULL;

	cocos2d::MenuItemImage *item = dynamic_cast<cocos2d::MenuItemImage*>(sender);

	int tag = item->getTag();

	switch (tag)
	{
	case MENU_ITEM_RUN_GAME:
		cocos2d::Director::getInstance()->popScene();
		break;
	case MENU_ITEM_TEST_SCENE_1:
		break;
	case MENU_ITEM_EXIT:
		{
			cocos2d::Director *director = cocos2d::Director::getInstance();
			director->end();
		}
		break;
	default: break;
	}
}
