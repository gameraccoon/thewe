#include "RegionDrawLayer.h"

RegionDrawLayer::RegionDrawLayer(void)
	: _isCreationAllowed(true)
{
	init();
}

bool RegionDrawLayer::init(void)
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

	_btnToggle = cocos2d::CCMenuItemImage::create("../_gamedata/btn-toggle-normal.png",
		"../_gamedata/btn-toggle-selected.png", this, menu_selector(RegionDrawLayer::_MenuInputListener));
	_btnDelete = cocos2d::CCMenuItemImage::create("../_gamedata/btn-delete-normal.png",
		"../_gamedata/btn-delete-selected.png", this, menu_selector(RegionDrawLayer::_MenuInputListener));
	_btnSaveXml = cocos2d::CCMenuItemImage::create("../_gamedata/btn-save-normal.png",
		"../_gamedata/btn-save-selected.png", this, menu_selector(RegionDrawLayer::_MenuInputListener));

	CCPoint pos;
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

	cocos2d::CCMenu *menu = cocos2d::CCMenu::create(_btnToggle, _btnDelete, _btnSaveXml, NULL);
	menu->setPosition(0.0f, 0.0f);

	addChild(_printPos);
	addChild(_printNum);
	addChild(menu);
	setTouchEnabled(true);
	
	/*_hull1.PushPoint(ccp(100, 100));
	_hull1.PushPoint(ccp(500, 900));
	_hull1.PushPoint(ccp(700, 1000));
	_hull1.PushPoint(ccp(900, 950));
	_hull1.PushPoint(ccp(900, 750));
	_hull1.PushPoint(ccp(600, 750));
	_hull1.PushPoint(ccp(600, 400));
	_hull1.PushPoint(ccp(900, 400));*/

	_isPointInHull = false;

	return true;
}

void RegionDrawLayer::visit(void)
{
	cocos2d::ccColor4F color;
	if (_isPointInHull)
	{
		color.r = 0.0f;
		color.g = 0.6f;
		color.b = 1.0f;
		color.a = 0.7f;
	}
	else
	{
		color.r = 1.0f;
		color.g = 0.6f;
		color.b = 0.0f;
		color.a = 0.7f;
	}

	_hull1.Draw(color);

	CCLayer::visit();
}
	
void RegionDrawLayer::ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	cocos2d::CCLayer::ccTouchesBegan(touches, event);

	cocos2d::CCTouch *touch = (cocos2d::CCTouch *)touches->anyObject();

	if (_isCreationAllowed)
	{
		_hull1.PushPoint(touch->getLocation());
	}

	char string[64];
	sprintf_s(string, "Num Points: %d", _hull1.GetPointsNum());
	_printNum->setString(string);
}

void RegionDrawLayer::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	cocos2d::CCLayer::ccTouchesEnded(touches, event);

	cocos2d::CCTouch *touch = (cocos2d::CCTouch *)touches->anyObject();
	_touchPos = touch->getLocation();

	if (_hull1.Contain(_touchPos))
	{
		_isPointInHull = !_isPointInHull;
	}
}

void RegionDrawLayer::ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	cocos2d::CCLayer::ccTouchesMoved(touches, event);

	cocos2d::CCTouch *touch = (cocos2d::CCTouch *)touches->anyObject();
	cocos2d::CCPoint point = touch->getLocation();

	char string[64];
	sprintf_s(string, "X: %d, Y: %d", (int)point.x, (int)point.y);
	_printPos->setString(string);
}

void RegionDrawLayer::_MenuInputListener(cocos2d::CCObject *sender)
{
	cocos2d::CCMenuItemImage *item = (cocos2d::CCMenuItemImage *)sender;

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
		break;
	default: break;
	}
}