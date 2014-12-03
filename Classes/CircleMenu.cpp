#include "CircleMenu.h"

CircleMenu::CircleMenu()
	: _animationFinishCallback(nullptr)
{
}

CircleMenu::~CircleMenu()
{
	if (_menu)
	{
		removeChild(_menu);
	}
}

CircleMenu* CircleMenu::create()
{
	CircleMenu* ret = new CircleMenu();
	if (ret && ret->init())
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

bool CircleMenu::init()
{
	if (!cocos2d::Node::init())
	{
		return false;
	}

	setVisible(false);

	scheduleUpdate();

	return true;
}

void CircleMenu::update(float)
{
	if (IsAnimationFinished())
	{
		if (_animationFinishCallback)
		{
			_animationFinishCallback();
		}
	}
}

void CircleMenu::AddNonMenuItem(cocos2d::Node* item)
{
	_items.pushBack(item);
	addChild(item);
}

void CircleMenu::AddMenuItem(cocos2d::MenuItem* item)
{
	_items.pushBack(item);
	_menuItems.pushBack(item);
}
	
void CircleMenu::AppearImmediately()
{
	InitMenuItems();

	setVisible(true);
}

void CircleMenu::AppearWithAnimation()
{
	if (!IsAnimationFinished())
	{
		return;
	}

	_menu->setPosition(cocos2d::Vec2(0.0f, 0.0f));

	int numButtonsToShow = _items.size();

	cocos2d::Vec2 dir(0.0f, 1.0f);
	const float dist = 45.0f;
	const float angle = 3.14159265f*2.0f / (float)numButtonsToShow;
	
	for (Items::iterator it = _items.begin(); it != _items.end(); ++it)
	{
		cocos2d::Node *item = (*it);

		if (it != _items.begin())
		{
			dir = dir.rotateByAngle(cocos2d::Vec2::ZERO, angle);
			dir.normalize();
		}

		item->stopAllActions();
		PrepareButtonToAppear(item, dir * dist);
	}

	setVisible(true);
}

void CircleMenu::DisappearImmedaitely(void)
{
	for (Items::iterator it = _items.begin(); it != _items.end(); ++it)
	{
		(*it)->stopAllActions();
	}

	setVisible(false);
}

void CircleMenu::DisappearWithAnimation(std::function<void ()> finishCallback)
{
	for (Items::iterator it = _items.begin(); it != _items.end(); ++it)
	{
		cocos2d::Node *item = (*it);

		item->stopAllActions();
		PrepareButtonToDisappear(item);
	}

	_animationFinishCallback = finishCallback;
}

void CircleMenu::InitMenuItems()
{
	_menu = cocos2d::Menu::createWithArray(_menuItems);
	_menu->setPosition(cocos2d::Vec2::ZERO);
	addChild(_menu, 0);

	_menuItems.clear();
}

void CircleMenu::PrepareButtonToAppear(cocos2d::Node *item, Vector2 pos)
{
	if (!item)
	{
		assert(false);
		return;
	}

	float initialScaleX = item->getScaleX();
	float initialScaleY = item->getScaleY();

	item->setPosition(cocos2d::Vec2::ZERO);
	item->setScale(0.4f);
	item->setOpacity(0);
	
	cocos2d::MoveTo *move = cocos2d::MoveTo::create(1.0f, pos);
	cocos2d::ScaleTo *scale = cocos2d::ScaleTo::create(1.0f, initialScaleX, initialScaleY);
	cocos2d::FadeIn *fade = cocos2d::FadeIn::create(0.25f);

	cocos2d::EaseElasticOut *ease_move = cocos2d::EaseElasticOut::create(move, 0.35f);
	cocos2d::EaseElasticOut *ease_scale = cocos2d::EaseElasticOut::create(scale, 1.0f);

	item->runAction(ease_move);
	item->runAction(ease_scale);
	item->runAction(fade);
}

void CircleMenu::PrepareButtonToDisappear(cocos2d::Node* item)
{
	if (!item)
	{
		assert(false);
		return;
	}
	
	cocos2d::MoveTo *move = cocos2d::MoveTo::create(0.35f, cocos2d::Vec2::ZERO);
	cocos2d::ScaleTo *scale = cocos2d::ScaleTo::create(0.35f, 0.4f, 0.4f);
	cocos2d::FadeOut *fade = cocos2d::FadeOut::create(0.25f);

	cocos2d::EaseBackInOut *ease_move = cocos2d::EaseBackInOut::create(move);
	cocos2d::EaseBackInOut *ease_scale = cocos2d::EaseBackInOut::create(scale);

	item->runAction(ease_move);
	item->runAction(ease_scale);
	item->runAction(fade);
}

bool CircleMenu::IsAnimationFinished(void)
{
	for (Items::iterator it = _items.begin(); it != _items.end(); ++it)
	{
		if ((*it)->getNumberOfRunningActions() > 0)
		{
			return false;
		}
	}

	return true;
}
