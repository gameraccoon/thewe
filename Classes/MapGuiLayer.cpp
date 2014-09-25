#include "MapGuiLayer.h"

#include <math.h>

#include "World.h"
#include "WorldLoader.h"
#include "GameScene.h"
#include "WorldMapLayer.h"
#include "MessageManager.h"

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
	}
	
	cocos2d::Director *director = cocos2d::Director::getInstance();
	Vector2 screen = director->getVisibleSize();
	Vector2 origin = director->getVisibleOrigin();\
	_messagesMargin = Vector2(5.0f, 5.0f);
	_messagesPosition = Vector2(origin + screen - _messagesMargin);

	Vector2 pos(0.0f, origin.y + screen.y);

	_printTime = cocos2d::LabelTTF::create("Time: 0", "Arial", 32);
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

	cocos2d::Menu *menu = cocos2d::Menu::create(_btnZoomIn, _btnZoomOut, _btnEditor, _btnSave, NULL);
	menu->setPosition(0.0f, 0.0f);

	addChild(menu);
	addChild(_printTime);
	
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
	case MENU_ITEM_PIN:
		dynamic_cast<WorldMapLayer*>(getParent())->ModifyZoom(1.25f);
		break;
	case MENU_ITEM_SAVE:
		WorldLoader::SaveGameState();
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
