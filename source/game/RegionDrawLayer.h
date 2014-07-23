#ifndef REGION_DRAW_LAYER_H
#define REGION_DRAW_LAYER_H

#include "RegionTestScene.h"

class RegionDrawLayer : public cocos2d::CCLayer
{
public:
	RegionDrawLayer(void);

	virtual bool init(void);
	virtual void visit(void);
	
	void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
	void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
	void ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event);

private:
	cocos2d::CCPoint _touchPos;
	
	cocos2d::CCLabelTTF *_printPos;
	cocos2d::CCLabelTTF *_printNum;

	ConvexHullTest _hull1;
	bool _isPointInHull;
};

#endif