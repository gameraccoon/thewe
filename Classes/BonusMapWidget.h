#ifndef BONUS_MAP_WIDGET_H
#define BONUS_MAP_WIDGET_H

#include "CellGameInterface.h"

class BonusMapWidget : public cocos2d::Node
{
public:
	BonusMapWidget(Town::WeakPtr town, Vector2 pos, Utils::GameTime waitTime);

	bool init(void) override;
	void update(float dt) override;

	void SetBonusBehavior(const std::function<void()> &func);
	void SetPickEnabled(bool flag);
	void TouchBegan(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);
	void TouchEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);

private:
	void SendMessageAboutDelete(void);

	cocos2d::FiniteTimeAction* GetShowAnimation(void) const;
	cocos2d::FiniteTimeAction* GetHideAnimation(void) const;

	Town::WeakPtr _town;

	cocos2d::Sprite *_texture;
	std::function<void()> _bonusBehavior;

	Utils::GameTime _startTime;
	Utils::GameTime _waitTime;

	Vector2 _pos;

	bool _destroyMsgSended;
	bool _isPickEnabled;
};

#endif
