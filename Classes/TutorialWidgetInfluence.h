#ifndef TUTORIAL_WIDGET_INFLUENCE_H
#define TUTORIAL_WIDGET_INFLUENCE_H

#include "TutorialWidget.h"

class TutorialWidgetInfluence : public TutorialWidget
{
public:
	explicit TutorialWidgetInfluence(Tutorial::WeakPtr tutorial);

	static TutorialWidgetInfluence* create(Tutorial::WeakPtr tutorial);

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
	cocos2d::Sprite *_arrow;
	cocos2d::Sprite *_spot;
	ScreenBlackoutWidget *_blackout;

	State _state;
	float _time;
};

#endif // TUTORIAL_WIDGET_INFLUENCE_H
