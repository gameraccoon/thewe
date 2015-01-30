#ifndef SPINOFF_DRAG_AND_DROP_H
#define SPINOFF_DRAG_AND_DROP_H

#include <cocos2d.h>

#include "Cell.h"
#include "MultipleImageSprite.h"
#include "MiscUtils.h"
#include "StripEffect.h"
#include "Vector2.h"

class WorldMapLayer;

class SpinoffDragAndDrop : public cocos2d::Node
{
private:
	enum class State : int
	{
		DISABLED = 0,
		STAY,
		DRAG,
		DROP
	};

public:
	SpinoffDragAndDrop(WorldMapLayer *worldMapLayer, MapProjector *proj, Cell::WeakPtr cellFrom,
		const Vector2 &offsetToCenter = Vector2(0.0f, 0.0f));

	bool init(void) override;
	void update(float dt) override;
	void setOpacity(GLubyte opacity) override;

	void SetEnabled(bool flag);

	void TouchesBegin(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);
	void TouchesEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);
	void TouchesMoved(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);
	void TouchesCancelled(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);

private:
	WorldMapLayer *_worldMapLayer;
	MapProjector *_projector;
	MultipleImageSprite *_texture;
	StripEffect *_strip;
	State _state;
	Cell::WeakPtr _cellFrom;

	Vector2 _touchWorld;
	Vector2 _touchLocal;

	const float _attractionDist;
	bool _isEnabled;
	bool _isAttracting;
};

#endif