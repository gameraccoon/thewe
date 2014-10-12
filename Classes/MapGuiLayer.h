#ifndef MAP_GUI_H
#define MAP_GUI_H

#include <cocos2d.h>
#include "MapProjector.h"
#include "MessageWidget.h"
#include "ProgressBar.h"

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
		,MENU_ITEM_SAVE
		,MENU_ITEM_MENU
	};

private:
	void _MenuInputListener(cocos2d::Ref *sender);

	int UpdateMessages();
	void UpdateMessagesPos();
	void AddNewMessage(int key, const std::map<int, UserMessage::Ptr>& messages);

private:
	cocos2d::MenuItemImage *_btnZoomIn;
	cocos2d::MenuItemImage *_btnZoomOut;
	cocos2d::MenuItemImage *_btnEditor;
	cocos2d::MenuItemImage *_btnSave;
	cocos2d::MenuItemImage *_btnMenu;

	MapProjector *_mapProjector;

	std::map<int, MessageWidget*> _userMessages;

	Vector2 _messagesMargin;
	Vector2 _messagesPosition;

	SquareProgressBar *_worldCaptureProgressBar;
};

#endif
