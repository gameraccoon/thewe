#include "MapGuiLayer.h"

#include "World.h"
#include "WorldLoader.h"
#include "GameScene.h"
#include "WorldMapLayer.h"
#include "CellsNetLayer.h"
#include "Log.h"

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
	
	cocos2d::Director *director = cocos2d::Director::getInstance();
	Vector2 screen = director->getVisibleSize();
	Vector2 origin = director->getVisibleOrigin();

	_widget = dynamic_cast<cocos2d::ui::Layout *>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("GameHud/GameHud.json"));
	_widget->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
	_widget->setPosition(origin + screen / 2.0f);
	
	cocos2d::Node *progBarParent = _widget->getChildByName("WorldCapturingState");
	_worldCapturingBar = dynamic_cast<cocos2d::ui::LoadingBar *>(progBarParent->getChildByName("Progress"));

	cocos2d::ui::Button *btnEdit = dynamic_cast<cocos2d::ui::Button *>(_widget->getChildByName("BtnEdit"));
	cocos2d::ui::Button *btnMenu = dynamic_cast<cocos2d::ui::Button *>(_widget->getChildByName("BtnMenu"));
	cocos2d::ui::Button *btnZoomOut = dynamic_cast<cocos2d::ui::Button *>(_widget->getChildByName("BtnZoomOut"));
	cocos2d::ui::Button *btnZoomIn = dynamic_cast<cocos2d::ui::Button *>(_widget->getChildByName("BtnZoomIn"));

	if (!btnEdit) {Log::Instance().writeWarning("GameHud: Failed to get widget with name BtnEdit"); return false;}
	if (!btnMenu) {Log::Instance().writeWarning("GameHud: Failed to get widget with name BtnMenu"); return false;}
	if (!btnZoomOut) {Log::Instance().writeWarning("GameHud: Failed to get widget with name BtnZoomOut"); return false;}
	if (!btnZoomIn) {Log::Instance().writeWarning("GameHud: Failed to get widget with name BtnZoomIn"); return false;}
	if (!_worldCapturingBar) {Log::Instance().writeWarning("GameHud: Failed to get widget with name Progress"); return false;}

	btnEdit->addTouchEventListener(CC_CALLBACK_2(MapGuiLayer::MenuInputListener, this));
	btnMenu->addTouchEventListener(CC_CALLBACK_2(MapGuiLayer::MenuInputListener, this));
	btnZoomOut->addTouchEventListener(CC_CALLBACK_2(MapGuiLayer::MenuInputListener, this));
	btnZoomIn->addTouchEventListener(CC_CALLBACK_2(MapGuiLayer::MenuInputListener, this));

	_worldCapturingBar->setPercent(0.0f);

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	btnEdit->setVisible(false);
	btnMenu->setVisible(false);
	btnZoomOut->setVisible(false);
	btnZoomIn->setVisible(false);
#endif

	addChild(_widget);
	scheduleUpdate();

	return true;
}

void MapGuiLayer::MenuInputListener(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType eventType)
{	
	if (eventType == cocos2d::ui::Widget::TouchEventType::BEGAN)
	{
		dynamic_cast<WorldMapLayer*>(getParent())->HideCellGameInterface();
	}
	else if (eventType == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		std::string name = dynamic_cast<cocos2d::ui::Button *>(sender)->getName();

		if (name == "BtnZoomIn") {
			dynamic_cast<WorldMapLayer*>(getParent())->ModifyZoom(1.25f);
		} else if (name == "BtnZoomOut") {
			dynamic_cast<WorldMapLayer*>(getParent())->ModifyZoom(0.8f);
		} else if (name == "BtnEdit") {
			dynamic_cast<GameScene*>(getParent()->getParent())->ToggleEditor();
		} else if (name == "BtnMenu") {
			ToggleCellsNetMenu();
		} else {
			Log::Instance().writeWarning("Game HUD input listener get unknown widget.");
		}
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
	_worldCapturingBar->setPercent(World::Instance().GetWorldCapturingState() * 100.0f);
}
