#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <cocos2d.h>
#include "Vector2.h"

class MainMenuScene : public cocos2d::Scene
{
public:
	MainMenuScene();
	virtual ~MainMenuScene();

	virtual bool init() override;

private:
	enum class MenuItemTag
	{
		MAP,
		SETTINGS,
		MAILBOX,
		TEAM,
		CHAT
	};

private:
	std::map<MenuItemTag, cocos2d::MenuItem*> _buttons;
	cocos2d::Menu *_mainMenu;

private:
	void _AddButton(std::string imgNormal, std::string imgPressed, Vector2 position, MenuItemTag tag);
	void _MenuInputListener(cocos2d::CCObject *sender);
};

#endif // MAIN_MENU_H
