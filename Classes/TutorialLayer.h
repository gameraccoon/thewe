#ifndef TUTORIAL_LAYER_H
#define TUTORIAL_LAYER_H

#include <cocos2d.h>
#include "TutorialWidget.h"

class TutorialLayer : public cocos2d::Layer
{
public:
	TutorialLayer(WorldMapLayer *worldMapLayer, MapProjector *projector);
	static TutorialLayer* create(WorldMapLayer *worldMapLayer, MapProjector *projector);

	virtual bool init(void) override;
	virtual void update(float delta) override;

private:
	TutorialWidget* _currentTutorial;

	WorldMapLayer *_worldMapLayer;
	MapProjector *_projector;
};

#endif // TUTORIAL_LAYER_H
