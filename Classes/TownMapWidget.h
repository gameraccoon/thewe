#ifndef TOWN_MAP_WIDGET_H
#define TOWN_MAP_WIDGET_H

#include "Town.h"
#include "MessageManager.h"

#include <cocos2d.h>

class TownMapWidget : public cocos2d::Node, public MessageReceiver
{
public:
	TownMapWidget(Town::Ptr town);
	~TownMapWidget(void);

	virtual bool init(void) override;

	void AcceptMessage(const Message &message) override;

	void SetHitArea(const cocos2d::Rect& hitArea);
	cocos2d::Rect GetHitArea() const;

	void SetTownImageVisible(bool visible);
	void SetProjectorUid(int uid);
	int GetProjectorUid(void) const;

	Town::Ptr GetTown(void) const;
	const cocos2d::Rect& GetTownRect(void) const;

private:
	Town::Ptr _town;
	
	cocos2d::Sprite *_townMapSprite;

	int _projectorUid;

	cocos2d::Rect _hitArea;
};

#endif
