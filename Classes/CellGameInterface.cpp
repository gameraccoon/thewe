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
}

CellMenuSelector* CellMenuSelector::create(MapProjector* proj, WorldMapLayer* map)
{
	CellMenuSelector* ret = new CellMenuSelector(proj, map);
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

bool CellMenuSelector::init()
{
	if (!cocos2d::Layer::init())
	{
		return false;
	}

	scheduleUpdate();
	setVisible(false);

	return true;
}

void CellMenuSelector::update(float dt)
{
	if (_IsAnimationFinished() && _isDisappearing)
	{
		OnMenuClosed();
		setVisible(false);
		_isDisappearing = false;
	}

	if (isVisible())
	{
		if (!_cell.expired())
		{
			Vector2 initialPos = _cell.lock()->GetInfo().location;
			Vector2 updatedPos = _projector->ProjectOnScreen(initialPos);

			_menu->setPosition(updatedPos);
		}
	}
}

bool CellMenuSelector::IsCursorOnMenu(const Vector2 &cursorPos) const
{
	for (Buttons::const_iterator it = _buttons.begin(); it != _buttons.end(); ++it)
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
	InitButtons(cell.lock());

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

	InitButtons(cell.lock());

	_cell = cell;
	_position = position;
	_isDisappearing = false;
	_menu->setPosition(cocos2d::Vec2(0.0f, 0.0f));

	int numButtonsToShow = _buttons.size();

	cocos2d::Vec2 dir(0.0f, 1.0f);
	const float dist = 45.0f;
	const float angle = 3.14159265f*2.0f / (float)numButtonsToShow;
	
	for (Buttons::iterator it = _buttons.begin(); it != _buttons.end(); ++it)
	{
		cocos2d::MenuItem *item = (*it);

		if (it != _buttons.begin())
		{
			dir = dir.rotateByAngle(cocos2d::Vec2::ZERO, angle);
			dir.normalize();
		}

		item->stopAllActions();
		_PrepareButtonToAppear(item, dir * dist);
	}

	setVisible(true);
}

void CellMenuSelector::DisappearImmedaitely(void)
{
	for (Buttons::iterator it = _buttons.begin(); it != _buttons.end(); ++it)
	{
		(*it)->stopAllActions();
	}

	OnMenuClosed();

	setVisible(false);
}

void CellMenuSelector::DisappearWithAnimation(void)
{
	for (Buttons::iterator it = _buttons.begin(); it != _buttons.end(); ++it)
	{
		cocos2d::MenuItem *item = (*it);

		item->stopAllActions();
		_PrepareButtonToDisappear(item);
	}

	_isDisappearing = true;
}

void CellMenuSelector::InitButtons(Cell::Ptr cell)
{
	if (_menu)
	{
		removeChild(_menu);
	}

	_buttons.clear();

	bool mustShowKillButton = World::Instance().IsCellUnderInvestigation(cell) || cell->IsState(Cell::State::AUTONOMY);

	using namespace cocos2d;
	if (mustShowKillButton)
	{
		cocos2d::MenuItemImage *btn;
		btn = MenuItemImage::create("marker_crosshair.png", "marker_crosshair_pressed.png", CC_CALLBACK_1(CellMenuSelector::OnKillButtonPressed, this));
		btn->setScale(1.3f, 1.3f);

		_buttons.pushBack(btn);
	}

	if (!cell->IsState(Cell::State::AUTONOMY))
	{
		cocos2d::MenuItemImage *btn1, *btn2, *btn3;

		btn1 = MenuItemImage::create("1_norm.png", "1_press.png", CC_CALLBACK_1(CellMenuSelector::OnSpinoffButtonPressed, this));
		btn1->setScale(0.85f, 0.85f);
		btn2 = MenuItemImage::create("2_norm.png", "2_press.png", CC_CALLBACK_1(CellMenuSelector::OnCellInfoButtonPressed, this));
		btn2->setScale(0.85f, 0.85f);
		btn3 = MenuItemImage::create("3_norm.png", "3_press.png", CC_CALLBACK_1(CellMenuSelector::OnTasksButtonPressed, this));
		btn3->setScale(0.85f, 0.85f);

		_buttons.pushBack(btn1);
		_buttons.pushBack(btn2);
		_buttons.pushBack(btn3);
	}

	_menu = cocos2d::Menu::createWithArray(_buttons);
	_menu->setPosition(_position);
	addChild(_menu, 0);
}

void CellMenuSelector::OnMenuClosed(void)
{
	cocos2d::Node *menu = getChildByName(_menuNodeName);
	if (menu)
	{
		removeChild(menu, true);

		_worldMapLayer->SetGuiEnabled(true);
		_worldMapLayer->SetMapInputEnabled(true);
	}
}

void CellMenuSelector::_PrepareButtonToAppear(cocos2d::MenuItem *item, Vector2 pos)
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

void CellMenuSelector::_PrepareButtonToDisappear(cocos2d::MenuItem *item)
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

void CellMenuSelector::CreateMenu(cocos2d::Layer* menu)
{
	if (_cell.lock()->GetInfo().state != Cell::State::READY)
	{
		DisappearWithAnimation();
		return;
	}

	menu->autorelease();
	menu->setName(_menuNodeName);
	addChild(menu);

	if (menu)
	{
		_worldMapLayer->SetGuiEnabled(false);
		_worldMapLayer->SetMapInputEnabled(false);
	}
}

void CellMenuSelector::OnCellInfoButtonPressed(cocos2d::Ref *sender)
{
	if (!getChildByName(_menuNodeName))
	{
		CreateMenu(new CellInfoMenu(_cell, this));
	}
}

void CellMenuSelector::OnTasksButtonPressed(cocos2d::Ref *sender)
{
	if (!getChildByName(_menuNodeName))
	{
		if (_cell.lock()->getCurrentTask().expired()) {
			CreateMenu(new CellTasksScreen(_cell, this));
		} else {
			CreateMenu(new CellTaskInfoMenu(_cell, this));
		}
	}
}

void CellMenuSelector::OnSpinoffButtonPressed(cocos2d::Ref *sender)
{
	if (!getChildByName(_menuNodeName))
	{
		CreateMenu(new CellSpinoffMenu(_cell, this, _worldMapLayer));
	}
}

void CellMenuSelector::OnKillButtonPressed(cocos2d::Ref *sender)
{
	_cell.lock()->BeginDestruction();
	DisappearWithAnimation();
}

bool CellMenuSelector::_IsAnimationFinished(void)
{
	for (Buttons::iterator it = _buttons.begin(); it != _buttons.end(); ++it)
	{
		if ((*it)->getNumberOfRunningActions() > 0)
		{
			return false;
		}
	}

	return true;
}
