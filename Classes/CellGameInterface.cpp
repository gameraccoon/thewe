#include "CellGameInterface.h"

CellMenuSelector::CellMenuSelector(void)
	: _isDisappearing(false)
{
	init();
}

bool CellMenuSelector::init()
{
	if (!cocos2d::Layer::init())
	{
		return false;
	}

	{
		using namespace cocos2d;

		_button.resize(CELL_NUM_TAGS);
		_button[CELL_OPEN_TASKS] = MenuItemImage::create("1_norm.png", "1_press.png", CC_CALLBACK_1(CellMenuSelector::_MenuInputListener, this));
		_button[CELL_OPEN_INFO] = MenuItemImage::create("2_norm.png", "2_press.png", CC_CALLBACK_1(CellMenuSelector::_MenuInputListener, this));
		_button[CELL_OPEN_SPINOFF] = MenuItemImage::create("2_norm.png", "2_press.png", CC_CALLBACK_1(CellMenuSelector::_MenuInputListener, this));

		_button[CELL_OPEN_TASKS]->setTag(CELL_OPEN_TASKS);
		_button[CELL_OPEN_INFO]->setTag(CELL_OPEN_INFO);
		_button[CELL_OPEN_SPINOFF]->setTag(CELL_OPEN_SPINOFF);
	}

	cocos2d::Menu *menu = cocos2d::Menu::create(_button[CELL_OPEN_TASKS],
		_button[CELL_OPEN_INFO], _button[CELL_OPEN_SPINOFF], nullptr);
	menu->setPosition(0.0f, 0.0f);

	addChild(menu, 0);
	scheduleUpdate();
	setVisible(false);

	return true;
}

void CellMenuSelector::update(float dt)
{
	if (_IsAnimationFinished() && _isDisappearing)
	{
		setVisible(false);
		_isDisappearing = false;
	}
}

bool CellMenuSelector::IsCursorOnMenu(const Vector2 &cursorPos) const
{
	for (ButtonsList::const_iterator it = _button.begin(); it != _button.end(); ++it)
	{
		if ((*it)->getBoundingBox().containsPoint(cursorPos))
		{
			return true;
		}
	}

	return false;
}
	
void CellMenuSelector::AppearImmediately(Cell::WeakPtr cell, const Vector2 &position)
{
	_cell = cell;
	_position = position;

	setVisible(true);
}

void CellMenuSelector::AppearWithAnimation(Cell::WeakPtr cell, const Vector2 &position)
{
	if (!_IsAnimationFinished())
	{
		return;
	}

	_cell = cell;
	_position = position;

	cocos2d::Vec2 dir(0.0f, 1.0f);
	const float dist = 45.0f;
	const float angle = 2.0f * 3.14159265f / (float)CELL_NUM_TAGS;

	for (ButtonsList::iterator it = _button.begin(); it != _button.end(); ++it)
	{
		cocos2d::MenuItemImage *item = (*it);

		// поворачиваем все векторы, кроме первого
		if (it != _button.begin())
		{
			dir = dir.rotateByAngle(cocos2d::Vec2::ZERO, angle);
			dir.normalize();
		}

		item->stopAllActions();
		_PrepearButtonToAppear(item, _position + dir * dist);
	}

	setVisible(true);
}

void CellMenuSelector::DisappearImmedaitely(void)
{
	for (ButtonsList::iterator it = _button.begin(); it != _button.end(); ++it)
	{
		(*it)->stopAllActions();
	}

	setVisible(false);
}

void CellMenuSelector::DisappearWithAnimation(void)
{
	for (ButtonsList::iterator it = _button.begin(); it != _button.end(); ++it)
	{
		cocos2d::MenuItemImage *item = (*it);

		item->stopAllActions();
		_PrepearButtonToDisappear(item);
	}

	_isDisappearing = true;
}

void CellMenuSelector::_PrepearButtonToAppear(cocos2d::MenuItemImage *item, Vector2 pos)
{
	if (!item)
	{
		return;
	}

	item->setPosition(_position);
	item->setScale(0.4f);
	item->setOpacity(0);

	cocos2d::MoveTo *move = cocos2d::MoveTo::create(1.0f, pos);
	cocos2d::ScaleTo *scale = cocos2d::ScaleTo::create(1.0f, 0.7f, 0.7f);
	cocos2d::FadeIn *fade = cocos2d::FadeIn::create(0.25f);

	cocos2d::EaseElasticOut *ease_move = cocos2d::EaseElasticOut::create(move, 0.35f);
	cocos2d::EaseElasticOut *ease_scale = cocos2d::EaseElasticOut::create(scale, 1.0f);

	item->runAction(ease_move);
	item->runAction(ease_scale);
	item->runAction(fade);
}

void CellMenuSelector::_PrepearButtonToDisappear(cocos2d::MenuItemImage *item)
{
	if (!item)
	{
		return;
	}
	
	cocos2d::MoveTo *move = cocos2d::MoveTo::create(0.35f, _position);
	cocos2d::ScaleTo *scale = cocos2d::ScaleTo::create(0.35f, 0.4f, 0.4f);
	cocos2d::FadeOut *fade = cocos2d::FadeOut::create(0.25f);

	cocos2d::EaseBackInOut *ease_move = cocos2d::EaseBackInOut::create(move);
	cocos2d::EaseBackInOut *ease_scale = cocos2d::EaseBackInOut::create(scale);

	item->runAction(ease_move);
	item->runAction(ease_scale);
	item->runAction(fade);
}

void CellMenuSelector::_MenuInputListener(cocos2d::Ref *sender)
{
	cocos2d::Ref *ref = sender;
}

bool CellMenuSelector::_IsAnimationFinished(void)
{
	for (ButtonsList::iterator it = _button.begin(); it != _button.end(); ++it)
	{
		if ((*it)->getNumberOfRunningActions() > 0)
		{
			return false;
		}
	}

	return true;
}