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

CellMenuSelector::~CellMenuSelector(void)
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

void CellMenuSelector::AcceptMessage(const Message &message)
{
	if (message.is("DragOnMapBegan"))
	{
		_btnInfo->runAction(cocos2d::FadeOut::create(0.2f));
		_btnTasks->runAction(cocos2d::FadeOut::create(0.2f));
	}
	else if (message.is("DragOnMapEnded"))
	{
		OnMenuClosed();
		RemoveMenu();
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
		_btnInfo = MenuItemImage::create("2_norm.png", "2_press.png", CC_CALLBACK_1(CellMenuSelector::OnCellInfoButtonPressed, this));
		_btnInfo->setScale(0.85f, 0.85f);
		_btnTasks = MenuItemImage::create("3_norm.png", "3_press.png", CC_CALLBACK_1(CellMenuSelector::OnTasksButtonPressed, this));
		_btnTasks->setScale(0.85f, 0.85f);
		_btnSpinoff = new SpinoffDragAndDrop(_worldMapLayer, _projector, cell, Vector2(0.0f, -45.0f));
		_btnSpinoff->setScale(0.85f, 0.85f);
		_btnSpinoff->SetEnabled(cell->IsReadyToCreateSpinoff());
		_btnSpinoff->autorelease();
		
		_circleMenu->AddNonMenuItem(_btnSpinoff);
		_circleMenu->AddMenuItem(_btnInfo);
		_circleMenu->AddMenuItem(_btnTasks);
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
	return _circleMenu != nullptr;
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
