#include "MapGuiLayer.h"

#include "WorldMap.h"
#include "GameScene.h"
#include "WorldMapLayer.h"

MapGuiLayer::MapGuiLayer(MapProjector *mapProjector)
	: _mapProjector(mapProjector)
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
		_btnZoomIn = cocos2d::CCMenuItemImage::create("btn-zoom-in-normal.png", "btn-zoom-in-selected.png",
			this, menu_selector(MapGuiLayer::_MenuInputListener));
		_btnZoomOut = cocos2d::CCMenuItemImage::create("btn-zoom-out-normal.png", "btn-zoom-out-selected.png",
			this, menu_selector(MapGuiLayer::_MenuInputListener));
		_btnEditor = cocos2d::CCMenuItemImage::create("btn-editor-normal.png", "btn-editor-selected.png",
			this, menu_selector(MapGuiLayer::_MenuInputListener));
	}
	
	cocos2d::CCDirector *director = cocos2d::CCDirector::sharedDirector();
	Point screen = director->getVisibleSize();
	Point origin = director->getVisibleOrigin();

	Point pos;
	pos.x = origin.x + 100.0f;
	pos.y = origin.y + screen.y - 100.0f;
	
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

	for (const Cell::Ptr cell : WorldMap::Instance().GetCells())
	{
		using namespace cocos2d;
		cocos2d::CCMenuItemSprite *pin = new cocos2d::CCMenuItemSprite();
		pin->initWithNormalSprite(
			cocos2d::CCSprite::create("pin.png"),
			cocos2d::CCSprite::create("pin.png"),
			cocos2d::CCSprite::create("pin.png"),
			this,
			menu_selector(MapGuiLayer::_MenuInputListener));
		pin->setTag(MENU_ITEM_PIN);
		//pin->setUserData();
		_mapProjector->AddMapPart(cell->GetLocation(), Point(100.0f, -300.0f), pin);
		addChild(pin);
		removeChild(pin);
		_mapProjector->RemoveMapPart(pin);
	}

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
	default: break;
	}
}