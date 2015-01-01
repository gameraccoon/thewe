#ifndef TUTORIAL_WIDGET_WELCOME_H
#define TUTORIAL_WIDGET_WELCOME_H

#include "TutorialWidget.h"

class TutorialWidgetWelcome : public TutorialWidget
{
public:
	explicit TutorialWidgetWelcome(Tutorial::WeakPtr tutorial);

	static TutorialWidgetWelcome* create(Tutorial::WeakPtr tutorial);

	void TouchesBegan(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);
	void TouchesMoved(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);
	void TouchesEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);

	virtual bool IsReadyToClose(void) const override;

protected:
	virtual bool init() override;
	virtual void update(float dt) override;

private:
	enum class State
	{
		FADE_IN,
		WAIT,
		FADE_OUT,
		FINISH
	};

	cocos2d::DrawNode *_fsQuad;
	cocos2d::ui::Text *_tapToContinue;
	cocos2d::ui::Text *_tutorialText;

	State _state;
	cocos2d::Vec2 _quad[4];
	float _time;
	float _alpha;
};

#endif // TUTORIAL_WIDGET_H
