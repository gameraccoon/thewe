#include "TaskStartButton.h"

const float TaskStartButton::TIME_SCALE = 1.0f/1.0f;
const cocos2d::Vec3 TaskStartButton::COLOR1 = cocos2d::Vec3(1.0f, 0.0f, 0.0f);
const cocos2d::Vec3 TaskStartButton::COLOR2 = cocos2d::Vec3(1.0f, 1.0f, 0.0f);
const cocos2d::Vec3 TaskStartButton::COLOR3 = cocos2d::Vec3(0.0f, 1.0f, 0.0f);

TaskStartButton* TaskStartButton::create(void)
{
	TaskStartButton *ret = new TaskStartButton();
	if (ret && ret->init())
	{
		ret->autorelease();
		return ret;
	}

	delete ret;
	return nullptr;
}

TaskStartButton::TaskStartButton(void)
	: _current(0.0)
	, _next(0.0f)
	, _time(0.0f)
	, _change(false)
{
}

TaskStartButton::~TaskStartButton(void)
{
}

void TaskStartButton::RenewProbability(float prob, bool animated)
{
	if (prob > 100.0f || prob < 0.0f) {
		Log::Instance().writeWarning("Wront value.");
		prob = Math::Clamp(100.0f, 0.0f, prob);
	}
	if (animated) {
		_next = prob;
		_time = 0.0f;
		_change = true;

		cocos2d::ProgressFromTo *effect;
		effect = cocos2d::ProgressFromTo::create(1.0f,_clip->getPercentage(), _next);
		_clip->stopAllActions();
		_clip->runAction(effect);

	} else {
		_current = prob;
		_next = prob;
		_clip->setPercentage(_current);
		_texture->setColor(CalculateColor(_current/100.0f));
	}
}

void TaskStartButton::SetStartTaskCallbak(const std::function<void()> &callback)
{
	_callback = callback;
}

bool TaskStartButton::init(void)
{
	if (!cocos2d::ui::Button::init()) {
		return false;
	}

	_texture = cocos2d::Sprite::create("ui/probability_circle.png");
	_texture->setColor(CalculateColor(_current));

	_clip = cocos2d::ProgressTimer::create(_texture);
	_clip->setPosition((cocos2d::Vec2)_texture->getContentSize()*0.5f);
	_clip->setPercentage(_current);

	loadTextures("ui/start_task_normal.png", "ui/start_task_press.png", "ui/start_task_disabled.png");
	addTouchEventListener(CC_CALLBACK_2(TaskStartButton::OnTouch, this));
	addChild(_clip);
	scheduleUpdate();

	return true;
}

void TaskStartButton::update(float dt)
{
	if (_clip->getNumberOfRunningActions() > 0) {
		float coeff = _clip->getPercentage() / 100.0f;
		cocos2d::Color3B color = CalculateColor(coeff);
		_texture->setColor(color);
	}
}

cocos2d::Color3B TaskStartButton::CalculateColor(float time) const
{
	cocos2d::Vec3 c1, c2;
	float t = 0.0f;
	if (time <= 0.5f) {
		t = time/0.5f;
		c1 = COLOR1;
		c2 = COLOR2;
	} else {
		t = (time-0.5f)/0.5f;
		c1 = COLOR2;
		c2 = COLOR3;
	}
	cocos2d::Vec3 cast = Math::Lerp(c1,c2,t);
	cocos2d::Color3B color;
	color.r = 255*cast.x;
	color.g = 255*cast.y;
	color.b = 255*cast.z;
	return color;
}

void TaskStartButton::OnTouch(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType eventType)
{
	if (eventType == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		_callback();
	}
}
