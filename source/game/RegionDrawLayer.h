#ifndef REGION_DRAW_LAYER_H
#define REGION_DRAW_LAYER_H

#include "RegionTestScene.h"

class RegionDrawLayer : public cocos2d::CCLayer
{
public:
	RegionDrawLayer(void);

	virtual bool init(void);
	virtual void visit(void);
	
	void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
	void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
	void ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event);

private:
	enum E_MENU_TEMS_TAG
	{
		MENU_ITEM_TOGGLE,
		MENU_ITEM_DELETE,
		MENU_ITEM_SAVE_XML
	};

private:
	cocos2d::CCPoint _touchPos;
	
	cocos2d::CCLabelTTF *_printPos;
	cocos2d::CCLabelTTF *_printNum;
	
	cocos2d::CCMenuItemImage *_btnToggle;
	cocos2d::CCMenuItemImage *_btnDelete;
	cocos2d::CCMenuItemImage *_btnSaveXml;

	ArbitraryHull _hull1;
	bool _isPointInHull;
	bool _isCreationAllowed;

	void _MenuInputListener(cocos2d::CCObject *sender);
};

#endif