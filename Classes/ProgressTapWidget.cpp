#include "ProgressTapWidget.h"

ProgressTapWidget::ProgressTapWidget() :
	_isRoundingNow(false),
	_tapChangeValue(0.04f),
	_tickChangeValue(0.1f),
	_backgroundSprite(nullptr),
	_roundsCountLabel(nullptr),
	_roundProgressBar(nullptr),
	_progress(0.0f)
{
}

ProgressTapWidget::~ProgressTapWidget()
{
}

ProgressTapWidget* ProgressTapWidget::create()
{
	ProgressTapWidget* ret = new ProgressTapWidget();
	if (ret && ret->init())
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

bool ProgressTapWidget::init(void)
{
	if (!cocos2d::Node::init())
	{
		return false;
	}

	// adding the common background sprite for this class-widget
	_backgroundSprite = cocos2d::Sprite::create("town.png");
	addChild(_backgroundSprite, 0);

	// adding the round progress bar for this class-widget
	_roundProgressBar = RoundProgressBar::create("cell.png", 1.0f);
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
	if (_isRoundingNow)
	{
		processRounding(dt);
	}
}

void ProgressTapWidget::processRounding(float dt)
{
	_progress += _tickChangeValue * dt;

	float visibleProgress = _roundProgressBar->GetCurrentProgress();
	_roundProgressBar->SetProgressImmediately(visibleProgress + (_progress * 100.0f - visibleProgress) * 0.1f);

	if (_progress > 1.0f)
	{
		stopRounding(false);
	}
	else if (_progress < 0.0f)
	{
		stopRounding(true);
	}
}

void ProgressTapWidget::initRounding()
{
	_isRoundingNow = true;
	_progress = 0.5f;
	_roundProgressBar->SetProgressImmediately(_progress * 100.0f);
}

void ProgressTapWidget::startRounding()
{
	_isRoundingNow = true;
}

void ProgressTapWidget::pauseRounding()
{
	_isRoundingNow = false;
}

static void SafeRunCallback(const ProgressTapWidget::Callback& callback)
{
	if (callback)
	{
		callback();
	}
}

void ProgressTapWidget::stopRounding(bool success)
{
	_isRoundingNow = false;

	if (success)
	{
		SafeRunCallback(_successCallback);
	}
	else
	{
		SafeRunCallback(_failureCallback);
	}
}

void ProgressTapWidget::setSuccessCallback(Callback callback)
{
	_successCallback = callback;
}

void ProgressTapWidget::setFailureCallback(Callback callback)
{
	_failureCallback = callback;
}

void ProgressTapWidget::touchBegan(const std::vector<cocos2d::Touch *>&, cocos2d::Event*)
{
	if (_isRoundingNow)
	{
		_progress -= _tapChangeValue;
	}
}
