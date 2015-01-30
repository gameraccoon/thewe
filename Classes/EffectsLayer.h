#ifndef EFFECTS_LAYER_H
#define EFFECTS_LAYER_H

#include <cocos2d.h>

#include "MiscUtils.h"

class EffectsLayer;
class Effect : public cocos2d::Node
{
	std::string _name;
	int _zOrder;
	cocos2d::Node *_relationObject;
	friend class EffectsLayer;

public:
	Effect(const std::string &name, int zOrder, cocos2d::Node *relation = nullptr);

	virtual bool init(void) override;
	virtual void update(float dt) override;
	virtual bool IsFinished(void) const = 0;
};

class EffectsLayer : public cocos2d::Layer
{
public:
	EffectsLayer(void);

	bool init(void) override;
	void update(float dt) override;

	void AddEffect(Effect *effect);

private:
	typedef std::vector<Effect *> Effects;

private:
	Effects _effects;
};

#endif