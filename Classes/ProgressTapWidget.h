#ifndef PROGRESS_TAP_WIDGET_H
#define PROGRESS_TAP_WIDGET_H

#include <cocos2d.h>
#include "ProgressBar.h"


/** @brief ProgressTapWidget is a widget which includes a RoundProgressBar and 

The main features of a ProgressTapWidget are:
- 

Properties of ProgressTapWidget:
- 


Limitations:
- 

*/

class ProgressTapWidget : public cocos2d::Node
{
private:

	enum Repetition {
		none, defined, endless
	} _currentRepetitionMode;

	bool isRoundingNow;
	int _roundsLeft;
	float _tapChangeValue;
	float _tickChangeValue;
	float _progressUpdateIntervalInSec;
	bool _tapInfluenceEnabled;

	cocos2d::Sprite *_backgroundSprite;
	cocos2d::Label * _roundsCountLabel;
	RoundProgressBar * _roundProgressBar;

	void processRounding();

	void touchBegan(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);

	

public:
	ProgressTapWidget();
	~ProgressTapWidget();

	virtual bool init(void) override;
	virtual void update(float dt) override;

	

	void initRounding(const int RepeatCount = 0);
	void startRounding();
	void pauseRounding();
	void stopRounding();
	





};

#endif

