#include "CellGameInterface.h"

#include "WorldMapLayer.h"
#include "CellTasksMenu.h"
#include "CellTaskInfoMenu.h"
#include "CellInfoMenu.h"

CellMenuSelector::CellMenuSelector(MapProjector *proj, WorldMapLayer *map)
	: _projector(proj)
	, _worldMapLayer(map)
	, _menuNodeName("CellMenu")
	, _circleMenu(nullptr)
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

	return true;
}
	
void CellMenuSelector::AppearImmediately(Cell::WeakPtr cell, const Vector2 &position)
{
	InitButtons(cell.lock());
	_circleMenu->setPosition(position);
	_circleMenu->AppearImmediately();

	_cell = cell;
}

void CellMenuSelector::update(float)
{
	if (_circleMenu)
	{
		Cell::Ptr cell = _cell.lock();
		if (cell)
		{
			Vector2 initialPos = cell->GetInfo().location;
			Vector2 updatedPos = _projector->ProjectOnScreen(initialPos);

			_circleMenu->setPosition(updatedPos);
		}
	}
}

void CellMenuSelector::AppearWithAnimation(Cell::WeakPtr cell, const Vector2 &position)
{
	InitButtons(cell.lock());
	_circleMenu->setPosition(position);
	_circleMenu->AppearWithAnimation();
	_cell = cell;
}

void CellMenuSelector::DisappearImmedaitely(void)
{
	if (_circleMenu)
	{
		_circleMenu->DisappearImmedaitely();
		RemoveMenu();
	}
}

void CellMenuSelector::RemoveMenu()
{
	removeChild(_circleMenu);
	_circleMenu = nullptr;
}

void CellMenuSelector::DisappearWithAnimation(void)
{
	if (_circleMenu)
	{
		_circleMenu->DisappearWithAnimation([this]()
			{
				OnMenuClosed();
				RemoveMenu();
			});
	}
}

void CellMenuSelector::InitButtons(Cell::Ptr cell)
{
	if (_circleMenu)
	{
		RemoveMenu();
	}

	_circleMenu = CircleMenu::create();
	addChild(_circleMenu, 0);

	bool mustShowKillButton = World::Instance().IsCellUnderInvestigation(cell) || cell->IsState(Cell::State::AUTONOMY);

	using namespace cocos2d;
	if (mustShowKillButton)
	{
		cocos2d::MenuItemImage *btn;
		btn = MenuItemImage::create("marker_crosshair.png", "marker_crosshair_pressed.png", CC_CALLBACK_1(CellMenuSelector::OnKillButtonPressed, this));
		btn->setScale(1.3f, 1.3f);

		_circleMenu->AddMenuItem(btn);
	}

	if (!cell->IsState(Cell::State::AUTONOMY))
	{
		cocos2d::MenuItemImage *btn1, *btn2, *btn3;

		btn1 = MenuItemImage::create("1_norm.png", "1_press.png", "1_disabled.png", CC_CALLBACK_1(CellMenuSelector::OnSpinoffButtonPressed, this));
		btn1->setScale(0.85f, 0.85f);
		btn1->setEnabled(cell->IsReadyToCreateSpinoff());
		btn2 = MenuItemImage::create("2_norm.png", "2_press.png", CC_CALLBACK_1(CellMenuSelector::OnCellInfoButtonPressed, this));
		btn2->setScale(0.85f, 0.85f);
		btn3 = MenuItemImage::create("3_norm.png", "3_press.png", CC_CALLBACK_1(CellMenuSelector::OnTasksButtonPressed, this));
		btn3->setScale(0.85f, 0.85f);

		_circleMenu->AddMenuItem(btn1);
		_circleMenu->AddMenuItem(btn2);
		_circleMenu->AddMenuItem(btn3);
	}

	_circleMenu->InitMenuItems();
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

bool CellMenuSelector::isOpened() const
{
	return (bool)_circleMenu;
}

void CellMenuSelector::PrepareButtonToAppear(cocos2d::Node *item, Vector2 pos)
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

void CellMenuSelector::PrepareButtonToDisappear(cocos2d::Node* item)
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

void CellMenuSelector::OnSpinoffButtonPressed(cocos2d::Ref*)
{
	if (!getChildByName(_menuNodeName))
	{
		Cell::Ptr cell = _cell.lock();
		if (cell && cell->IsReadyToCreateSpinoff())
		{
			_worldMapLayer->SetNextCellParent(_cell);
			DisappearWithAnimation();

			if (World::Instance().GetTutorialManager().IsTutorialStateAvailable("ReadyToCreateSpinoff"))
			{
				World::Instance().GetTutorialManager().RunTutorialFunction("OnReadyToCreateFirstSpinoff");
			}
		}
	}
}

void CellMenuSelector::OnKillButtonPressed(cocos2d::Ref *sender)
{
	_cell.lock()->BeginDestruction();
	DisappearWithAnimation();
}
