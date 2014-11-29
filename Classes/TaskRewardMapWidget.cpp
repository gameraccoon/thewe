#include "TaskRewardMapWidget.h"

#include "MessageManager.h"
#include "Log.h"

TaskRewardMapWidget::TaskRewardMapWidget(Cell::WeakPtr cell, const Task::Info &task,
										 Utils::GameTime waitTime, cocos2d::Vec2 pos, float scale)
	: _cell(cell)
	, _task(task)
	, _pos(pos)
	, _scale(scale)
	, _waitTime(waitTime)
	, _startTime(Utils::GetGameTime())
	, _isDestroyMsgSended(false)
{
	init();
}

bool TaskRewardMapWidget::init(void)
{
	cocos2d::EventListenerTouchAllAtOnce *touch = cocos2d::EventListenerTouchAllAtOnce::create();
	touch->onTouchesEnded = CC_CALLBACK_2(TaskRewardMapWidget::TouchEnded, this);
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touch, this);

	_texture = cocos2d::Sprite::create("task_reward.png");
	_texture->setPosition(_pos);
	_texture->setScale(_scale);

	addChild(_texture);
	scheduleUpdate();

	return true;
}

void TaskRewardMapWidget::update(float dt)
{
	if ((Utils::GetGameTime() - _startTime) >= _waitTime && !_isDestroyMsgSended)
	{
		SendMessageAboutDelete();
		_isDestroyMsgSended = true;
	}
}

void TaskRewardMapWidget::PickReward(void)
{
	World::Instance().GetTaskManager().CallCuccessfulCompletition(_cell, &_task);
	SendMessageAboutDelete();
}

bool TaskRewardMapWidget::IsTaskId(const std::string &id) const
{
	return _task.id == id;
}

void TaskRewardMapWidget::TouchEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
	Vector2 location = convertTouchToNodeSpace(touches.at(0));
	if (_texture) {
		if (_texture->getBoundingBox().containsPoint(location)) {
			PickReward();
		}
	}
}

void TaskRewardMapWidget::SendMessageAboutDelete(void) const
{
	Message message("DeleteTaskRewardWidget");
	message.variables.SetInt("CELL_UID", _cell.lock()->GetUid());
	message.variables.SetString("TASK_ID", _task.id);
	MessageManager::Instance().PutMessage(message);
}