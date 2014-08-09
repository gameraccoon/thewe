#ifndef TOWN_INFO_LAYER_H
#define TOWN_INFO_LAYER_H

#include "Town.h"
#include <cocos2d.h>

class TownInfoLayer : public cocos2d::CCLayer
{
public:
	TownInfoLayer(void);

	virtual bool init() override;
	
	virtual void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event) override;
	virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event) override;
	virtual void ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event) override;

	void SelectTown(Town::Ptr town);

private:
	Town::Ptr _town;

	cocos2d::CCMenu *_menu;

	cocos2d::CCLabelTTF *_nameText;
	cocos2d::CCLabelTTF *_populText;
	cocos2d::CCLabelTTF *_descText;
};

#endif