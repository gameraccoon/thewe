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

	void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
	void ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);

private:
	cocos2d::CCSprite *_movingSprite;
	cocos2d::CCPoint _touchPos;

	void _IdleUpdate(float timeDelta);
};

#endif
