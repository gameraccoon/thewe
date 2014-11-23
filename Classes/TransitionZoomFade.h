#ifndef TRANSITION_ZOOM_FADE_H
#define TRANSITION_ZOOM_FADE_H

#include <cocos2d.h>

class TransitionZoomFade : public cocos2d::TransitionScene
{
public:
	TransitionZoomFade(float t, Scene *scene);

	static TransitionZoomFade* create(float t, Scene* scene);
	virtual void onEnter() override;
};

#endif // TRANSITION_ZOOM_FADE_H
