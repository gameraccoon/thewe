#ifndef TUTORIAL_WIDGET_AFTER_FIRST_CELL_H
#define TUTORIAL_WIDGET_AFTER_FIRST_CELL_H

#include "TutorialWidget.h"

class TutorialWidgetAfterFirstCell : public TutorialWidget
{
public:
	explicit TutorialWidgetAfterFirstCell(Tutorial::WeakPtr tutorial, WorldMapLayer *worldMapLayer, MapProjector *projector);

	static TutorialWidgetAfterFirstCell* create(Tutorial::WeakPtr tutorial, WorldMapLayer *worldMapLayer, MapProjector *projector);

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
		FOCUS_ON_CELL,
		FOCUS_ON_BUTTON,
		DISAPPEAR,
		FINISH
	};

	WorldMapLayer *_worldMapLayer;
	MapProjector *_projector;
	Cell::WeakPtr _firstCell;
	ScreenBlackoutWidget *_blackout;
	cocos2d::Sprite *_arrow, *_spot;
	cocos2d::ui::Text *_tutorialText;

	State _state;
	Vector2 _startViewLocation;
	Vector2 _targetViewLocation;
	float _time;
};

#endif // TUTORIAL_WIDGET_AFTER_FIRST_CELL_H
