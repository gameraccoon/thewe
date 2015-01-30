#include "TasksMenuWidget.h"

#include "World.h"

TasksMenuWidget* TasksMenuWidget::create(void)
{
	TasksMenuWidget *ret = new TasksMenuWidget();
	if (ret && ret->init())
	{
		ret->autorelease();
		return ret;
	}

	delete ret;
	return nullptr;
}

TasksMenuWidget::TasksMenuWidget(void)
{
	MessageManager::Instance().RegisterReceiver(this, "OpenTasksMenu");
	MessageManager::Instance().RegisterReceiver(this, "CloseTasksMenu");
}

TasksMenuWidget::~TasksMenuWidget(void)
{
}

bool TasksMenuWidget::init(void)
{
	_widget = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("ui_gameplay/tasks_menu.ExportJson");

	dynamic_cast<cocos2d::ui::Button *>(_widget->getChildByName("Close"))->addTouchEventListener(CC_CALLBACK_2(TasksMenuWidget::OnClosePressed, this));

	addChild(_widget);

	Hide();
	
	return true;
}

void TasksMenuWidget::OnClosePressed(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType eventType)
{
	if (eventType == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		Hide();
	}
}

void TasksMenuWidget::AcceptMessage(const Message &message)
{
	if (message.is("OpenTasksMenu"))
	{
		Show();
		_cell = World::Instance().GetCellsNetwork().GetCellByUid(message.variables.GetInt("UID"));
	}
	if (message.is("CloseTasksMenu"))
	{
		Hide();
		_cell = Cell::Ptr();
	}
}

void TasksMenuWidget::Show(void)
{
	setVisible(true);
}

void TasksMenuWidget::Hide(void)
{
	setVisible(false);
}