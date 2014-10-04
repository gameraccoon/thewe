#ifndef TOWN_MAP_WIDGET_H
#define TOWN_MAP_WIDGET_H

#include "Town.h"

#include <cocos2d.h>

class TownMapWidget : public cocos2d::Node
{
public:
	TownMapWidget(Town::Ptr town);

	virtual bool init(void) override;

	void SetHitArea(float beginX, float beginY, float endX, float endY);
	void GetHitArea(float &beginX, float &beginY, float &endX, float &endY) const;

	void SetProjectorUid(int uid);
	int GetProjectorUid(void) const;

	Town::Ptr GetTown(void) const;
	const cocos2d::Rect& GetTownRect(void) const;

private:
	Town::Ptr _town;

	int _projectorUid;

	float _hitAreaBeginX, _hitAreaEndX;
	float _hitAreaBeginY, _hitAreaEndY;

	cocos2d::Sprite *_townMapSprite;
};

#endif