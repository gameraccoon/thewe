#ifndef SPLASH_SCREEN_SCENE_H
#define SPLASH_SCREEN_SCENE_H

#include <cocos2d.h>

class SplashScreenScene : public cocos2d::Scene
{
public:
	SplashScreenScene();
	virtual ~SplashScreenScene();
	static SplashScreenScene* create();

	virtual bool init() override;
	virtual void update(float delta) override;

	void SetLoadingFinished();

private:
	cocos2d::Sprite *_splashLogo;

	float _timeToShow;
	float _timeElapsed;
	bool _isLoadingComplete;
};

#endif // SPLASH_SCREEN_SCENE_H
