#include "BonusMapWidget.h"

#include "MessageManager.h"

BonusMapWidget::BonusMapWidget(Cell::WeakPtr cell, Vector2 pos, Utils::GameTime waitTime)
	: _cell(cell)
	, _startTime(Utils::GetGameTime())
	, _waitTime(waitTime)
	, _pos(pos)
	, _destroyMsgSended(false)
	, _isPickEnabled(true)
{
	init();
}

bool BonusMapWidget::init(void)
{
	cocos2d::EventListenerTouchAllAtOnce *listener = cocos2d::EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(BonusMapWidget::TouchBegan, this);
	listener->onTouchesEnded = CC_CALLBACK_2(BonusMapWidget::TouchEnded, this);
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	_texture = cocos2d::Sprite::create("bonus_icon.png");
	_texture->setPosition(0.0f, 0.0f);
	_texture->setScale(0.01f);
	_texture->setOpacity(0);
	_texture->setAnchorPoint(cocos2d::Vec2(0.5f, 0.0f));
	_texture->runAction(GetShowAnimation());

	addChild(_texture);
	scheduleUpdate();

	return true;
}

void BonusMapWidget::update(float dt)
{
	if ((Utils::GetGameTime() - _startTime) >= _waitTime && !_destroyMsgSended)
	{
		cocos2d::CallFunc *func_end = cocos2d::CallFunc::create([&](){SendMessageAboutDelete();});
		_texture->runAction(cocos2d::Sequence::create(GetHideAnimation(), func_end, nullptr));
		_destroyMsgSended = true;
	}
}

void BonusMapWidget::SetBonusBehavior(const std::function<void()> &func)
{
	_bonusBehavior = func;
}

void BonusMapWidget::SetPickEnabled(bool flag)
{
	_isPickEnabled = flag;
}

void BonusMapWidget::TouchBegan(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
	if (!_isPickEnabled) {
		return;
	}
}

void BonusMapWidget::TouchEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
	if (!_isPickEnabled) {
		return;
	}

	Vector2 location = convertTouchToNodeSpace(touches[0]);

	if (_texture->getBoundingBox().containsPoint(location)) {
		// apply some bonus here
		cocos2d::CallFunc *func_bonus = cocos2d::CallFunc::create(_bonusBehavior);
		cocos2d::CallFunc *func_end = cocos2d::CallFunc::create([&](){SendMessageAboutDelete();});
		cocos2d::Sequence *seq = cocos2d::Sequence::create(GetHideAnimation(), func_bonus, func_end, nullptr);
		_texture->runAction(seq);
	}
}

void BonusMapWidget::SendMessageAboutDelete(void)
{
	Message message("DeleteBonusWidget");
	message.variables.SetInt("CELL_UID", _cell.lock()->GetUid());
	MessageManager::Instance().PutMessage(message);
}

cocos2d::FiniteTimeAction* BonusMapWidget::GetShowAnimation(void) const
{
	cocos2d::FadeIn *fade_in = cocos2d::FadeIn::create(0.35f);
	cocos2d::MoveTo *move_up = cocos2d::MoveTo::create(0.5f, _pos);
	cocos2d::ScaleTo *scale = cocos2d::ScaleTo::create(0.4f, 2.5f, 2.5f, 1.0f);
	cocos2d::EaseCubicActionOut *scale_ease = cocos2d::EaseCubicActionOut::create(scale);

	return cocos2d::Spawn::create(fade_in, move_up, scale_ease, nullptr);
}

cocos2d::FiniteTimeAction* BonusMapWidget::GetHideAnimation(void) const
{
	cocos2d::FadeOut *fade_out = cocos2d::FadeOut::create(0.1f);
	cocos2d::MoveTo *move_up = cocos2d::MoveTo::create(0.25f, cocos2d::Vec2(0.0f, 0.0f));
	cocos2d::ScaleTo *scale = cocos2d::ScaleTo::create(0.2f, 0.01f, 0.01f, 1.0f);

	return cocos2d::Spawn::create(fade_out, move_up, scale, nullptr);
}
