#include "MainMenuScene.h"

#include "Color.h"
#include "Vector2.h"
#include "Log.h"
#include "GameScene.h"
#include "TransitionZoomFade.h"
#include "CellsNetLayer.h"

MainMenuScene::MainMenuScene(cocos2d::Scene* gameScene)
{
	_gameScene = gameScene;
}

MainMenuScene::~MainMenuScene(void)
{
	Log::Instance().writeLog("Main menu unloaded sucessfully");
}

void MainMenuScene::_AddButton(std::string imgNormal, std::string imgPressed, Vector2 position, MenuItemTag tag)
{
	cocos2d::MenuItem *button;

	{
		using namespace cocos2d;
		button = cocos2d::MenuItemImage::create(imgNormal.c_str(), imgPressed.c_str(),
			CC_CALLBACK_1(MainMenuScene::_MenuInputListener, this));
	}

	button->setPosition(position);
	button->setTag(static_cast<int>(tag));
	_buttons.insert(std::pair<MenuItemTag, cocos2d::MenuItem*>(tag, button));
}

bool MainMenuScene::init(void)
{
	if (!cocos2d::CCScene::init())
	{
		return false;
	}

	cocos2d::Director *director = cocos2d::Director::getInstance();

	Vector2 client = director->getVisibleSize();
	Vector2 origin = director->getVisibleOrigin();
	Vector2 center(origin.x + client.x / 2.0f, origin.y + client.y - 100.0f);

	_AddButton("btn-menu_map-normal.png", "btn-menu_map-selected.png", center + Vector2(0.0f, -100.0f),
			  MenuItemTag::MAP);

	_AddButton("btn-mail-normal.png", "btn-mail-selected.png", center + Vector2(-300.0f, -270.0f),
			  MenuItemTag::MAILBOX);

	_AddButton("btn-chat-normal.png", "btn-chat-selected.png", center + Vector2(-100.0f, -270.0f),
			  MenuItemTag::CHAT);

	_AddButton("btn-command-normal.png", "btn-command-selected.png", center + Vector2(100.0f, -270.0f),
			  MenuItemTag::TEAM);

	_AddButton("btn-settings-normal.png", "btn-settings-selected.png", center + Vector2(300.0f, -270.0f),
			  MenuItemTag::SETTINGS);

	// cast map to vector
	cocos2d::Vector<cocos2d::MenuItem*> v;
	v.reserve(_buttons.size());
	std::for_each(_buttons.begin(),_buttons.end(),
		[&v](const std::map<MenuItemTag, cocos2d::MenuItem*>::value_type& p)
		{ v.pushBack(p.second); });

	_mainMenu = cocos2d::Menu::createWithArray(v);
	_mainMenu->setPosition(0.0f, 0.0f);

	addChild(_mainMenu);

	return true;
}

void MainMenuScene::_MenuInputListener(cocos2d::Ref *sender)
{
	cocos2d::MenuItem *item = dynamic_cast<cocos2d::MenuItem*>(sender);

	MenuItemTag tag = static_cast<MenuItemTag>(item->getTag());

	switch (tag)
	{
	case MainMenuScene::MenuItemTag::MAP:
	{
		GameScene* scene = new GameScene();
		scene->autorelease();
		scene->init();
		cocos2d::TransitionScene* transition = TransitionZoomFade::create(1, scene);
		cocos2d::Director::getInstance()->replaceScene(transition);
	}
		break;
	case MainMenuScene::MenuItemTag::MAILBOX:
		break;
	case MainMenuScene::MenuItemTag::SETTINGS:
		break;
	case MainMenuScene::MenuItemTag::CHAT:
		break;
	case MainMenuScene::MenuItemTag::TEAM:
		break;
	default: break;
	}
}
