#include "RegionDrawLayer.h"

RegionDrawLayer::RegionDrawLayer(void)
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

	addChild(_printPos);
	addChild(_printNum);
	setTouchEnabled(true);
	
	//CCLayer::schedule(schedule_selector(WorldMapLayer::_IdleUpdate));

	_hull1.AddPoint(ccp(100, 100));
	_hull1.AddPoint(ccp(500, 900));
	_hull1.AddPoint(ccp(700, 1000));
	_hull1.AddPoint(ccp(900, 950));
	_hull1.AddPoint(ccp(900, 750));
	_hull1.AddPoint(ccp(600, 750));
	_hull1.AddPoint(ccp(600, 400));
	_hull1.AddPoint(ccp(900, 400));

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
	cocos2d::CCTouch *touch = (cocos2d::CCTouch *)touches->anyObject();

	_hull1.AddPoint(touch->getLocation());

	char string[64];
	sprintf_s(string, "Num Points: %d", _hull1.GetPointsNum());
	_printNum->setString(string);
}

void RegionDrawLayer::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	cocos2d::CCTouch *touch = (cocos2d::CCTouch *)touches->anyObject();
	_touchPos = touch->getLocation();

	if (_hull1.Contain(_touchPos))
	{
		_isPointInHull = !_isPointInHull;
	}
}

void RegionDrawLayer::ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	cocos2d::CCTouch *touch = (cocos2d::CCTouch *)touches->anyObject();
	cocos2d::CCPoint point = touch->getLocation();

	char string[64];
	sprintf_s(string, "X: %d, Y: %d", (int)point.x, (int)point.y);
	_printPos->setString(string);
}