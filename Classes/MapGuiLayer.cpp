#include "MapGuiLayer.h"

#include <math.h>

#include "World.h"
#include "WorldLoader.h"
#include "GameScene.h"
#include "WorldMapLayer.h"
#include "MessageManager.h"
#include "CellsNetLayer.h"

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
		_btnMenu = cocos2d::CCMenuItemImage::create("Btn_Menu.png", "Btn_Menu_Pressed.png",
			this, menu_selector(MapGuiLayer::_MenuInputListener));
	}
	
	cocos2d::Director *director = cocos2d::Director::getInstance();
	Vector2 screen = director->getVisibleSize();
	Vector2 origin = director->getVisibleOrigin();\
	_messagesMargin = Vector2(5.0f, 5.0f);
	_messagesPosition = Vector2(origin + screen - _messagesMargin);

	cocos2d::Vector<cocos2d::MenuItem*> menuItems;

	Vector2 pos(0.0f, origin.y + screen.y);

	_btnMenu->setScale(1.0f);
	_btnMenu->setTag(MENU_ITEM_MENU);
	_btnMenu->setPosition(pos + Vector2(0.0f, -68.0f));
	_btnMenu->setAnchorPoint(cocos2d::Vec2(0.0f, 1.0f));
	menuItems.pushBack(_btnMenu);

	_btnSave->setScale(1.0f);
	_btnSave->setTag(MENU_ITEM_SAVE);
	_btnSave->setPosition(pos);
	_btnSave->setAnchorPoint(cocos2d::Vec2(0.0f, 1.0f));
	menuItems.pushBack(_btnSave);

#if CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID && CC_TARGET_PLATFORM != CC_PLATFORM_IOS
	pos += Vector2(_btnSave->getContentSize().width - 8.0f, 0.0f);

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
		{
			cocos2d::Layer* cellsNetLayer = new CellsNetLayer();
			addChild(cellsNetLayer);
			cellsNetLayer->autorelease();
			dynamic_cast<WorldMapLayer*>(getParent())->SetMapInputEnabled(false);
		}
		break;
	default: break;
	}
}

void MapGuiLayer::update(float delta)
{
	int changed = UpdateMessages();
	if (changed > 0)
	{
		UpdateMessagesPos();
	}

	_worldCaptureProgressBar->SetProgressPercentage(World::Instance().GetWorldCapturingState() * 100.0f);
}

int MapGuiLayer::UpdateMessages()
{
	std::set<int> visibleKeys;
	for (auto message : _userMessages)
	{
		int key = message.first;

		visibleKeys.insert(key);

		if (message.second->IsOutdated())
		{
			MessageManager::Instance().RemoveMessage(key);
		}
	}

	const std::map<int, UserMessage::Ptr> messages = MessageManager::Instance().GetMessages();
	std::set<int> realKeys;
	for (auto message : messages)
	{
		realKeys.insert(message.first);
	}

	int changed = 0;
	std::set<int> newKeys;
	set_difference(realKeys.begin(), realKeys.end(),
				   visibleKeys.begin(), visibleKeys.end(),
				   inserter(newKeys, newKeys.begin()));

	if (newKeys.size() > 0)
	{
		for (int key : newKeys)
		{
			AddNewMessage(key, messages);
			changed++;
		}
	}

	std::set<int> keysToRemove;
	set_difference(visibleKeys.begin(), visibleKeys.end(),
				   realKeys.begin(), realKeys.end(),
				   inserter(keysToRemove, keysToRemove.begin()));

	if (keysToRemove.size() > 0)
	{
		for (int key : keysToRemove)
		{
			removeChild(_userMessages.at(key));
			_userMessages.erase(key);

			changed++;
		}
	}

	return changed;
}

void MapGuiLayer::UpdateMessagesPos()
{
	Vector2 position = _messagesPosition;

	for (auto message : _userMessages)
	{
		cocos2d::MoveTo *move = cocos2d::MoveTo::create(0.3f, position);
		message.second->stopAllActions();
		message.second->runAction(move);
		position.y += -message.second->getContentSize().height - _messagesMargin.y;
	}
}

void MapGuiLayer::AddNewMessage(int key, const std::map<int, UserMessage::Ptr>& messages)
{
	UserMessage::Ptr message = messages.at(key);
	MessageWidget* messageWidget = MessageWidget::create(message);

	Vector2 position = _messagesPosition;

	position.y += _userMessages.size() * (-messageWidget->getContentSize().height - _messagesMargin.y);

	messageWidget->setPosition(position);
	addChild(messageWidget);
	_userMessages.insert(std::pair<int, MessageWidget*>(key, messageWidget));
}
