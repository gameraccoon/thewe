#ifndef MAP_GUI_H
#define MAP_GUI_H

#include <cocos2d.h>
#include <cocostudio/CCSGUIReader.h>
#include <ui/CocosGUI.h>

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
	void MenuInputListener(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType eventType);

	void ToggleCellsNetMenu();

private:
	cocos2d::ui::Layout *_widget;
	cocos2d::ui::LoadingBar *_worldCapturingBar;

	MapProjector *_mapProjector;

	CellsNetLayer* _cellsNetLayer;
};

#endif
