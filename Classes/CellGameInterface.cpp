#include "CellGameInterface.h"

#include "WorldMapLayer.h"
#include "CellTasksMenu.h"
#include "CellTaskInfoMenu.h"
#include "CellInfoMenu.h"
#include "CellSpinoffMenu.h"

CellMenuSelector::CellMenuSelector(MapProjector *proj, WorldMapLayer *map)
	: _projector(proj)
	, _worldMapLayer(map)
	, _isDisappearing(false)
	, _menuNodeName("CellMenu")
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
		_button[CELL_OPEN_TASKS] = MenuItemImage::create("3_norm.png", "3_press.png", CC_CALLBACK_1(CellMenuSelector::_MenuInputListener, this));
		_button[CELL_OPEN_INFO] = MenuItemImage::create("2_norm.png", "2_press.png", CC_CALLBACK_1(CellMenuSelector::_MenuInputListener, this));
		_button[CELL_OPEN_SPINOFF] = MenuItemImage::create("1_norm.png", "1_press.png", CC_CALLBACK_1(CellMenuSelector::_MenuInputListener, this));

		_button[CELL_OPEN_TASKS]->setTag(CELL_OPEN_TASKS);
		_button[CELL_OPEN_INFO]->setTag(CELL_OPEN_INFO);
		_button[CELL_OPEN_SPINOFF]->setTag(CELL_OPEN_SPINOFF);
	}
	
	_menu = cocos2d::Menu::create(_button[CELL_OPEN_TASKS],
		_button[CELL_OPEN_INFO], _button[CELL_OPEN_SPINOFF], nullptr);
	_menu->setPosition(_position);

	addChild(_menu, 0);
	scheduleUpdate();
	setVisible(false);

	return true;
}

void CellMenuSelector::update(float dt)
{
	if (_IsAnimationFinished() && _isDisappearing)
	{
		OnCellMenuClosed();
		setVisible(false);
		_isDisappearing = false;
	}

	if (isVisible())
	{
		Vector2 initialPos = _cell.lock()->GetInfo().location;
		Vector2 updatedPos = _projector->ProjectOnScreen(initialPos);

		_menu->setPosition(updatedPos);
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
	_isDisappearing = false;

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
	_isDisappearing = false;

	_menu->setPosition(_position);

	cocos2d::Vec2 dir(0.0f, 1.0f);
	const float dist = 45.0f;
	const float angle = 2.0f * 3.14159265f / (float)CELL_NUM_TAGS;

	for (ButtonsList::iterator it = _button.begin(); it != _button.end(); ++it)
	{
		cocos2d::MenuItemImage *item = (*it);

		if (it != _button.begin())
		{
			dir = dir.rotateByAngle(cocos2d::Vec2::ZERO, angle);
			dir.normalize();
		}

		item->stopAllActions();
		_PrepearButtonToAppear(item, dir * dist);
	}

	setVisible(true);
}

void CellMenuSelector::DisappearImmedaitely(void)
{
	for (ButtonsList::iterator it = _button.begin(); it != _button.end(); ++it)
	{
		(*it)->stopAllActions();
	}

	OnCellMenuClosed();

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

void CellMenuSelector::OnCellMenuClosed(void)
{
	cocos2d::Node *menu = getChildByName(_menuNodeName);

	if (menu)
	{
		removeChild(menu, true);
		//DisappearWithAnimation();

		_worldMapLayer->SetGuiEnabled(true);
		_worldMapLayer->SetMapInputEnabled(true);
	}
}

void CellMenuSelector::_PrepearButtonToAppear(cocos2d::MenuItemImage *item, Vector2 pos)
{
	if (!item)
	{
		assert(false);
		return;
	}

	item->setPosition(cocos2d::Vec2::ZERO);
	item->setScale(0.4f);
	item->setOpacity(0);

	cocos2d::MoveTo *move = cocos2d::MoveTo::create(1.0f, pos);
	cocos2d::ScaleTo *scale = cocos2d::ScaleTo::create(1.0f, 0.85f, 0.85f);
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

void CellMenuSelector::_MenuInputListener(cocos2d::Ref *sender)
{
	if (getChildByName(_menuNodeName))
	{
		return;
	}

	if (_cell.lock()->GetInfo().state != Cell::READY)
	{
		DisappearWithAnimation();
		return;
	}

	cocos2d::MenuItemImage *item = dynamic_cast<cocos2d::MenuItemImage *>(sender);
	cocos2d::Layer *menu = nullptr;

	CELL_MENU_TAGS tag = (CELL_MENU_TAGS)item->getTag();

	switch (tag)
	{
	case CELL_OPEN_TASKS:
		if (_cell.lock()->IsCurrentTaskExists())
			menu = new CellTaskInfoMenu(_cell, this);
		else
			menu = new CellTasksScreen(_cell, this);
		menu->autorelease();
		menu->setName(_menuNodeName);
		addChild(menu);
		break;
	case CELL_OPEN_INFO:
		menu = new CellInfoMenu(_cell, this);
		menu->autorelease();
		menu->setName(_menuNodeName);
		addChild(menu);
		break;
	case CELL_OPEN_SPINOFF:
		menu = new CellSpinoffMenu(_cell, this, _worldMapLayer);
		menu->autorelease();
		menu->setName(_menuNodeName);
		addChild(menu);
		break;
	default: break;
	};

	if (menu)
	{
		_worldMapLayer->SetGuiEnabled(false);
		_worldMapLayer->SetMapInputEnabled(false);
	}
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
