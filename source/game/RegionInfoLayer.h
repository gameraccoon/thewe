#ifndef REGION_INFO_LAYER_H
#define REGION_INFO_LAYER_H

#include "MapProjector.h"

class RegionInfoLayer : public cocos2d::CCLayer
{
public:
	RegionInfoLayer();

	virtual bool init();
	
	void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
	void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
	void ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event);

private:
	enum E_MENU_TEMS_TAG
	{
		MENU_ITEM_BACK
	};

private:
	void NavigateBack();
	
	cocos2d::CCMenuItemImage *_btnBack;

	void _MenuInputListener(cocos2d::CCObject *sender);
};

#endif