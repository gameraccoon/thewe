#ifndef MAP_DRAW_AND_DROP_WIDGET_H
#define MAP_DRAW_AND_DROP_WIDGET_H

#include <cocos2d.h>

#include "Cell.h"
#include "MultipleImageSprite.h"
#include "MiscUtils.h"
#include "StripEffect.h"
#include "Vector2.h"

class WorldMapLayer;

class MapDragAndDropWidget : public cocos2d::Node
{
public:
	struct Settings
	{
		std::string normalImage;
		std::string pressedImage;
		std::string disabledImage;
	};

private:
	enum class State : int
	{
		DISABLED = 0,
		STAY,
		DRAG,
		DROP
	};

public:
	MapDragAndDropWidget(const Settings &settings, WorldMapLayer *worldMapLayer,
		Cell::WeakPtr cellFrom, const Vector2 &offsetToCenter = Vector2(0.0f, 0.0f));

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
	MultipleImageSprite *_texture;
	StripEffect *_strip;
	Settings _settings;
	State _state;
	Cell::WeakPtr _cellFrom;

	const float _attractionDist;
	bool _isEnabled;
};

#endif