#ifndef TUTORIAL_WIDGET_SPINOFF_H
#define TUTORIAL_WIDGET_SPINOFF_H

#include "TutorialWidget.h"

class TutorialWidgetSpinoff : public TutorialWidget
{
public:
    explicit TutorialWidgetSpinoff(Tutorial::WeakPtr tutorial, WorldMapLayer *worldMapLayer, MapProjector *projector);

    static TutorialWidgetSpinoff* create(Tutorial::WeakPtr tutorial, WorldMapLayer *worldMapLayer, MapProjector *projector);

	void TouchesBegan(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);
	void TouchesMoved(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);
	void TouchesEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);

protected:
    virtual bool init() override;
	virtual void update(float dt) override;

	static cocos2d::Action* MakeHandDragAction(cocos2d::Vec2 point1, cocos2d::Vec2 point2);
	static cocos2d::Action* MakeHandPressAction(cocos2d::Vec2 point);

private:
	enum class State
	{
		FOCUS_ON_CELL,
		FOCUS_ON_BUTTON,
		FOCUS_ON_DRAG
	};

private:
	Town::WeakPtr FindSuitableSpinoffTown(void) const;

	WorldMapLayer *_worldMapLayer;
	MapProjector *_projector;
	ScreenBlackoutWidget *_blackout;
	Cell::WeakPtr _cell;
	Town::WeakPtr _spinoffTown;
	cocos2d::ui::Text *_text;
	cocos2d::Sprite *_arrow;
	cocos2d::Sprite *_hand;
	cocos2d::Sprite *_roundSpot;

	State _state;
	Vector2 _viewStartLocation;
	float _viewStartScale;
	float _time;
	float _viewFocusTime;
	float _stateChangeTime;
	float _prevAlpha;
	float _nextAlpha;
	bool _viewFocused;
	bool _mustChangeState;
};

#endif // TUTORIAL_WIDGET_SPINOFF_H
