#ifndef APPLICATION_H
#define APPLICATION_H

#include <cocos2d.h>
#include <pugixml.hpp>

using namespace cocos2d;

class AppDelegate : private cocos2d::CCApplication
{
public:
	AppDelegate();
    virtual ~AppDelegate();

    virtual bool applicationDidFinishLaunching();
    virtual void applicationDidEnterBackground();
    virtual void applicationWillEnterForeground();
	void _MenuInputListener(CCObject *sender);

private:
	enum E_MENU_ITEMS_TAGS
	{
		MENU_ITEM_RUN_GAME,
		MENU_ITEM_TEST_SCENE_1,
		MENU_ITEM_EXIT
	};

private:
	cocos2d::CCScene *_menuScene;

	cocos2d::CCMenuItemImage *_btnRunGame;
	cocos2d::CCMenuItemImage *_btnTestScene1;
	cocos2d::CCMenuItemImage *_btnExitGame;
	cocos2d::CCMenu *_mainMenu;
};

#endif