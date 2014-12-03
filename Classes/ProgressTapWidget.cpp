#include "ProgressTapWidget.h"

ProgressTapWidget::ProgressTapWidget() :
_currentRepetitionMode(endless),
isRoundingNow(false),
_roundsLeft(0.0f),
_tapChangeValue(-1.0f),
_tickChangeValue(1.0f),
_progressUpdateIntervalInSec(0.1f),
_tapInfluenceEnabled(false),
_backgroundSprite(nullptr),
_roundsCountLabel(nullptr),
_roundProgressBar(nullptr)
{
	init();
}


ProgressTapWidget::~ProgressTapWidget()
{
	delete _roundProgressBar;
}

bool ProgressTapWidget::init(void)
{
	if (!cocos2d::Node::init()) return false;

	// adding the common background sprite for this class-widget
	_backgroundSprite = cocos2d::Sprite::create("town.png");
	addChild(_backgroundSprite, 0);

	// adding the round progress bar for this class-widget
	_roundProgressBar = new RoundProgressBar("cell.png", 1.0f);
	addChild(_roundProgressBar, 1);

	//_roundsCountLabel = cocos2d::Label::create("", "arial.ttf", 20);
	//addChild(_roundsCountLabel, 2);

	// adding events for current ProgressTapWidget
	cocos2d::EventListenerTouchAllAtOnce *touch = cocos2d::EventListenerTouchAllAtOnce::create();
	touch->onTouchesBegan = CC_CALLBACK_2(ProgressTapWidget::touchBegan, this);
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touch, this);

	scheduleUpdate();

	return true;
}

void ProgressTapWidget::update(float dt)
{
	static float toUpdate;

	if (toUpdate <= _progressUpdateIntervalInSec)
		toUpdate += dt;
	else
	{
		if (isRoundingNow)
			processRounding();
		toUpdate = 0.0f;
	}
}

void ProgressTapWidget::processRounding()
{
	if (_currentRepetitionMode != endless && !_roundsLeft)
	{
		stopRounding();
		return;
	}
	_roundProgressBar->SetProgressImmediately(_roundProgressBar->GetCurrentProgress() + _tickChangeValue);
	if (_roundProgressBar->IsFinished())
	{		
		if (_currentRepetitionMode == endless || --_roundsLeft)
			_roundProgressBar->SetProgressImmediately(0.0f);
	}
}

void ProgressTapWidget::initRounding(const int _repeatsCount)
{
	switch (_repeatsCount)
	{
	case 0:
		_currentRepetitionMode = endless;
		break;
	default:
		_currentRepetitionMode = defined;
		break;
	}
	isRoundingNow = true;
	_roundsLeft = _repeatsCount;
	_tapInfluenceEnabled = true;
}

void ProgressTapWidget::startRounding()
{
	isRoundingNow = true;
}

void ProgressTapWidget::pauseRounding()
{
	isRoundingNow = false;
	_tapInfluenceEnabled = false;
}

void ProgressTapWidget::stopRounding()
{
	isRoundingNow = false;
	_tapInfluenceEnabled = false;
	_roundsLeft = 0;
}

void ProgressTapWidget::touchBegan(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
	if (_tapInfluenceEnabled)
	{
		const float & current_progress = _roundProgressBar->GetCurrentProgress();
		_roundProgressBar->SetProgressImmediately(current_progress + _tapChangeValue);
		if (current_progress == 0.0f)
			stopRounding();
	}
}





