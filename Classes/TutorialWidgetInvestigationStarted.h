#ifndef TUTORIAL_WIDGET_INVESTIGATION_STARTED_H
#define TUTORIAL_WIDGET_INVESTIGATION_STARTED_H

#include "TutorialWidget.h"

class TutorialWidgetInvestigationStarted : public TutorialWidget
{
public:
    explicit TutorialWidgetInvestigationStarted(Tutorial::WeakPtr tutorial, WorldMapLayer *worldMapLayer, MapProjector *projector);

    static TutorialWidgetInvestigationStarted* create(Tutorial::WeakPtr tutorial, WorldMapLayer *worldMapLayer, MapProjector *projector);

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
		MOVE,
		SHOW_FINGER,
		HIDE,
		FINISH
	};

	float _startViewScale;
	float _time;
	Vector2 _startViewPos;
	State _state;

	cocos2d::Sprite *_finger;
	cocos2d::ui::Text *_text;
	Cell::WeakPtr _cell;
	WorldMapLayer *_worldMapLayer;
	MapProjector *_projector;
};

#endif // TUTORIAL_WIDGET_SPINOFF_H
