#ifndef MAP_GUI_H
#define MAP_GUI_H

#include "cocos2d.h"

class MapGuiLayer : public cocos2d::CCLayer
{
public:
	MapGuiLayer(void);

	virtual bool init(void) override;

	void menuCloseCallback(cocos2d::CCObject *Sender);

private:
	enum E_MENU_TEMS_TAG
	{
		MENU_ITEM_ZOOM_IN,
		MENU_ITEM_ZOOM_OUT,
		MENU_ITEM_EDITOR
	};

private:
	void _MenuInputListener(cocos2d::CCObject *sender);

	cocos2d::CCMenuItemImage *_btnZoomIn;
	cocos2d::CCMenuItemImage *_btnZoomOut;
	cocos2d::CCMenuItemImage *_btnEditor;
};

#endif
