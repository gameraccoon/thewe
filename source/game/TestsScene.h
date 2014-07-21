#ifndef TEST_SCENE_H
#define TEST_SCENE_H

#include "cocos2d.h"

class HelloWorldLayer : public cocos2d::CCLayer
{
public:
	virtual bool init(void);

	static cocos2d::CCScene* scene(void);

	void menuCloseCallback(cocos2d::CCObject *Sender);

	CREATE_FUNC(HelloWorldLayer);
};

#endif
