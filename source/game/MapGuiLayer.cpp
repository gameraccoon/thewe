#include "MapGuiLayer.h"

#include "WorldMap.h"
#include "GameScene.h"
#include "WorldMapLayer.h"

MapGuiLayer::MapGuiLayer(void)
{
	init();
}

bool MapGuiLayer::init(void)
{
	if (!CCLayer::init())
	{
		return false;
	}

	setTouchEnabled(true);
    setKeypadEnabled(true);

	{
		using namespace cocos2d;
		_btnZoomIn = cocos2d::CCMenuItemImage::create("../_gamedata/btn-zoom-in-normal.png",
			"../_gamedata/btn-zoom-in-selected.png", this, menu_selector(MapGuiLayer::_MenuInputListener));
		_btnZoomOut = cocos2d::CCMenuItemImage::create("../_gamedata/btn-zoom-out-normal.png",
			"../_gamedata/btn-zoom-out-selected.png", this, menu_selector(MapGuiLayer::_MenuInputListener));
		_btnEditor = cocos2d::CCMenuItemImage::create("../_gamedata/btn-editor-normal.png",
			"../_gamedata/btn-editor-selected.png", this, menu_selector(MapGuiLayer::_MenuInputListener));
	}

	
	cocos2d::CCDirector *director = cocos2d::CCDirector::sharedDirector();
	cocos2d::CCSize screen = director->getVisibleSize();
	cocos2d::CCPoint origin = director->getVisibleOrigin();

	cocos2d::CCPoint pos;
	pos.x = origin.x + 100.0f;
	pos.y = origin.y + screen.height - 100.0f;
	
	_btnZoomIn->setScale(2.0f);
	_btnZoomIn->setTag(MENU_ITEM_ZOOM_IN);
	_btnZoomIn->setPosition(pos - ccp(0.0f, 100.0f));
	_btnZoomOut->setScale(2.0f);
	_btnZoomOut->setTag(MENU_ITEM_ZOOM_OUT);
	_btnZoomOut->setPosition(pos - ccp(0.0f, 200.0f));
	_btnEditor->setScale(2.0f);
	_btnEditor->setTag(MENU_ITEM_EDITOR);
	_btnEditor->setPosition(pos - ccp(0.0f, 300.0f));

	cocos2d::CCMenu *menu = cocos2d::CCMenu::create(_btnZoomIn, _btnZoomOut, _btnEditor, NULL);
	menu->setPosition(0.0f, 0.0f);

	addChild(menu);

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
		dynamic_cast<GameScene*>(getParent()->getParent())->ShowEditor();
		break;
	default: break;
	}
}