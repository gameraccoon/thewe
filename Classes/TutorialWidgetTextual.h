#ifndef TUTORIAL_WIDGET_TEXTUAL_H
#define TUTORIAL_WIDGET_TEXTUAL_H

#include "TutorialWidget.h"

class TutorialWidgetTextual : public TutorialWidget
{
public:
	explicit TutorialWidgetTextual(Tutorial::WeakPtr tutorial);

	static TutorialWidgetTextual* create(Tutorial::WeakPtr tutorial);

	void TouchesBegan(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);
	void TouchesMoved(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);
	void TouchesEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);

protected:
	virtual bool init() override;
	virtual void update(float dt) override;

private:
	enum class State
	{
		SHOW,
		STAND,
		HIDE,
		FINISH
	};

	cocos2d::ui::Text *_tutorialText;
	cocos2d::ui::Text *_tapToContinue;
	cocos2d::DrawNode *_fsQuad;
	cocos2d::Vec2 _quad[4];

	State _state;
	float _time;
};

#endif // TUTORIAL_WIDGET_TEXTUAL_H
