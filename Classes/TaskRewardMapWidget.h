#ifndef TASK_RESULT_WIDGET_H
#define TASK_RESULT_WIDGET_H

#include "CellGameInterface.h"

class TaskRewardMapWidget : public cocos2d::Node
{
public:
	TaskRewardMapWidget(Cell::WeakPtr cell, const Task::Info &task, Utils::GameTime waitTime, cocos2d::Vec2 pos, float scale);

	bool init(void) override;
	void update(float dt) override;

	void SetPickEnabled(bool flag);
	bool IsTaskId(const std::string &id) const;
	void PickReward(void);
	void TouchBegan(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);
	void TouchEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);

private:
	void SendMessageAboutDelete(void) const;
	
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
	bool _isWaving;
};

#endif