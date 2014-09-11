#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <cocos2d.h>

class MainMenuScene : public cocos2d::Scene
{
public:
	MainMenuScene();
	virtual ~MainMenuScene();

	virtual bool init() override;

	void _MenuInputListener(cocos2d::Ref *sender);

private:
	enum E_MENU_ITEMS_TAGS
	{
		MENU_ITEM_RUN_GAME,
		MENU_ITEM_TEST_SCENE_1,
		MENU_ITEM_EXIT
	};

private:
	cocos2d::MenuItemImage *_btnRunGame;
	cocos2d::MenuItemImage *_btnTestScene1;
	cocos2d::MenuItemImage *_btnExitGame;
	cocos2d::Menu *_mainMenu;

private:
};

#endif // MAIN_MENU_H
