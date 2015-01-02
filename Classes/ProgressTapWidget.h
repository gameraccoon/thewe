#ifndef PROGRESS_TAP_WIDGET_H
#define PROGRESS_TAP_WIDGET_H

#include <cocos2d.h>
#include "ProgressBar.h"


class ProgressTapWidget : public cocos2d::Node
{
private:
	enum Repetition
	{
		none, defined, endless
	};

public:
	ProgressTapWidget();
	~ProgressTapWidget();
	static ProgressTapWidget* create();

	virtual bool init(void) override;
	virtual void update(float dt) override;

	void initRounding(const int RepeatCount = 0);
	void startRounding();
	void pauseRounding();
	void stopRounding();

private:
	Repetition _currentRepetitionMode;

	bool _isRoundingNow;
	int _roundsLeft;
	float _tapChangeValue;
	float _tickChangeValue;
	float _progressUpdateIntervalInSec;
	bool _tapInfluenceEnabled;

	cocos2d::Sprite *_backgroundSprite;
	cocos2d::Label *_roundsCountLabel;
	RoundProgressBar *_roundProgressBar;

private:
	void processRounding();

	void touchBegan(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);

};

#endif

