#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <cocos2d.h>
#include <cocostudio/CCSGUIReader.h>
#include <ui/CocosGUI.h>

#include "Vector2.h"

class MainMenuScene : public cocos2d::Scene
{
public:
	MainMenuScene(cocos2d::Scene* gameScene);
	virtual ~MainMenuScene();

	virtual bool init() override;

private:
	cocos2d::ui::Layout *_widget;

private:
	void MenuInputListener(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType eventType);
	cocos2d::Scene* _gameScene;
};

#endif // MAIN_MENU_H
