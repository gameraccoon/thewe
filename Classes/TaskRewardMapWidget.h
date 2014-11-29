#ifndef TASK_RESULT_WIDGET_H
#define TASK_RESULT_WIDGET_H

#include "CellGameInterface.h"

class TaskRewardMapWidget : public cocos2d::Node
{
private:
	enum class State
	{
		EFFECT_SHOW,
		EFFECT_STAY,
		EFFECT_HIDE
	};

public:
	TaskRewardMapWidget(Cell::WeakPtr cell, const Task::Info &task, Utils::GameTime waitTime, cocos2d::Vec2 pos, float scale);

	bool init(void) override;
	void update(float dt) override;

	bool IsTaskId(const std::string &id) const;
	void PickReward(void);
	void TouchEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);

private:
	void SendMessageAboutDelete(void) const;

	Cell::WeakPtr _cell;
	Task::Info _task;
	
	Utils::GameTime _startTime;
	Utils::GameTime _waitTime;

	cocos2d::Sprite *_texture;
	cocos2d::Vec2 _pos;
	float _scale;
	bool _isDestroyMsgSended;

	State _state;
};

#endif