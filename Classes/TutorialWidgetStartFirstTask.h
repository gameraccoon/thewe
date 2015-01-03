#ifndef TUTORIAL_WIDGET_START_FIRST_TASK_H
#define TUTORIAL_WIDGET_START_FIRST_TASK_H

#include "TutorialWidget.h"

class TutorialWidgetStartFirstTask : public TutorialWidget
{
public:
	explicit TutorialWidgetStartFirstTask(Tutorial::WeakPtr tutorial);

	static TutorialWidgetStartFirstTask* create(Tutorial::WeakPtr tutorial);

	void TouchesBegan(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);
	void TouchesMoved(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);
	void TouchesEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);

	bool IsReadyToClose(void) const override;

protected:
	virtual bool init() override;
	virtual void update(float dt) override;

private:
	enum class State
	{
		APPEAR,
		FOCUS,
		WAIT,
		DISAPPEAR,
		FINISH
	};

	cocos2d::Sprite *_arrow;
	cocos2d::Sprite *_spot;
	cocos2d::ui::Text *_text;
	ScreenBlackoutWidget *_blackout;

	State _state;
	Vector2 _spotStartPos;
	float _time;
};

#endif // TUTORIAL_WIDGET_BEFORE_FIRST_TASK_H
