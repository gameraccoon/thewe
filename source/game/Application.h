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
		MENU_ITEM_RUN_WORLD_SCENE,
		MENU_ITEM_RUN_REGION_SCENE
	};

private:
	cocos2d::CCScene *_menuScene;

	cocos2d::CCMenuItemImage *_btnRunWorldTestScene;
	cocos2d::CCMenuItemImage *_btnRunCountryScene;
	cocos2d::CCMenu *_mainMenu;
};

#endif