#ifndef TUTORIAL_WIDGET_FIRST_CELL_H
#define TUTORIAL_WIDGET_FIRST_CELL_H

#include "TutorialWidget.h"

class TutorialWidgetFirstCell : public TutorialWidget
{
public:
	explicit TutorialWidgetFirstCell(Tutorial::WeakPtr tutorial, WorldMapLayer *worldMapLayer, MapProjector *projector);

	static TutorialWidgetFirstCell* create(Tutorial::WeakPtr tutorial, WorldMapLayer *worldMapLayer, MapProjector *projector);
	
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
		FOCUS,
		WAIT,
		DISAPPEAR,
		FINISH
	};

	Vector2 _startViewPos;
	float _startViewScale;
	float _time;

	cocos2d::ui::Text *_tutorialText;

	WorldMapLayer *_worldMapLayer;
	MapProjector *_projector;
	State _state;
};

#endif // TUTORIAL_WIDGET_FIRST_CELL_H
