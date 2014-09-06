#include "CellGameInterface.h"

CellMenuSelector::CellMenuSelector(Cell::WeakPtr cell, const Vector2 &position)
	: _cell(cell)
	, _position(position)
{
	init();
}

bool CellMenuSelector::init()
{
	if (!cocos2d::Layer::init())
	{
		return false;
	}

	_test = cocos2d::Sprite::create("1_norm.png");
	_test->setPosition(_position);
	_test->setScale(0.4f);
	_test->setOpacity(0);

	cocos2d::MoveTo *move = cocos2d::MoveTo::create(1.0f, _position + Vector2(0.0f, 65.0f));
	cocos2d::ScaleTo *scale = cocos2d::ScaleTo::create(1.0f, 0.7f, 0.7f);
	cocos2d::FadeIn *fade = cocos2d::FadeIn::create(0.25f);

	cocos2d::EaseElasticOut *ease_move = cocos2d::EaseElasticOut::create(move, 0.35f);
	cocos2d::EaseElasticOut *ease_scale = cocos2d::EaseElasticOut::create(scale, 1.0f);

	_test->runAction(ease_move);
	_test->runAction(ease_scale);
	_test->runAction(fade);

	addChild(_test, 0);

	return true;
}