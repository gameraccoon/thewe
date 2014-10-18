#ifndef MAP_GUI_H
#define MAP_GUI_H

#include <cocos2d.h>
#include "MapProjector.h"
#include "ProgressBar.h"
#include "CellsNetLayer.h"

class MapGuiLayer : public cocos2d::Layer
{
public:
	MapGuiLayer(MapProjector *mapProjector);

	virtual bool init(void) override;
	virtual void update(float delta);

	void menuCloseCallback(cocos2d::Ref *Sender);

private:
	enum E_MENU_TEMS_TAG
	{
		MENU_ITEM_ZOOM_IN
		,MENU_ITEM_ZOOM_OUT
		,MENU_ITEM_EDITOR
		,MENU_ITEM_MENU
	};

private:
	void _MenuInputListener(cocos2d::Ref *sender);

	void ToggleCellsNetMenu();

private:
	cocos2d::MenuItemImage *_btnZoomIn;
	cocos2d::MenuItemImage *_btnZoomOut;
	cocos2d::MenuItemImage *_btnEditor;
	cocos2d::MenuItemImage *_btnMenu;

	MapProjector *_mapProjector;

	SquareProgressBar *_worldCaptureProgressBar;

	CellsNetLayer* _cellsNetLayer;
};

#endif
