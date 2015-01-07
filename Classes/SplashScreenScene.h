#ifndef SPLASH_SCREEN_SCENE_H
#define SPLASH_SCREEN_SCENE_H

#include <cocos2d.h>

class SplashScreenScene : public cocos2d::Scene
{
public:
	SplashScreenScene();
	virtual ~SplashScreenScene();

	virtual bool init() override;
	virtual void update(float delta) override;

private:
	cocos2d::Sprite *_splashLogo;

	float _timeToShow;
	float _timeElapsed;
};

#endif // SPLASH_SCREEN_SCENE_H
