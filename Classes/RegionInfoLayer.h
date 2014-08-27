#ifndef REGION_INFO_LAYER_H
#define REGION_INFO_LAYER_H

#include <cocos2d.h>
#include "Region.h"

class RegionInfoLayer : public cocos2d::CCLayer
{
public:
	RegionInfoLayer(const Region::Info &info);

	virtual bool init();
	
private:
	enum E_MENU_TEMS_TAG
	{
		MENU_ITEM_BACK
	};

private:	
	cocos2d::MenuItemImage *_btnBack;
	cocos2d::CCLabelTTF *_populationText;
	cocos2d::CCLabelTTF *_regionNameText;
	cocos2d::CCLabelTTF *_shortDescText;

	cocos2d::DrawNode *_bkgDraw;

	Region::Info _regionInfo;

	void _MenuInputListener(cocos2d::Object *sender);
	void _InitBackground(cocos2d::DrawNode *background) const;
};

#endif