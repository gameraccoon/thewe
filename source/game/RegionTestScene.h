#ifndef COUNTRY_TEST_SCENE_H
#define COUNTRY_TEST_SCENE_H

#include "cocos2d.h"

#include "ArbitraryHull.h"

using namespace cocos2d;

class RegionTestScene : public CCScene
{
public:
	RegionTestScene(void);

	virtual bool init(void);

private:
	cocos2d::CCMenuItemImage *_btnSaveXml;
	cocos2d::CCMenuItemImage *_btnToggle;
	cocos2d::CCMenu *_mainMenu;

	void _MenuInputListener(cocos2d::CCObject *sender);
};

#endif