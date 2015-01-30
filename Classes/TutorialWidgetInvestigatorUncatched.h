#ifndef TUTORIAL_WIDGET_INVESTIGATOR_UNCATCHED_H
#define TUTORIAL_WIDGET_INVESTIGATOR_UNCATCHED_H

#include "TutorialWidget.h"

class TutorialWidgetInvestigatorUncatched : public TutorialWidget
{
public:
	explicit TutorialWidgetInvestigatorUncatched(Tutorial::WeakPtr tutorial, WorldMapLayer *worldMapLayer, MapProjector *projector);

    static TutorialWidgetInvestigatorUncatched* create(Tutorial::WeakPtr tutorial, WorldMapLayer *worldMapLayer, MapProjector *projector);

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
		WAIT,
		FOCUS,
		HIDE,
		FINISH
	};

	cocos2d::ui::Text *_text;
	cocos2d::Sprite *_spot;
	ScreenBlackoutWidget *_blackout;
	WorldMapLayer *_worldMapLayer;
	MapProjector *_projector;

	typedef std::pair<Cell::WeakPtr, cocos2d::Sprite *> Arrow;
	std::vector<Arrow> _arrows;
	Cell::WeakPtr _cell;
	State _state;
	Vector2 _viewStartPos;
	float _viewStartScale;
	float _prevAlpha;
	float _nextAlpha;
	float _localTime;
	float _time;
	float _alphaTime;
	bool _mustChangeAlpha;
	bool _textHiden;
};

#endif // TUTORIAL_WIDGET_INVESTIGATOR_UNCATCHED_H
