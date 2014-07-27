#ifndef EDITOR_LAYER_H
#define EDITOR_LAYER_H

#include "MapProjector.h"
#include "ArbitraryHull.h"
#include "WorldLoader.h"

class EditorLayer : public cocos2d::CCLayer
{
public:
	EditorLayer(MapProjector* projector);

	virtual bool init(void);
	virtual void visit(void);
	
	void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
	void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
	void ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event);

private:
	enum E_MENU_TEMS_TAG
	{
		MENU_ITEM_TOGGLE,
		MENU_ITEM_DELETE,
		MENU_ITEM_SAVE_XML,
		MENU_ITEM_BACK
	};

private:
	cocos2d::CCPoint _touchPos;
	
	cocos2d::CCLabelTTF *_printPos;
	cocos2d::CCLabelTTF *_printNum;
	
	cocos2d::CCMenuItemImage *_btnToggle;
	cocos2d::CCMenuItemImage *_btnDelete;
	cocos2d::CCMenuItemImage *_btnSaveXml;
	cocos2d::CCMenuItemImage *_btnBack;

	ArbitraryHull _hull1;
	bool _isCreationAllowed;

	void _MenuInputListener(cocos2d::CCObject *sender);

	MapProjector *_mapProjector;
};

#endif