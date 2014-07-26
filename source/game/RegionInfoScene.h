#ifndef REGION_INFO_SCENE_H
#define REGION_INFO_SCENE_H

#include <cocos2d.h>

class RegionInfoScene : public cocos2d::CCScene
{
public:
	RegionInfoScene();

	virtual bool init();
private:
	cocos2d::CCMenuItemImage *_btnSaveXml;
	cocos2d::CCMenuItemImage *_btnToggle;
	cocos2d::CCMenu *_mainMenu;

	void _MenuInputListener(cocos2d::CCObject *sender);
};

#endif