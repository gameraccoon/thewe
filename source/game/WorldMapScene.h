#ifndef WORLD_MAP_SCENE_H
#define WORLD_MAP_SCENE_H

#include "cocos2d.h"

#include "ConvexHullTest.h"

class WorldMapLayer : public cocos2d::CCLayer
{
public:
	virtual bool init(void);

	static cocos2d::CCScene* scene(void);

	void menuCloseCallback(cocos2d::CCObject *Sender);

	CREATE_FUNC(WorldMapLayer);

	void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
	void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
	void ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event);

	virtual void visit(void);

private:
	enum E_BUTTONTS_TAGS
	{
		BTN_SAVE_XML,
		BTN_TOGGLE,
	};

private:
	cocos2d::CCSprite *_movingSprite;
	cocos2d::CCPoint _touchPos;
	
	cocos2d::CCLabelTTF *_printPos;
	cocos2d::CCLabelTTF *_printNum;

	cocos2d::CCMenuItemImage *_btnSaveXml;
	cocos2d::CCMenuItemImage *_btnToggle;
	cocos2d::CCMenu *_mainMenu;

	ConvexHullTest _hull1;
	bool _isPointInHull;

	void _IdleUpdate(float timeDelta);
	void _MenuInputListener(cocos2d::CCObject *sender);
};

#endif
