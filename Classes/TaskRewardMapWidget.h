#ifndef TASK_RESULT_WIDGET_H
#define TASK_RESULT_WIDGET_H

#include "EffectsLayer.h"
#include "Cell.h"
#include "Task.h"
#include "MiscUtils.h"

class CellMapWidget;

class TaskRewardMapWidget : public Effect
{
public:
	TaskRewardMapWidget(CellMapWidget *widget, Cell::WeakPtr cell, const Task::Info &task,
		Utils::GameTime waitTime, cocos2d::Vec2 pos, float scale);

	bool init(void) override;
	void update(float dt) override;
	bool IsFinished(void) const override;

	bool IsTaskId(const std::string &id) const;
	void PickReward(bool squeeze = true);
	void TouchBegan(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);

private:	
	cocos2d::FiniteTimeAction* GetJumpAnimation(void);
	cocos2d::FiniteTimeAction* GetPickAnimation(void);
	cocos2d::FiniteTimeAction* GetLateAnimation(void);

	Cell::WeakPtr _cell;
	Task::Info _task;
	
	Utils::GameTime _startTime;
	Utils::GameTime _waitTime;

	cocos2d::Sprite *_texture;
	cocos2d::Action *_waving;

	cocos2d::Vec2 _pos;
	float _scale;
	float _accentEffTime;
	bool _isDestroyMsgSended;
	bool _isPickEnabled;
	bool _isLiveFinished;
	bool _isWaving;
};

#endif