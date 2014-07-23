#ifndef COUNTRY_TEST_SCENE_H
#define COUNTRY_TEST_SCENE_H

#include "cocos2d.h"

#include "ConvexHullTest.h"

using namespace cocos2d;

class RegionTestScene : public CCScene
{
public:
	RegionTestScene(void);

	virtual bool init(void);
	//virtual void visit(void);
	
	//void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
	//void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
	//void ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event);

private:
	cocos2d::CCMenuItemImage *_btnSaveXml;
	cocos2d::CCMenuItemImage *_btnToggle;
	cocos2d::CCMenu *_mainMenu;

	void _MenuInputListener(cocos2d::CCObject *sender);
};

#endif