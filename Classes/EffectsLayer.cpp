#include "EffectsLayer.h"

#include "Log.h"

EffectsLayer::EffectsLayer(void)
{
	init();
}

bool EffectsLayer::init(void)
{
	if (!cocos2d::Layer::init()) {
		return false;
	}

	scheduleUpdate();

	return true;
}

void EffectsLayer::update(float dt)
{
	for (Effects::iterator it = _effects.begin(); it != _effects.end();) {
		Effect *eff = (*it);
		if (eff->IsFinished()) {
			removeChild(eff);
			it = _effects.erase(it);
		}
		else
			++it;
	}
}

void EffectsLayer::AddEffect(Effect *effect)
{
	if (!effect) {
		Log::Instance().writeWarning("EffectsLayer: Failed to add nullptr effect.");
		return;
	}

	_effects.push_back(effect);
	addChild(effect, effect->_zOrder);
}