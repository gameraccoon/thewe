#ifndef TASK_START_BUTTON_H
#define TASK_START_BUTTON_H

#include "GameInterface.h"

class TaskStartButton : public cocos2d::ui::Button
{
public:
	static TaskStartButton* create(void);

	void RenewProbability(float prob, bool animated = true);
	void SetStartTaskCallbak(const std::function<void()> &callback);

protected:
	TaskStartButton(void);
	~TaskStartButton(void);

	bool init(void) override;
	void update(float dt) override;

	void OnTouch(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType eventType);

	static const float TIME_SCALE;
	static const cocos2d::Vec3 COLOR1;
	static const cocos2d::Vec3 COLOR2;
	static const cocos2d::Vec3 COLOR3;

private:
	cocos2d::Color3B CalculateColor(float time) const;

	std::function<void()> _callback;

	cocos2d::Sprite *_texture;
	cocos2d::ProgressTimer *_clip;
	float _current;
	float _next;
	float _time;
	bool _change;
};

#endif
