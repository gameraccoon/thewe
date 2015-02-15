#include "CellRadialMenu.h"

#include "World.h"

CellRadialMenu* CellRadialMenu::create(MapProjector *projector)
{
	CellRadialMenu *ret = new CellRadialMenu(projector);
	if (ret && ret->init())
	{
		ret->autorelease();
		return ret;
	}

	delete ret;
	return nullptr;
}

CellRadialMenu::CellRadialMenu(MapProjector *projector)
	: _projector(projector)
{
	MessageManager::Instance().RegisterReceiver(this, "OpenCellMenu");
	MessageManager::Instance().RegisterReceiver(this, "CloseCellMenu");
}

CellRadialMenu::~CellRadialMenu(void)
{
}

void CellRadialMenu::SetTargetCell(Cell::WeakPtr cell)
{
	_cell = cell;
}

void CellRadialMenu::Show(bool animated)
{
	if (_buttons.empty()) {
		return;
	}

	setVisible(true);

	cocos2d::Vec2 D(0,1);
	float dist = 50.0f;
	float angle = Math::PI*2.0f / _buttons.size();
	float time = 0.0f;

	if (animated)
	{
		std::size_t idx = 0;
		do {
			cocos2d::Node *btn = _buttons[idx];
			D=D.rotateByAngle(cocos2d::Vec2::ZERO, angle);
			D.normalize();
			cocos2d::MoveBy *move = cocos2d::MoveBy::create(0.2f, D*dist);
			cocos2d::ScaleTo *scale = cocos2d::ScaleTo::create(0.2f, 1.0f, 1.0f, 1.0f);
			cocos2d::FadeIn *fadein = cocos2d::FadeIn::create(0.2f);
			cocos2d::Spawn *spawn = cocos2d::Spawn::create(move, scale, fadein, nullptr);
			cocos2d::DelayTime *delay = cocos2d::DelayTime::create(time);
			btn->runAction(cocos2d::Sequence::create(delay, spawn, nullptr));
			time += 0.04f;
			idx++;
		} while (idx < _buttons.size());
	}
	else
	{
		for (auto btn : _buttons) {
			D=D.rotateByAngle(cocos2d::Vec2::ZERO, angle);
			D.normalize();
			btn->setPosition(D*dist);
			btn->setScale(1.0f);
			btn->setOpacity(255);
		}
	}
}

void CellRadialMenu::Hide(bool animated)
{
	if (animated) {
		cocos2d::CallFunc *f = cocos2d::CallFunc::create([&](){
			_cell = Cell::Ptr();
			setVisible(false);
		});
		float time = 0.0f;
		for (auto btn : _buttons) {
			cocos2d::MoveTo *move = cocos2d::MoveTo::create(0.2f, cocos2d::Vec2(0,0));
			cocos2d::ScaleTo *scale = cocos2d::ScaleTo::create(0.2f, 0.1f, 0.1f, 1.0f);
			cocos2d::FadeOut *fadeout = cocos2d::FadeOut::create(0.2f);
			cocos2d::Spawn *spawn = cocos2d::Spawn::create(move, scale, fadeout, nullptr);
			cocos2d::DelayTime *delay = cocos2d::DelayTime::create(time);
			cocos2d::Sequence *seq = cocos2d::Sequence::create(delay, spawn, f, nullptr);
			btn->runAction(seq);
			time += 0.03f;
		}
	} else {
		for (auto btn : _buttons) {
			btn->setPosition(0.0f, 0.0f);
			btn->setScale(0.1f);
			btn->setOpacity(0);
		}
		setVisible(false);
	}
}

void CellRadialMenu::AcceptMessage(const Message &message)
{
	int uid = -1;
	if (!_cell.expired()) {
		uid = _cell.lock()->GetUid();
	}

	if (message.is("OpenCellMenu") && uid != message.variables.GetInt("UID"))
	{
		if (IsEffectsFinished()) {
			Hide(false);
			_cell = World::Instance().GetCellsNetwork().GetCellByUid(message.variables.GetInt("UID"));
			Show(true);
		}
	}
	if (message.is("CloseCellMenu"))
	{
		Hide(IsEffectsFinished() ? true : false);
	}
}

bool CellRadialMenu::IsOpened(void) const
{
	return isVisible();
}

bool CellRadialMenu::IsBelongTo(Cell::Ptr cell) const
{
	return _cell.lock() == cell;
}

bool CellRadialMenu::IsEffectsFinished(void) const
{
	for (auto btn : _buttons) {
		if (btn->getNumberOfRunningActions() > 0) {
			return false;
		}
	}
	return true;
}

bool CellRadialMenu::init(void)
{
	_buttons.resize(ButtonsType::BUTTONS_AMOUNT);

	_buttons[ButtonsType::OPEN_TASK_MENU] = cocos2d::ui::Button::create("ui/radial_tasks_normal.png", "ui/radial_tasks_press.png");
	_buttons[ButtonsType::OPEN_RECRUTMENT_MENU] = cocos2d::ui::Button::create("ui/recrut_normal.png", "ui/recrut_press.png");
	_buttons[ButtonsType::OPEN_SPINOFF_TEAM_MENU] = cocos2d::ui::Button::create("ui/spinoff_normal.png", "ui/spinoff_press.png");
	_buttons[ButtonsType::OPEN_TASK_MENU]->setTag((int)ButtonsType::OPEN_TASK_MENU);
	_buttons[ButtonsType::OPEN_RECRUTMENT_MENU]->setTag((int)ButtonsType::OPEN_RECRUTMENT_MENU);
	_buttons[ButtonsType::OPEN_SPINOFF_TEAM_MENU]->setTag((int)ButtonsType::OPEN_SPINOFF_TEAM_MENU);
	
	dynamic_cast<cocos2d::ui::Button *>(_buttons[ButtonsType::OPEN_TASK_MENU])->addTouchEventListener(CC_CALLBACK_2(CellRadialMenu::ButtonTouchHandler, this));
	dynamic_cast<cocos2d::ui::Button *>(_buttons[ButtonsType::OPEN_RECRUTMENT_MENU])->addTouchEventListener(CC_CALLBACK_2(CellRadialMenu::ButtonTouchHandler, this));
	dynamic_cast<cocos2d::ui::Button *>(_buttons[ButtonsType::OPEN_SPINOFF_TEAM_MENU])->addTouchEventListener(CC_CALLBACK_2(CellRadialMenu::ButtonTouchHandler, this));

	Hide(false);

	for (auto node : _buttons) {
		addChild(node);
	}
	
	scheduleUpdate();
	setVisible(false);
	setScale(1.0f);

	return true;
}

void CellRadialMenu::update(float dt)
{
	if (!_cell.expired())
	{
		Vector2 world = _cell.lock()->GetLocation();
		Vector2 screen = _projector->ProjectOnScreen(world);
		setPosition(screen);
	}
}

void CellRadialMenu::ButtonTouchHandler(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType eventType)
{
	if (_cell.expired()) {
		return;
	}

	if (eventType == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		cocos2d::Node *node = dynamic_cast<cocos2d::Node *>(sender);
		if (node)
		{
			std::string name;
			ButtonsType type = (ButtonsType)node->getTag();
			
			switch (type) {
			case ButtonsType::OPEN_TASK_MENU: 
				name = "OpenTasksMenu";
				break;
			case ButtonsType::OPEN_RECRUTMENT_MENU:
				name = "OpenRecrutMenu";
				break;
			case ButtonsType::OPEN_SPINOFF_TEAM_MENU:
				name = "OpenSpinoffMenu";
				break;
			}

			if (!name.empty()) {
				Message message(name);
				message.variables.SetInt("UID", _cell.lock()->GetUid());
				MessageManager::Instance().PutMessage(message);
				Hide(true);
			}
		}
		else
		{
			Log::Instance().writeWarning("Failed to cast types.");
		}
	}
}