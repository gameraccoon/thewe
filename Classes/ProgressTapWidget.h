#ifndef PROGRESS_TAP_WIDGET_H
#define PROGRESS_TAP_WIDGET_H

#include <functional>
#include <cocos2d.h>
#include "ProgressBar.h"

class ProgressTapWidget : public cocos2d::Node
{
public:
	typedef std::function<void()> Callback;

public:
	ProgressTapWidget();
	~ProgressTapWidget();
	static ProgressTapWidget* create();

	virtual bool init(void) override;
	virtual void update(float dt) override;

	void initRounding();
	void startRounding();
	void pauseRounding();
	void stopRounding(bool success);

	void setSuccessCallback(Callback callback);
	void setFailureCallback(Callback callback);

private:
	bool _isRoundingNow;
	float _tapChangeValue;
	float _tickChangeValue;
	float _progress;

	cocos2d::Sprite *_backgroundSprite;
	cocos2d::Label *_roundsCountLabel;
	RoundProgressBar *_roundProgressBar;

	Callback _successCallback;
	Callback _failureCallback;

private:
	void processRounding(float dt);
	void touchBegan(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);
};

#endif

