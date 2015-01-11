#ifndef INVESTIGATOR_TAP_BUTTON_H
#define INVESTIGATOR_TAP_BUTTON_H

#include <functional>
#include <cocos2d.h>
#include "ProgressBar.h"

class InvestigatorTapButton : public cocos2d::Node
{
public:
	typedef std::function<void()> Callback;

public:
	InvestigatorTapButton();
	~InvestigatorTapButton();
	static InvestigatorTapButton* create();

	virtual bool init(void) override;
	virtual void update(float dt) override;

	void appear(float jumpHeight);
	void hide(void);
	void initRounding(float tickChange, float tapChange);
	void startRounding(float delay);
	void pauseRounding();
	void stopRounding(bool success);

	void setSuccessCallback(Callback callback);
	void setFailureCallback(Callback callback);

private:
	bool _isRoundingNow;
	float _tapChangeValue;
	float _tickChangeValue;
	float _progress;
	float _initialScale;
	float _startDelay;
	float _startDelayTime;

	cocos2d::Sprite *_backgroundSprite;
	cocos2d::Label *_roundsCountLabel;
	RoundProgressBar *_roundProgressBar;

	Callback _successCallback;
	Callback _failureCallback;

private:
	void processRounding(float dt);
	void touchBegan(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);
};

#endif // INVESTIGATOR_TAP_BUTTON_H

