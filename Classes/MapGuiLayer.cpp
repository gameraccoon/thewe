#include "MapGuiLayer.h"

#include <math.h>

#include "World.h"
#include "WorldLoader.h"
#include "GameScene.h"
#include "WorldMapLayer.h"

MapGuiLayer::MapGuiLayer(MapProjector *mapProjector)
	: _mapProjector(mapProjector)
{
	init();
}

bool MapGuiLayer::init(void)
{
	if (!cocos2d::Layer::init())
	{
		return false;
	}

	setTouchEnabled(true);
    setKeypadEnabled(true);

	{
		using namespace cocos2d;
		_btnZoomIn = cocos2d::CCMenuItemImage::create("Btn_Plus.png", "Btn_Plus_Pressed.png",
			this, menu_selector(MapGuiLayer::_MenuInputListener));
		_btnZoomOut = cocos2d::CCMenuItemImage::create("Btn_Minus.png", "Btn_Minus_Pressed.png",
			this, menu_selector(MapGuiLayer::_MenuInputListener));
		_btnEditor = cocos2d::CCMenuItemImage::create("Btn_Edit.png", "Btn_Edit_Pressed.png",
			this, menu_selector(MapGuiLayer::_MenuInputListener));
		_btnSave = cocos2d::CCMenuItemImage::create("Btn_S.png", "Btn_S_Pressed.png",
			this, menu_selector(MapGuiLayer::_MenuInputListener));

		/*
		_btnZoomIn = cocos2d::CCMenuItemImage::create("btn-zoom-in-normal.png", "btn-zoom-in-selected.png",
			this, menu_selector(MapGuiLayer::_MenuInputListener));
		_btnZoomOut = cocos2d::CCMenuItemImage::create("btn-zoom-out-normal.png", "btn-zoom-out-selected.png",
			this, menu_selector(MapGuiLayer::_MenuInputListener));
		_btnEditor = cocos2d::CCMenuItemImage::create("btn-editor-normal.png", "btn-editor-selected.png",
			this, menu_selector(MapGuiLayer::_MenuInputListener));
		_btnSave = cocos2d::CCMenuItemImage::create("btn-save-game-normal.png", "btn-save-game-selected.png",
			this, menu_selector(MapGuiLayer::_MenuInputListener));
		*/
	}
	
	cocos2d::CCDirector *director = cocos2d::CCDirector::sharedDirector();
	Vector2 screen = director->getVisibleSize();
	Vector2 origin = director->getVisibleOrigin();

	Vector2 pos(0.0f, origin.y + screen.y);

	_printTime = cocos2d::CCLabelTTF::create("Time: 0", "Arial", 32);
	_printTime->setPosition(Vector2(pos.x, origin.y + screen.y - 50));
	
	_btnEditor->setScale(1.0f);
	_btnEditor->setTag(MENU_ITEM_EDITOR);
	_btnEditor->setPosition(pos);
	_btnEditor->setAnchorPoint(cocos2d::Vec2(0.0f, 1.0f));
		
	pos += Vector2(_btnEditor->getContentSize().width - 8.0f, 0.0f);

	_btnSave->setScale(1.0f);
	_btnSave->setTag(MENU_ITEM_SAVE);
	_btnSave->setPosition(pos);
	_btnSave->setAnchorPoint(cocos2d::Vec2(0.0f, 1.0f));

	pos += Vector2(_btnSave->getContentSize().width - 7.5f, 0.0f);
	
	_btnZoomOut->setScale(1.0f);
	_btnZoomOut->setTag(MENU_ITEM_ZOOM_OUT);
	_btnZoomOut->setPosition(pos);
	_btnZoomOut->setAnchorPoint(cocos2d::Vec2(0.0f, 1.0f));
	
	pos += Vector2(_btnZoomOut->getContentSize().width - 7.5f, 0.0f);
	
	_btnZoomIn->setScale(1.0f);
	_btnZoomIn->setTag(MENU_ITEM_ZOOM_IN);
	_btnZoomIn->setPosition(pos);
	_btnZoomIn->setAnchorPoint(cocos2d::Vec2(0.0f, 1.0f));

	cocos2d::CCMenu *menu = cocos2d::CCMenu::create(_btnZoomIn, _btnZoomOut, _btnEditor, _btnSave, NULL);
	menu->setPosition(0.0f, 0.0f);

	addChild(menu);
	addChild(_printTime);
	
	scheduleUpdate();

	return true;
}

void MapGuiLayer::_MenuInputListener(cocos2d::CCObject *sender)
{
	cocos2d::CCMenuItemImage *item = dynamic_cast<cocos2d::CCMenuItemImage*>(sender);

	int tag = item->getTag();

	switch (tag)
	{
	case MENU_ITEM_ZOOM_IN:
		dynamic_cast<WorldMapLayer*>(getParent())->ModifyZoom(1.25f);
		break;
	case MENU_ITEM_ZOOM_OUT:
		dynamic_cast<WorldMapLayer*>(getParent())->ModifyZoom(0.8f);
		break;
	case MENU_ITEM_EDITOR:
		dynamic_cast<GameScene*>(getParent()->getParent())->ToggleEditor();
		break;
	case MENU_ITEM_PIN:
		dynamic_cast<WorldMapLayer*>(getParent())->ModifyZoom(1.25f);
		break;
	case MENU_ITEM_SAVE:
		WorldLoader::SaveGameState();
		break;
	default: break;
	}
}
