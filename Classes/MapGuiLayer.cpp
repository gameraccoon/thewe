#include "MapGuiLayer.h"

#include "World.h"
#include "WorldLoader.h"
#include "GameScene.h"
#include "WorldMapLayer.h"
#include "CellsNetLayer.h"

MapGuiLayer::MapGuiLayer(MapProjector *mapProjector)
	: _mapProjector(mapProjector)
	, _cellsNetLayer(nullptr)
{
	init();
}

bool MapGuiLayer::init(void)
{
	if (!cocos2d::Layer::init())
	{
		return false;
	}

	{
		using namespace cocos2d;
		_btnZoomIn = cocos2d::CCMenuItemImage::create("Btn_Plus.png", "Btn_Plus_Pressed.png",
			CC_CALLBACK_1(MapGuiLayer::_MenuInputListener, this));
		_btnZoomOut = cocos2d::CCMenuItemImage::create("Btn_Minus.png", "Btn_Minus_Pressed.png",
			CC_CALLBACK_1(MapGuiLayer::_MenuInputListener, this));
		_btnEditor = cocos2d::CCMenuItemImage::create("Btn_Edit.png", "Btn_Edit_Pressed.png",
			CC_CALLBACK_1(MapGuiLayer::_MenuInputListener, this));
		_btnMenu = cocos2d::CCMenuItemImage::create("Btn_Menu.png", "Btn_Menu_Pressed.png",
			CC_CALLBACK_1(MapGuiLayer::_MenuInputListener, this));
	}
	
	cocos2d::Director *director = cocos2d::Director::getInstance();
	Vector2 screen = director->getVisibleSize();
	Vector2 origin = director->getVisibleOrigin();

	cocos2d::Vector<cocos2d::MenuItem*> menuItems;

#if CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID && CC_TARGET_PLATFORM != CC_PLATFORM_IOS
	Vector2 pos(0.0f, origin.y + screen.y);

	_btnMenu->setScale(1.0f);
	_btnMenu->setTag(MENU_ITEM_MENU);
	_btnMenu->setPosition(pos + Vector2(0.0f, -68.0f));
	_btnMenu->setAnchorPoint(cocos2d::Vec2(0.0f, 1.0f));
	menuItems.pushBack(_btnMenu);

	_btnEditor->setScale(1.0f);
	_btnEditor->setTag(MENU_ITEM_EDITOR);
	_btnEditor->setPosition(pos);
	_btnEditor->setAnchorPoint(cocos2d::Vec2(0.0f, 1.0f));
	menuItems.pushBack(_btnEditor);

	pos += Vector2(_btnEditor->getContentSize().width - 7.5f, 0.0f);
	
	_btnZoomOut->setScale(1.0f);
	_btnZoomOut->setTag(MENU_ITEM_ZOOM_OUT);
	_btnZoomOut->setPosition(pos);
	_btnZoomOut->setAnchorPoint(cocos2d::Vec2(0.0f, 1.0f));
	menuItems.pushBack(_btnZoomOut);
	
	pos += Vector2(_btnZoomOut->getContentSize().width - 7.5f, 0.0f);
	
	_btnZoomIn->setScale(1.0f);
	_btnZoomIn->setTag(MENU_ITEM_ZOOM_IN);
	_btnZoomIn->setPosition(pos);
	_btnZoomIn->setAnchorPoint(cocos2d::Vec2(0.0f, 1.0f));
	menuItems.pushBack(_btnZoomIn);
#endif

	cocos2d::Menu *menu = cocos2d::Menu::createWithArray(menuItems);
	menu->setPosition(0.0f, 0.0f);

	addChild(menu);

	float indentX = 20.0f;
	_worldCaptureProgressBar = new SquareProgressBar(origin.x + screen.x - indentX * 2,
													 20.0f,
													 cocos2d::Color4F(1.0f, 0.5f, 0, 1.0f));
	_worldCaptureProgressBar->setPosition(indentX, 0);
	_worldCaptureProgressBar->autorelease();
	_worldCaptureProgressBar->SetProgressPercentage(World::Instance().GetWorldCapturingState() * 100.0f);
	addChild(_worldCaptureProgressBar);

	scheduleUpdate();

	return true;
}

void MapGuiLayer::_MenuInputListener(cocos2d::Ref *sender)
{
	cocos2d::MenuItemImage *item = dynamic_cast<cocos2d::MenuItemImage*>(sender);

	dynamic_cast<WorldMapLayer*>(getParent())->HideCellGameInterface();

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
	case MENU_ITEM_MENU:
		ToggleCellsNetMenu();
		break;
	default: break;
	}
}

void MapGuiLayer::ToggleCellsNetMenu()
{
	if (!_cellsNetLayer)
	{
		_cellsNetLayer = new CellsNetLayer();
		addChild(_cellsNetLayer);
		_cellsNetLayer->autorelease();
		dynamic_cast<WorldMapLayer*>(getParent())->SetMapInputEnabled(false);
	}
	else
	{
		removeChild(_cellsNetLayer);
		dynamic_cast<WorldMapLayer*>(getParent())->SetMapInputEnabled(true);
		_cellsNetLayer = nullptr;
	}
}

void MapGuiLayer::update(float delta)
{
	_worldCaptureProgressBar->SetProgressPercentage(World::Instance().GetWorldCapturingState() * 100.0f);
}
