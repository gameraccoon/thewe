#include "TasksListWidget.h"

#include "World.h"

TasksListItem* TasksListItem::createWithTask(Task::Ptr task)
{
	TasksListItem *ret = new TasksListItem();
	if (ret && ret->initWithTask(task))
	{
		ret->autorelease();
		return ret;
	}

	delete ret;
	return nullptr;
}

TasksListItem* TasksListItem::createEmpty(void)
{
	TasksListItem *ret = new TasksListItem();
	if (ret && ret->initEmpty())
	{
		ret->autorelease();
		return ret;
	}

	delete ret;
	return nullptr;
}

TasksListItem::TasksListItem(void)
	: _state(false)
{
}

TasksListItem::~TasksListItem(void)
{
}

bool TasksListItem::initWithTask(Task::Ptr task)
{
	if (!cocos2d::ui::Button::init()) {
		return false;
	}

	_task = task;

	loadTextures("ui/tasks_menu_item_normal.png", "ui/tasks_menu_item_press.png");
	addTouchEventListener(CC_CALLBACK_2(TasksListItem::OnPress, this));

	setTitleFontName("EuropeNormal.ttf");
	setTitleFontSize(18.0f);
	setTitleColor(cocos2d::Color3B(0,0,0));
	setTitleText(_task->GetInfo().id);

	return true;
}

bool TasksListItem::initEmpty(void)
{
	if (!cocos2d::ui::Button::init()) {
		return false;
	}
	return true;
}

void TasksListItem::OnPress(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType eventType)
{
	if (getOpacity() <= 0) {
		return;
	}

	if (eventType == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		_state = !_state;
		UpdateTexture();
		Message message("TaskItemSelected");
		message.variables.SetInt("Tag", getTag());
		MessageManager::Instance().PutMessage(message);
	}
}

Task::WeakPtr TasksListItem::GetTask(void) const
{
	return _task;
}

void TasksListItem::ReleaseToggle(void)
{
	_state = false;
	UpdateTexture();
}

bool TasksListItem::IsPressed(void) const
{
	return _state;
}

void TasksListItem::UpdateTexture(void)
{
	std::string texture = _state ? "ui/tasks_menu_item_press.png" : "ui/tasks_menu_item_normal.png";
	loadTextureNormal(texture);
}

const float TasksListWidget::VISIBLE_AREA_WIDTH = 380.0f;
const float TasksListWidget::VISIBLE_AREA_HEIGHT = 220.0f;

TasksListWidget* TasksListWidget::create(Cell::WeakPtr cell)
{
	TasksListWidget *ret = new TasksListWidget(cell);
	if (ret && ret->init())
	{
		ret->autorelease();
		return ret;
	}

	delete ret;
	return nullptr;
}

TasksListWidget::TasksListWidget(Cell::WeakPtr cell)
	: _cell(cell)
{
	MessageManager::Instance().RegisterReceiver(this, "TaskItemSelected");
}
	
TasksListWidget::~TasksListWidget(void)
{
}

void TasksListWidget::FillList(const TaskManager::Tasks &tasks)
{
}

void TasksListWidget::AcceptMessage(const Message &message)
{
	if (message.is("TaskItemSelected"))
	{
		auto childrens = getItems();
		for (auto child : childrens) {
			TasksListItem *item;
			item = dynamic_cast<TasksListItem *>(child);
			if (item && item->getTag() != message.variables.GetInt("Tag")) {
				item->ReleaseToggle();
			}
		}

		TasksListItem *selected = dynamic_cast<TasksListItem *>(getChildByTag(message.variables.GetInt("Tag")));
		if (selected) {
			if (selected->IsPressed()) {
				_selectedTask = selected->GetTask();
			} else {
				_selectedTask = Task::Ptr();
			}
			MessageManager::Instance().PutMessage(Message("RefreshTaskSlots"));
		} else {
			Log::Instance().writeWarning("Type cast failed.");
		}
	}
}

bool TasksListWidget::IsTaskSelected(void) const
{
	return !_selectedTask.expired();
}

Task::WeakPtr TasksListWidget::GetSelectedTask(void) const
{
	return _selectedTask;
}

bool TasksListWidget::init(void)
{
	if (!cocos2d::ui::ListView::init()) {
		return false;
	}

	setClippingEnabled(false);
	setItemsMargin(5.0f);
	setGravity(cocos2d::ui::ListView::Gravity::CENTER_HORIZONTAL);
	setContentSize(cocos2d::Size(VISIBLE_AREA_WIDTH, VISIBLE_AREA_HEIGHT));
	scheduleUpdate();
	
	TasksListItem *first = TasksListItem::createEmpty();
	first->setVisible(false);
	pushBackCustomItem(first);

	TaskManager::Tasks tasks = World::Instance().GetTaskManager().GetAvailableTasks(_cell);

	int index = 0;
	for (Task::Ptr task : tasks) {
		TasksListItem *item = TasksListItem::createWithTask(task);
		item->setTag(index);
		pushBackCustomItem(item);
		++index;
	}

	TasksListItem *last = TasksListItem::createEmpty();
	last->setVisible(false);
	pushBackCustomItem(last);

	return true;
}

void TasksListWidget::update(float dt)
{
	float height_half = VISIBLE_AREA_HEIGHT * 0.5f;
	cocos2d::Vec2 C = cocos2d::Vec2(0.0f, height_half);
	cocos2d::Vector<cocos2d::ui::Widget *> &items = getItems();

	for (cocos2d::Vector<cocos2d::ui::Widget *>::iterator it = items.begin(); it != items.end(); ++it)
	{
		cocos2d::ui::Widget *item = (*it);
		cocos2d::Vec2 P = cocos2d::Vec2(0.0f, item->getParent()->getPosition().y + item->getPosition().y);
		float D = C.getDistance(P);
		float T = fabs(1.0f - Math::Clamp(1.0f, 0.0f, D / height_half));
		float alpha = T;
		alpha -= 1.0f;
		alpha = alpha*alpha*alpha + 1.0f;
		float scale = T;
		scale -= 1.0f;
		scale = scale*scale*scale*scale*scale + 1.0f;
		item->setOpacity(255*alpha);
		item->setScale(Math::Lerp(1.0f, 0.6f, fabs(1.0f - scale)));
	}
}
