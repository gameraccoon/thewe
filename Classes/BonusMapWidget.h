#ifndef BONUS_MAP_WIDGET_H
#define BONUS_MAP_WIDGET_H

#include "CellGameInterface.h"
#include "EffectsLayer.h"

class BonusMapWidget : public Effect
{
public:
	BonusMapWidget(cocos2d::Node *widget, Cell::WeakPtr cell, Vector2 pos, Utils::GameTime waitTime);

	bool init(void) override;
	void update(float dt) override;
	bool IsFinished(void) const override;

	void SetBonusBehavior(const std::function<void()> &func);
	void SetPickEnabled(bool flag);
	void TouchBegan(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);
	void TouchEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);

private:
	cocos2d::FiniteTimeAction* GetShowAnimation(void) const;
	cocos2d::FiniteTimeAction* GetHideAnimation(void) const;

	Cell::WeakPtr _cell;

	cocos2d::Sprite *_texture;
	std::function<void()> _bonusBehavior;

	Utils::GameTime _startTime;
	Utils::GameTime _waitTime;

	Vector2 _pos;

	bool _destroyMsgSended;
	bool _isPickEnabled;
	bool _isLiveFinished;
};

#endif
