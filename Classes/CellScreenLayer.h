#ifndef CELL_SCREEN_LAYER_H
#define CELL_SCREEN_LAYER_H

#include <cocos2d.h>
#include "Cell.h"
#include "WorldMapLayer.h"
#include "TaskManager.h"

class CellScreenLayer : public cocos2d::Layer
{
public:
	CellScreenLayer(Cell::WeakPtr cell, WorldMapLayer *worldMapLayer);

	virtual bool init() override;
	virtual void update(float delta) override;
	
private:
	enum E_MENU_TEMS_TAG
	{
		MENU_ITEM_BACK
		,MENU_ITEM_CREATE_CHILD
	};

private:
	WorldMapLayer *_worldMapLayer;

	cocos2d::MenuItemImage *_btnBack;
	cocos2d::MenuItemImage *_btnCreteChild;
	cocos2d::LabelTTF *_membersText;
	cocos2d::LabelTTF *_cashText;
	cocos2d::LabelTTF *_moralText;
	cocos2d::LabelTTF *_contentmentText;
	cocos2d::LabelTTF *_childCountText;
	cocos2d::LabelTTF *_hasParentText;
	cocos2d::LabelTTF *_currentTaskText;
	
	cocos2d::DrawNode *_bkgDraw;

	Cell::WeakPtr _cell;

	TaskManager::TasksList _availableTasks;

	void _MenuInputListener(cocos2d::Object *sender);
	void _InitBackground(cocos2d::DrawNode *background) const;

	void _TaskClickListener(cocos2d::Object *sender);
};

#endif // CELL_SCREEN_LAYER_H