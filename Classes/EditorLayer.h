#ifndef EDITOR_LAYER_H
#define EDITOR_LAYER_H

#include "MapProjector.h"
#include "ArbitraryHull.h"
#include "WorldLoader.h"
#include <cocos2d.h>
#include <pugixml.hpp>

class EditorLayer : public cocos2d::CCLayer
{
public:
	EditorLayer(MapProjector* projector);

	virtual bool init(void);

	void update(float dt) override;
	
	void TouchesBegan(const std::vector<cocos2d::Touch* > &touches, cocos2d::Event* event);
	void TouchesEnded(const std::vector<cocos2d::Touch* > &touches, cocos2d::Event* event);
	void TouchesMoved(const std::vector<cocos2d::Touch* > &touches, cocos2d::Event* event);

private:
	enum E_MENU_TEMS_TAG
	{
		MENU_ITEM_TOGGLE,
		MENU_ITEM_DELETE,
		MENU_ITEM_SAVE_XML,
		MENU_ITEM_RELOAD_WORLD
	};

private:
	void SaveHullToXml(const ArbitraryHull& hull, pugi::xml_document &docXml);
	bool SaveHullToXml(const ArbitraryHull& hull, const char *xmlFilename);

	Vector2 _touchPos;
	
	cocos2d::DrawNode *_hullDrawer;

	cocos2d::Label *_printPos;
	cocos2d::Label *_printNum;
	
	cocos2d::MenuItemImage *_btnToggle;
	cocos2d::MenuItemImage *_btnDelete;
	cocos2d::MenuItemImage *_btnSaveXml;
	cocos2d::MenuItemImage *_btnReloadWorld;

	ArbitraryHull _hull1;
	bool _isCreationAllowed;

	void _MenuInputListener(cocos2d::CCObject *sender);

	MapProjector *_mapProjector;

	Vector2 _touchLastPoint;
};

#endif