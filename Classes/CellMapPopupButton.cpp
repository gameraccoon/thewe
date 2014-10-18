#include "CellMapPopupButton.h"

CellMapPopupButton::CellMapPopupButton(const Settings &settings)
	: _settings(settings)
	, _jumpHeight(1.0f)
{
	init();
}

bool CellMapPopupButton::init(void)
{
	_button = cocos2d::MenuItemImage::create(
		  _settings.normalStateImage
		, _settings.pressedStateImage
		, _settings.callbackOnPress);
	_button->setPosition(0.0f, 0.0f);
	_button->setAnchorPoint(cocos2d::Vec2(0.5f, 0.0f));

	_startScale.setPoint(0.1f, 0.1f);

	cocos2d::Menu *menu = cocos2d::Menu::create(_button, nullptr);
	menu->setPosition(0.0f, 0.0f);

	setVisible(false);
	addChild(menu, 0);

	return true;
}

void CellMapPopupButton::SetOnPressCallback(cocos2d::ccMenuCallback &callback)
{
	_button->setCallback(callback);
}

void CellMapPopupButton::Appear(float jumpHeight)
{
	_jumpHeight = jumpHeight;

	setVisible(true);
	_button->setEnabled(true);
	_button->setScale(_startScale.x, _startScale.y);
	_button->setPosition(0.0f, 0.0f);
	_button->stopAllActions();
	AddJumpInAnimation(_button);
}

void CellMapPopupButton::Disappear(bool useWarningAnim)
{
	_button->setEnabled(false);
	_button->stopAllActions();

	if (useWarningAnim)
	{
		AddWarningOutAnimation(_button);
	}
	else
	{
		AddJumpOutAnimation(_button);
	}
}

void CellMapPopupButton::AddJumpInAnimation(cocos2d::Node *target)
{
	if (!target) {
		return;
	}

	float initialScaleX = 1.0f;
	float initialScaleY = 1.0f;

	cocos2d::MoveTo *move = cocos2d::MoveTo::create(0.5f, cocos2d::Vec2(0.0f, _jumpHeight));
	cocos2d::ScaleTo *scale = cocos2d::ScaleTo::create(0.5f, initialScaleX, initialScaleY, 1.0f);
	cocos2d::FadeIn *fadein = cocos2d::FadeIn::create(0.5f);

	cocos2d::EaseCubicActionOut *moveEase = cocos2d::EaseCubicActionOut::create(move);
	cocos2d::EaseCubicActionOut *scaleEase = cocos2d::EaseCubicActionOut::create(scale);

	cocos2d::Spawn *motion = cocos2d::Spawn::create(moveEase, scaleEase, fadein, nullptr);
	cocos2d::CallFunc *func = cocos2d::CallFunc::create(CC_CALLBACK_0(CellMapPopupButton::FinishAppearCAllback, this));

	target->runAction(cocos2d::Sequence::create(motion, func, nullptr));
}

void CellMapPopupButton::AddJumpOutAnimation(cocos2d::Node *target)
{
	if (!target) {
		return;
	}

	cocos2d::MoveTo *move = cocos2d::MoveTo::create(0.25f, cocos2d::Vec2(0.0f, 0.0f));
	cocos2d::ScaleTo *scale = cocos2d::ScaleTo::create(0.25f, _startScale.x, _startScale.y, 1.0f);
	cocos2d::FadeOut *fadeout = cocos2d::FadeOut::create(0.25f);

	cocos2d::EaseCubicActionIn *moveEase = cocos2d::EaseCubicActionIn::create(move);
	cocos2d::EaseCubicActionIn *scaleEase = cocos2d::EaseCubicActionIn::create(scale);

	cocos2d::Spawn *motion = cocos2d::Spawn::create(moveEase, scaleEase, fadeout, nullptr);
	cocos2d::CallFunc *func = cocos2d::CallFunc::create(CC_CALLBACK_0(CellMapPopupButton::FinishDisappearCallback, this));

	target->runAction(cocos2d::Sequence::create(motion, func, nullptr));
}

void CellMapPopupButton::AddWarningOutAnimation(cocos2d::Node *target)
{
	if (!target) {
		return;
	}

	float totalDuration = 1.5f;
	float scaleDuration = totalDuration / 6.0f; // 3 scale with in\out stages

	cocos2d::ScaleTo *scaleIn1 = cocos2d::ScaleTo::create(scaleDuration, 1.4f, 1.4f, 1.0f);
	cocos2d::ScaleTo *scaleOut1 = cocos2d::ScaleTo::create(scaleDuration, 0.9f, 0.9f, 1.0f);
	cocos2d::ScaleTo *scaleIn2 = cocos2d::ScaleTo::create(scaleDuration, 1.2f, 1.2f, 1.0f);
	cocos2d::ScaleTo *scaleOut2 = cocos2d::ScaleTo::create(scaleDuration, 0.7f, 0.7f, 1.0f);
	cocos2d::ScaleTo *scaleIn3 = cocos2d::ScaleTo::create(scaleDuration, 0.9f, 0.9f, 1.0f);
	cocos2d::ScaleTo *scaleOut3 = cocos2d::ScaleTo::create(scaleDuration, 0.5f, 0.5f, 1.0f);

	cocos2d::FadeOut *fadeout = cocos2d::FadeOut::create(totalDuration);

	cocos2d::Sequence *pulsation = cocos2d::Sequence::create(
		scaleIn1
		,scaleOut1
		,scaleIn2
		,scaleOut2
		,scaleIn3
		,scaleOut3
		,nullptr);

	cocos2d::Spawn *motion = cocos2d::Spawn::create(pulsation, fadeout, nullptr);
	cocos2d::CallFunc *func = cocos2d::CallFunc::create(CC_CALLBACK_0(CellMapPopupButton::FinishDisappearCallback, this));

	target->runAction(cocos2d::Sequence::create(motion, func, nullptr));
}

void CellMapPopupButton::AddStayAnimation(cocos2d::Node *target)
{
	if (!target) {
		return;
	}
	
	cocos2d::ScaleTo *scaleIn = cocos2d::ScaleTo::create(0.25f, 0.92f, 0.95f, 1.0f);
	cocos2d::ScaleTo *scaleOut = cocos2d::ScaleTo::create(0.40f, 1.08f, 1.05f, 1.0f);

	cocos2d::EaseCircleActionOut *scaleOutEase = cocos2d::EaseCircleActionOut::create(scaleOut);

	cocos2d::Sequence *pulsation = cocos2d::Sequence::create(scaleIn, scaleOutEase, nullptr);
	cocos2d::RepeatForever *repeat = cocos2d::RepeatForever::create(pulsation);

	target->runAction(repeat);
}

void CellMapPopupButton::FinishAppearCAllback(void)
{
	AddStayAnimation(_button);
}

void CellMapPopupButton::FinishDisappearCallback(void)
{
	setVisible(false);
}