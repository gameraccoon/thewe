#ifndef CELL_SCREEN_LAYER_H
#define CELL_SCREEN_LAYER_H

#include <cocos2d.h>
#include "Cell.h"

class CellScreenLayer : public cocos2d::CCLayer
{
public:
	CellScreenLayer(Cell::Ptr cell);

	virtual bool init() override;
	virtual void update(float delta) override;
	
	virtual void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event) override;
	virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event) override;
	virtual void ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event) override;

private:
	enum E_MENU_TEMS_TAG
	{
		MENU_ITEM_BACK
	};

private:
	cocos2d::CCMenuItemImage *_btnBack;
	cocos2d::CCLabelTTF *_membersText;
	cocos2d::CCLabelTTF *_cashText;
	cocos2d::CCLabelTTF *_moralText;
	cocos2d::CCLabelTTF *_contentmentText;
	cocos2d::CCLabelTTF *_childCountText;
	cocos2d::CCLabelTTF *_hasParentText;

	cocos2d::CCDrawNode *_bkgDraw;

	Cell::Ptr _cell;

	void _MenuInputListener(cocos2d::CCObject *sender);
	void _InitBackground(cocos2d::CCDrawNode *background) const;
};

#endif // CELL_SCREEN_LAYER_H