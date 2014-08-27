#ifndef TOWN_INFO_LAYER_H
#define TOWN_INFO_LAYER_H

#include "Town.h"
#include <cocos2d.h>

class TownInfoLayer : public cocos2d::CCLayer
{
public:
	TownInfoLayer(void);

	virtual bool init() override;
	
	void SelectTown(Town::WeakPtr town);

private:
	Town::WeakPtr _town;

	cocos2d::CCMenu *_menu;

	cocos2d::LabelTTF *_nameText;
	cocos2d::LabelTTF *_populText;
	cocos2d::LabelTTF *_descText;
};

#endif