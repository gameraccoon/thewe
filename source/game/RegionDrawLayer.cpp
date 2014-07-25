#include "RegionDrawLayer.h"

#include "WorldMap.h"

RegionDrawLayer::RegionDrawLayer(void)
	: _isCreationAllowed(true)
	, _mapProjector(cocos2d::CCPoint(0.0f, 0.0f), 2.5f)
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

	// сообщаем где находится центр окна вывода
	_mapProjector.SetScreenCenter(origin + screen / 2.0f);
	// ставим спрайт карты ровно в центр экрана
	_mapProjector.SetShift(origin + screen / 2.0f);

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
	
	CCLayer::addChild(_mapProjector.GetSprite());
	addChild(_printPos);
	addChild(_printNum);
	addChild(menu);
	setTouchEnabled(true);

	return true;
}

void RegionDrawLayer::visit(void)
{
	CCLayer::visit();

	if (_isCreationAllowed)
	{
		ArbitraryHull visibleHull;
		for (auto &point : _hull1.GetPoints())
		{
			visibleHull.PushPoint(_mapProjector.ProjectOnScreen(point));
		}

		visibleHull.Draw();
	}
	else
	{
		for (auto regionIterator : WorldMap::Instance().GetRegions())
		{
			ArbitraryHull hull = regionIterator.second->GetHull();
			ArbitraryHull projectedHull;
			for (auto &point : hull.GetPoints())
			{
				projectedHull.PushPoint(_mapProjector.ProjectOnScreen(point));
			}
			projectedHull.Draw();
		}
	}
}

void RegionDrawLayer::ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	cocos2d::CCLayer::ccTouchesBegan(touches, event);
	
	cocos2d::CCTouch *touch = dynamic_cast<cocos2d::CCTouch*>(touches->anyObject());

	if (_isCreationAllowed)
	{
		_hull1.PushPoint(_mapProjector.ProjectOnMap(touch->getLocation()));
	}
	else
	{
		_touchLastPoint = touch->getLocation();
	}

	char string[64];
	sprintf_s(string, "Num Points: %d", _hull1.GetPointsNum());
	_printNum->setString(string);
}

void RegionDrawLayer::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	cocos2d::CCLayer::ccTouchesEnded(touches, event);

	cocos2d::CCTouch *touch = dynamic_cast<cocos2d::CCTouch*>(touches->anyObject());
	_touchPos = touch->getLocation();
}

void RegionDrawLayer::ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	cocos2d::CCLayer::ccTouchesMoved(touches, event);
	
	cocos2d::CCTouch *touch = dynamic_cast<cocos2d::CCTouch*>(touches->anyObject());
	cocos2d::CCPoint point = touch->getLocation();

	if (!_isCreationAllowed)
	{
		_mapProjector.SetShift(_mapProjector.GetShift() - _touchLastPoint + touch->getLocation());
		_touchLastPoint = touch->getLocation();
	}

	char string[64];
	sprintf_s(string, "X: %d, Y: %d", (int)point.x, (int)point.y);
	_printPos->setString(string);
}

void RegionDrawLayer::_MenuInputListener(cocos2d::CCObject *sender)
{
	cocos2d::CCMenuItemImage *item = dynamic_cast<cocos2d::CCMenuItemImage*>(sender);

	int tag = item->getTag();

	switch (tag)
	{
	case MENU_ITEM_TOGGLE:
		if (_isCreationAllowed)
		{
			FinalizeRegion("Italy", _hull1);
		}
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

void RegionDrawLayer::FinalizeRegion(std::string regionName, ArbitraryHull hull)
{
	Region::Ptr region = WorldMap::Instance().GetRegion(regionName);
	if (!region)
	{
		WorldMap::Instance().CreateRegion(regionName);
		region = WorldMap::Instance().GetRegion(regionName);
	}

	region->SetHull(_hull1);
}