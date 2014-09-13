#include "MainMenuScene.h"

#include "Vector2.h"
#include "Log.h"

MainMenuScene::MainMenuScene(void)
{
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
			this, menu_selector(MainMenuScene::_MenuInputListener));
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

	_AddButton("btn-menu_map-normal.png", "btn-menu_map-selected.png", center - Vector2(0.0f, 0.0f),
			  MenuItemTag::MAP);

	// переводим map в массив
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
		cocos2d::Director::getInstance()->popScene();
		break;
	case MainMenuScene::MenuItemTag::MESSAGES:
		break;
	case MainMenuScene::MenuItemTag::SETTINGS:
		break;
	case MainMenuScene::MenuItemTag::EXIT:
		{
			cocos2d::Director *director = cocos2d::Director::getInstance();
			director->end();
		}
		break;
	default: break;
	}
}
