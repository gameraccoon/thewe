#include "EditorLayer.h"

#include "WorldMap.h"
#include "GameScene.h"

EditorLayer::EditorLayer(MapProjector* projector)
	: _isCreationAllowed(true)
	, _mapProjector(projector)
{
	init();
}

bool EditorLayer::init(void)
{
	if (!CCLayer::init())
	{
		return false;
	}
	
	_touchPos.x = 0.0f;
	_touchPos.y = 0.0f;

	cocos2d::CCDirector *director = cocos2d::CCDirector::sharedDirector();
	cocos2d::CCSize screen = director->getVisibleSize();
	cocos2d::CCPoint origin = director->getVisibleOrigin();

	_printPos = cocos2d::CCLabelTTF::create("X: 0, Y: 0", "Arial", 32);
	_printPos->setPosition(cocos2d::CCPoint(origin.x + 200, origin.y + screen.height - 100));

	_printNum = cocos2d::CCLabelTTF::create("Num Points: 0", "Arial", 32);
	_printNum->setPosition(cocos2d::CCPoint(origin.x + 200, origin.y + screen.height - 150));

	{
		using namespace cocos2d;
		_btnToggle = cocos2d::CCMenuItemImage::create("btn-toggle-normal.png", "btn-toggle-selected.png",
			this, menu_selector(EditorLayer::_MenuInputListener));
		_btnDelete = cocos2d::CCMenuItemImage::create("btn-delete-normal.png", "btn-delete-selected.png",
			this, menu_selector(EditorLayer::_MenuInputListener));
		_btnSaveXml = cocos2d::CCMenuItemImage::create("btn-save-normal.png", "btn-save-selected.png",
			this, menu_selector(EditorLayer::_MenuInputListener));
		_btnReloadWorld = cocos2d::CCMenuItemImage::create("btn-reload-world-normal.png", "btn-reload-world-selected.png",
			this, menu_selector(EditorLayer::_MenuInputListener));
	}

	cocos2d::CCPoint pos;
	pos.x = origin.x + screen.width / 2.0f;
	pos.y = origin.y + screen.height - 100.0f;

	_btnToggle->setScale(4.0f);
	_btnToggle->setTag(MENU_ITEM_TOGGLE);
	_btnToggle->setPosition(pos - ccp(-700.0f, 100.0f));
	_btnDelete->setScale(4.0f);
	_btnDelete->setTag(MENU_ITEM_DELETE);
	_btnDelete->setPosition(pos - ccp(-700.0f, 300.0f));
	_btnSaveXml->setScale(4.0f);
	_btnSaveXml->setTag(MENU_ITEM_SAVE_XML);
	_btnSaveXml->setPosition(pos - ccp(-700.0f, 500.0f));
	_btnReloadWorld->setScale(4.0f);
	_btnReloadWorld->setTag(MENU_ITEM_RELOAD_WORLD);
	_btnReloadWorld->setPosition(pos - ccp(-700.0f, 700.0f));

	cocos2d::CCMenu *menu = cocos2d::CCMenu::create(_btnToggle, _btnDelete, _btnSaveXml, _btnReloadWorld, NULL);
	menu->setPosition(0.0f, 0.0f);
	
	addChild(_printPos);
	addChild(_printNum);
	addChild(menu);
	setTouchEnabled(true);

	// долгая операция.
	WorldLoader::LoadWorld();

	return true;
}

void EditorLayer::visit(void)
{
	CCLayer::visit();

	if (_isCreationAllowed)
	{
		ArbitraryHull visibleHull;
		for (auto &point : _hull1.GetPoints())
		{
			visibleHull.PushPoint(_mapProjector->ProjectOnScreen(point));
		}

		visibleHull.Draw();
	}
}

void EditorLayer::ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	CCLayer::ccTouchesBegan(touches, event);
	
	cocos2d::CCTouch *touch = dynamic_cast<cocos2d::CCTouch*>(touches->anyObject());

	if (_isCreationAllowed)
	{
		_hull1.PushPoint(_mapProjector->ProjectOnMap(touch->getLocation()));
	}

	char string[64];
	sprintf_s(string, "Num Points: %d", _hull1.GetPointsNum());
	_printNum->setString(string);
}

void EditorLayer::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	CCLayer::ccTouchesEnded(touches, event);

	cocos2d::CCTouch *touch = dynamic_cast<cocos2d::CCTouch*>(touches->anyObject());
	_touchPos = touch->getLocation();
}

void EditorLayer::ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	CCLayer::ccTouchesMoved(touches, event);
	
	cocos2d::CCTouch *touch = dynamic_cast<cocos2d::CCTouch*>(touches->anyObject());
	cocos2d::CCPoint point = touch->getLocation();

	char string[64];
	sprintf_s(string, "X: %d, Y: %d", (int)point.x, (int)point.y);
	_printPos->setString(string);
}

void EditorLayer::_MenuInputListener(cocos2d::CCObject *sender)
{
	cocos2d::CCMenuItemImage *item = dynamic_cast<cocos2d::CCMenuItemImage*>(sender);

	int tag = item->getTag();

	switch (tag)
	{
	case MENU_ITEM_TOGGLE:
		_isCreationAllowed = !_isCreationAllowed;
		break;
	case MENU_ITEM_DELETE:
		_hull1.PopPoint();
		break;
	case MENU_ITEM_SAVE_XML:
		_hull1.SaveToXml("../_gamedata/hulls.xml");
		break;
	case MENU_ITEM_RELOAD_WORLD:
		break;
	default: break;
	}
}
