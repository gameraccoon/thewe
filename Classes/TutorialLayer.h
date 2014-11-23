#ifndef TUTORIAL_LAYER_H
#define TUTORIAL_LAYER_H

#include <cocos2d.h>
#include "TutorialWidget.h"

class TutorialLayer : public cocos2d::Layer
{
public:
	TutorialLayer();
	static TutorialLayer* create();

	virtual bool init(void) override;
	virtual void update(float delta);

private:
	TutorialWidget* _currentTutorial;
};

#endif // TUTORIAL_LAYER_H
