#include "MainMenuScene.h"

#include "Color.h"
#include "Vector2.h"
#include "Log.h"
#include "GameScene.h"
#include "TransitionZoomFade.h"
#include "CellsNetLayer.h"

MainMenuScene::MainMenuScene()
{
}

MainMenuScene::~MainMenuScene(void)
{
	WRITE_LOG("Main menu unloaded sucessfully");
}

bool MainMenuScene::init(void)
{
	if (!cocos2d::CCScene::init())
	{
		return false;
	}

	cocos2d::Director *director = cocos2d::Director::getInstance();

	Vector2 client = director->getVisibleSize();
	Vector2 origin = director->getVisibleOrigin();
	Vector2 center(origin.x + client.x / 2.0f, origin.y + client.y - 100.0f);

	_widget = dynamic_cast<cocos2d::ui::Layout *>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("ui_main_menu/ui_main_menu.ExportJson"));
	_widget->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
	_widget->setPosition(origin + client / 2.0f);

	cocos2d::ui::Button *btnMap = dynamic_cast<cocos2d::ui::Button *>(_widget->getChildByName("BtnMap"));
	cocos2d::ui::Button *btnMail = dynamic_cast<cocos2d::ui::Button *>(_widget->getChildByName("BtnMail"));
	cocos2d::ui::Button *btnChat = dynamic_cast<cocos2d::ui::Button *>(_widget->getChildByName("BtnChat"));
	cocos2d::ui::Button *btnCommand = dynamic_cast<cocos2d::ui::Button *>(_widget->getChildByName("BtnCommand"));
	cocos2d::ui::Button *btnSettings = dynamic_cast<cocos2d::ui::Button *>(_widget->getChildByName("BtnSettings"));

	if (!btnMap) {WRITE_WARN("MainMenu: Failed to get BtnMap widget."); return false;}
	if (!btnMail) {WRITE_WARN("MainMenu: Failed to get BtnMail widget."); return false;}
	if (!btnChat) {WRITE_WARN("MainMenu: Failed to get BtnChat widget."); return false;}
	if (!btnCommand) {WRITE_WARN("MainMenu: Failed to get BtnCommand widget."); return false;}
	if (!btnSettings) {WRITE_WARN("MainMenu: Failed to get BtnSettings widget."); return false;}

	btnMap->addTouchEventListener(CC_CALLBACK_2(MainMenuScene::MenuInputListener, this));
	btnMail->addTouchEventListener(CC_CALLBACK_2(MainMenuScene::MenuInputListener, this));
	btnChat->addTouchEventListener(CC_CALLBACK_2(MainMenuScene::MenuInputListener, this));
	btnCommand->addTouchEventListener(CC_CALLBACK_2(MainMenuScene::MenuInputListener, this));
	btnSettings->addTouchEventListener(CC_CALLBACK_2(MainMenuScene::MenuInputListener, this));

	addChild(_widget);

	return true;
}

void MainMenuScene::MenuInputListener(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType eventType)
{
	if (eventType == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		std::string name = dynamic_cast<cocos2d::ui::Button *>(sender)->getName();

		if (name == "BtnMap") {
			GameScene* scene = new GameScene(this);
			scene->autorelease();
			scene->init();
			cocos2d::TransitionScene* transition = TransitionZoomFade::create(1, scene);
			cocos2d::Director::getInstance()->replaceScene(transition);
		} else if (name == "BtnMail") {
		} else if (name == "BtnChat") {
		} else if (name == "BtnCommand") {
		} else if (name == "BtnSettings") {
		} else {
			Log::Instance().writeWarning("Main Menu input listener get unknown widget.");
		}
	}
}
