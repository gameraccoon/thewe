#include "TasksMenuWidget.h"

#include "World.h"

TasksMenuWidget* TasksMenuWidget::create(Cell::WeakPtr cell)
{
	TasksMenuWidget *ret = new TasksMenuWidget();
	if (ret && ret->init(cell))
	{
		ret->autorelease();
		return ret;
	}

	delete ret;
	return nullptr;
}

TasksMenuWidget::TasksMenuWidget(void)
{
	MessageManager::Instance().RegisterReceiver(this, "BeginMemberMove");
	MessageManager::Instance().RegisterReceiver(this, "RefreshTaskSlots");
	MessageManager::Instance().RegisterReceiver(this, "RemoveMemberFromSlot");
}

TasksMenuWidget::~TasksMenuWidget(void)
{
	MessageManager::Instance().UnregisterReceiver(this, "BeginMemberMove");
	MessageManager::Instance().UnregisterReceiver(this, "RefreshTaskSlots");
	MessageManager::Instance().UnregisterReceiver(this, "RemoveMemberFromSlot");
}

bool TasksMenuWidget::init(Cell::WeakPtr cell)
{
	if (!cocos2d::Node::init()) {
		return false;
	}

	_cell = cell;

	_widget = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("ui_gameplay/tasks_menu.ExportJson");

	cocos2d::ui::Helper::seekWidgetByName(_widget, "Close")->addTouchEventListener(CC_CALLBACK_2(TasksMenuWidget::OnClosePressed, this));

	_membersPage = MembersPage::create();
	_membersPage->FillWithMembers(_cell.lock()->GetAllMembers());
	_membersPage->setPositionX(_widget->getContentSize().width*0.5f - _membersPage->getContentSize().width*0.5f);
	_membersPage->setPositionY(MembersPage::SPACING);
	_widget->addChild(_membersPage, 1);

	_membersSlot = MembersSlot::create();
	_membersSlot->setVisible(false);
	_widget->addChild(_membersSlot, 1);

	_btnScrollLeft = cocos2d::ui::Button::create("ui/scroll_l_normal.png", "ui/scroll_l_pressed.png");
	_btnScrollLeft->addTouchEventListener(CC_CALLBACK_2(TasksMenuWidget::OnPageLeft, this));
	_btnScrollLeft->setVisible(false);
	_btnScrollRight = cocos2d::ui::Button::create("ui/scroll_r_normal.png", "ui/scroll_r_pressed.png");
	_btnScrollRight->addTouchEventListener(CC_CALLBACK_2(TasksMenuWidget::OnPageRight, this));
	_btnScrollRight->setVisible(false);
	_widget->addChild(_btnScrollLeft, 1);
	_widget->addChild(_btnScrollRight, 1);

	_tasksList = TasksListWidget::create(_cell);
	_tasksList->setPosition(cocos2d::Vec2(400.0f, 130.0f));
	_widget->addChild(_tasksList, 1);

	_startButton = TaskStartButton::create();
	_startButton->setPosition(cocos2d::Vec2(220.0f, 230.0f));
	_startButton->SetStartTaskCallbak([&](){StartTask();});
	_widget->addChild(_startButton, 1);

	addChild(_widget, 0);
	scheduleUpdate();

	Hide();
	
	return true;
}

void TasksMenuWidget::update(float dt)
{
	for (std::vector<MemberMover*>::iterator it = _movers.begin(); it != _movers.end();) {
		MemberMover *mover = (*it);
		if (mover->IsState(MemberMover::State::FINISH)) {
			removeChild(mover);
			it = _movers.erase(it);
		} else 
			++it;
	}

	if (_membersPage->getPages().size() > 1)
	{
		_btnScrollLeft->setVisible(true);
		_btnScrollRight->setVisible(true);
		_btnScrollLeft->setPositionX(_membersPage->getLeftBoundary() - _btnScrollLeft->getContentSize().width * 0.5f);
		_btnScrollLeft->setPositionY(_membersPage->getPositionY() + _btnScrollLeft->getContentSize().height * 0.5f);
		_btnScrollRight->setPositionX(_membersPage->getRightBoundary() + _btnScrollRight->getContentSize().width * 0.5f);
		_btnScrollRight->setPositionY(_membersPage->getPositionY() + _btnScrollRight->getContentSize().height * 0.5f);
	}
	else
	{
		_btnScrollLeft->setVisible(false);
		_btnScrollRight->setVisible(false);
	}
}

void TasksMenuWidget::OnClosePressed(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType eventType)
{
	if (eventType == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		MessageManager::Instance().PutMessage(Message("CloseTasksMenu"));
	}
}

void TasksMenuWidget::OnPageLeft(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType eventType)
{
	if (eventType == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		int page = _membersPage->getCurPageIndex();
		_membersPage->scrollToPage(page-1);
	}
}

void TasksMenuWidget::OnPageRight(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType eventType)
{
	if (eventType == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		int page = _membersPage->getCurPageIndex();
		_membersPage->scrollToPage(page+1);
	}
}

void TasksMenuWidget::Show(void)
{
	_startButton->RenewProbability(100.0);
}

void TasksMenuWidget::Hide(void)
{
}

void TasksMenuWidget::AcceptMessage(const Message &message)
{
	if (message.is("BeginMemberMove") && _tasksList->IsTaskSelected())
	{
		MemberWidget *widget = nullptr;
		for (auto page : _membersPage->getPages()) {
			for (auto item : page->getChildren()) {
				if (item->getTag() == message.variables.GetInt("Index"))
				{
					widget = dynamic_cast<MemberWidget *>(item);
					break;
				}
			}
		}

		if (widget && _membersSlot->IsAbleToAddMember(widget->GetMemberPtr()))
		{
			MemberMover *mover = MemberMover::create(getContentSize(),
				widget->getScale(),
				widget->getWorldPosition(),
				_membersSlot->FindPlace(widget->GetMemberPtr()),
				_membersSlot,
				widget->GetMemberPtr());

			addChild(mover, 2);
			_movers.push_back(mover);
		}
	}
	if (message.is("RefreshTaskSlots"))
	{
		if (_tasksList->IsTaskSelected()) {
			Task::Ptr taskPtr = _tasksList->GetSelectedTask().lock();
			_membersSlot->setVisible(true);
			_membersSlot->FillByTaskRequire(taskPtr);
			_membersSlot->setPositionX(_widget->getContentSize().width*0.5f - _membersSlot->getContentSize().width*0.5f);
			_membersSlot->setPositionY(_widget->getContentSize().height -_membersSlot->getContentSize().height - MembersPage::SPACING);
		} else {
			_membersSlot->setVisible(false);
		}
	}
	if (message.is("RemoveMemberFromSlot"))
	{
		_membersSlot->RemoveMember(message.variables.GetInt("Tag"));
	}
}

void TasksMenuWidget::StartTask(void)
{ 
}
