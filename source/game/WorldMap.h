#ifndef WORLD_MAP_H
#define WORLD_MAP_H

#include "cocos2d.h"

class WorldMapLayer : public cocos2d::CCLayer
{
public:
	virtual bool init(void);

	static cocos2d::CCScene* scene(void);

	void menuCloseCallback(cocos2d::CCObject *Sender);

	CREATE_FUNC(WorldMapLayer);
};

#endif
