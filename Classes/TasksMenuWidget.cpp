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

	cocos2d::ui::Button *btnClose = dynamic_cast<cocos2d::ui::Button *>(cocos2d::ui::Helper::seekWidgetByName(_widget, "Close"));
	cocos2d::ui::Button *btnPageLeft = dynamic_cast<cocos2d::ui::Button *>(cocos2d::ui::Helper::seekWidgetByName(_widget, "PageLeft"));
	cocos2d::ui::Button *btnPageRight = dynamic_cast<cocos2d::ui::Button *>(cocos2d::ui::Helper::seekWidgetByName(_widget, "PageRight"));
	_membersPage = dynamic_cast<cocos2d::ui::PageView *>(cocos2d::ui::Helper::seekWidgetByName(_widget, "MembersPage"));

	btnClose->addTouchEventListener(CC_CALLBACK_2(TasksMenuWidget::OnClosePressed, this));
	btnPageLeft->addTouchEventListener(CC_CALLBACK_2(TasksMenuWidget::OnPageLeft, this));
	btnPageRight->addTouchEventListener(CC_CALLBACK_2(TasksMenuWidget::OnPageRight, this));

	_membersPage->setTouchEnabled(false);

	FillMembersPage();

	addChild(_widget, 0);

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

void TasksMenuWidget::FillMembersPage(void)
{
	// test code
	std::vector<MemberWidget *> members;
	for (int i = 0; i < 11; ++i) {
		members.push_back(MemberWidget::create());
	}

	float width = 120.0f;
	float space = 10.0f;
	float x=10.0f, y=15.0f;
	int page = 0;

	for (std::size_t index = 0; index < members.size(); ++index) {
		MemberWidget *widget = members[index];
		int place = index % 5;
		widget->setPosition(cocos2d::Vec2(width*place + ((place+1)*space), y));
		_membersPage->addWidgetToPage(widget, page, true);
		if (place+1 == 5) {
			++page;
		}
	}
}
