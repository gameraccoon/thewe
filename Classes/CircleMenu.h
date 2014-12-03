#ifndef CIRCLE_MENU_H
#define CIRCLE_MENU_H

#include <cocos2d.h>
#include <cocos-ext.h>
#include <cocostudio/CCSGUIReader.h>
#include <ui/CocosGUI.h>

#include <functional>

#include "Vector2.h"

class CircleMenu : public cocos2d::Node
{
public:
	CircleMenu();
	~CircleMenu();

	static CircleMenu* create();

	virtual bool init() override;
	virtual void update(float deltatime) override;

	void AddNonMenuItem(cocos2d::Node* item);
	void AddMenuItem(cocos2d::MenuItem* item);
	
	void AppearImmediately();
	void AppearWithAnimation();
	void DisappearImmedaitely(void);
	void DisappearWithAnimation(std::function<void()> finishCallback);
	void InitMenuItems();
	bool IsDisappearing() const;
	bool IsAnimationFinished(void);

private:
	typedef cocos2d::Vector<cocos2d::Node*> Items;
	typedef cocos2d::Vector<cocos2d::MenuItem*> MenuItems;

private:
	void PrepareButtonToAppear(cocos2d::Node* item, Vector2 pos);
	void PrepareButtonToDisappear(cocos2d::Node* item);

private:
	cocos2d::Menu *_menu;

	Items _items;
	MenuItems _menuItems;

	std::function<void()> _animationFinishCallback;
};

#endif
