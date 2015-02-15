#include "InvestigatorTapButton.h"

#include "World.h"
#include "GameInfo.h"

InvestigatorTapButton::InvestigatorTapButton() :
	_isRoundingNow(false),
	_tapChangeValue(0.04f),
	_tickChangeValue(0.1f),
	_initialScale(0.1f),
	_startDelay(0.0f),
	_startDelayTime(0.0f),
	_backgroundSprite(nullptr),
	_roundsCountLabel(nullptr),
	_roundProgressBar(nullptr),
	_progress(0.0f)
{
}

InvestigatorTapButton::~InvestigatorTapButton()
{
}

InvestigatorTapButton* InvestigatorTapButton::create()
{
	InvestigatorTapButton* ret = new InvestigatorTapButton();
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

bool InvestigatorTapButton::init(void)
{
	if (!cocos2d::Node::init())
	{
		return false;
	}

	// adding the common background sprite for this class-widget
	_backgroundSprite = cocos2d::Sprite::create("effects/marker_crosshair.png");
	_backgroundSprite->setAnchorPoint(cocos2d::Vec2(0.5f, 0.0f));
	_backgroundSprite->setScale(_initialScale);
	_backgroundSprite->setVisible(false);
	addChild(_backgroundSprite, 0);

	// adding the round progress bar for this class-widget
	_roundProgressBar = RoundProgressBar::create("effects/marker_crosshair_progress.png", 1.0f, 0.5f);
	_roundProgressBar->setPosition((cocos2d::Vec2)_backgroundSprite->getContentSize() / 2.0f);
	_backgroundSprite->addChild(_roundProgressBar);

	// adding events for current ProgressTapWidget
	cocos2d::EventListenerTouchAllAtOnce *touch = cocos2d::EventListenerTouchAllAtOnce::create();
	touch->onTouchesBegan = CC_CALLBACK_2(InvestigatorTapButton::touchBegan, this);
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touch, this);

	scheduleUpdate();

	return true;
}

void InvestigatorTapButton::update(float dt)
{
	if (_isRoundingNow)
	{
		processRounding(dt);
	}
}

void InvestigatorTapButton::processRounding(float dt)
{
	if (_startDelayTime > _startDelay)
	{
		_progress += _tickChangeValue * dt;

		float visibleProgress = _roundProgressBar->GetCurrentProgress();
		_roundProgressBar->SetProgressImmediately(visibleProgress + (_progress * 100.0f - visibleProgress) * 0.05f);

		if (_progress > 1.0f)
		{
			stopRounding(false);
		}
		else if (_progress < 0.0f)
		{
			stopRounding(true);
		}
	}
	else
	{
		_startDelayTime += dt;
	}
}

void InvestigatorTapButton::appear(float jumpHeight)
{
	cocos2d::MoveTo *move = cocos2d::MoveTo::create(0.5f, cocos2d::Vec2(0.0f, jumpHeight));
	cocos2d::ScaleTo *scale = cocos2d::ScaleTo::create(0.5f, 1.0f, 1.0f, 1.0f);
	cocos2d::FadeIn *fadein = cocos2d::FadeIn::create(0.5f);

	cocos2d::EaseCubicActionOut *moveEase = cocos2d::EaseCubicActionOut::create(move);
	cocos2d::EaseCubicActionOut *scaleEase = cocos2d::EaseCubicActionOut::create(scale);

	cocos2d::Spawn *motion = cocos2d::Spawn::create(moveEase, scaleEase, fadein, nullptr);

	_backgroundSprite->setVisible(true);
	_backgroundSprite->setScale(_initialScale);
	_backgroundSprite->stopAllActions();
	_backgroundSprite->runAction(motion);

	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("investigator.wav");
}

void InvestigatorTapButton::hide(void)
{
	cocos2d::MoveTo *move = cocos2d::MoveTo::create(0.5f, cocos2d::Vec2(0.0f, 0.0f));
	cocos2d::ScaleTo *scale = cocos2d::ScaleTo::create(0.5f, _initialScale, _initialScale);
	cocos2d::FadeOut *fadeout = cocos2d::FadeOut::create(0.5f);

	cocos2d::EaseCubicActionOut *move_ease = cocos2d::EaseCubicActionOut::create(move);
	cocos2d::EaseCubicActionOut *scale_ease = cocos2d::EaseCubicActionOut::create(scale);

	cocos2d::Spawn *motion = cocos2d::Spawn::create(move_ease, scale_ease, fadeout, nullptr);
	cocos2d::CallFunc *func = cocos2d::CallFunc::create([&](){
		_backgroundSprite->setVisible(false);
	});

	_backgroundSprite->stopAllActions();
	_backgroundSprite->runAction(cocos2d::Sequence::create(motion, func, nullptr));
}

void InvestigatorTapButton::initRounding(float tickChange, float tapChange)
{
	_isRoundingNow = true;
	_tickChangeValue = tickChange;
	_tapChangeValue = tapChange;
	_progress = 0.5f;
	_roundProgressBar->SetProgressImmediately(_progress * 100.0f);
}

void InvestigatorTapButton::startRounding(float delay)
{
	_isRoundingNow = true;
	_startDelayTime = 0.0f;
	_startDelay = delay;
}

void InvestigatorTapButton::pauseRounding()
{
	_isRoundingNow = false;
}

static void SafeRunCallback(const InvestigatorTapButton::Callback& callback)
{
	if (callback)
	{
		callback();
	}
}

void InvestigatorTapButton::stopRounding(bool success)
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

void InvestigatorTapButton::setSuccessCallback(Callback callback)
{
	_successCallback = callback;
}

void InvestigatorTapButton::setFailureCallback(Callback callback)
{
	_failureCallback = callback;
}

void InvestigatorTapButton::touchBegan(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
	if (_isRoundingNow)
	{
		cocos2d::Vec2 finger = convertToNodeSpace(touches[0]->getLocation());
		if (_backgroundSprite->getBoundingBox().containsPoint(finger)) {
			_progress -= _tapChangeValue;
			if (_backgroundSprite->getNumberOfRunningActions() <= 0) {
				cocos2d::ScaleTo *scale1 = cocos2d::ScaleTo::create(0.04f, 1.1f, 0.9f, 1.0f);
				cocos2d::ScaleTo *scale2 = cocos2d::ScaleTo::create(0.09f, 1.0f, 1.0f, 1.0f);
				cocos2d::Sequence *seq = cocos2d::Sequence::create(scale1, scale2, nullptr);
				_backgroundSprite->runAction(seq);
			}
		}
	}
}
