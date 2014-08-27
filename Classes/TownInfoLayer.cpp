#include "TownInfoLayer.h"

TownInfoLayer::TownInfoLayer(void)
{
	init();
}

bool TownInfoLayer::init()
{
	if (!cocos2d::Layer::init())
	{
		return false;
	}

	cocos2d::CCDrawNode *_bkg = cocos2d::CCDrawNode::create();

	const float width = 800.0f;
	const float height = 600.0f;

	cocos2d::Point origin = cocos2d::Director::sharedDirector()->getVisibleOrigin();

	cocos2d::Point vertices[4];
	vertices[0] = origin;
	vertices[1] = origin + cocos2d::Point(width, 0.0f);
	vertices[2] = origin + cocos2d::Point(width, height);
	vertices[3] = origin + cocos2d::Point(0.0f, height);

	Color fill(0.3f, 0.3f, 0.3f, 0.7f);
	Color border(0.6f, 0.6f, 0.6f, 0.7f);
	
	_bkg->drawPolygon(vertices, 4, fill, 10.0f, border);

	_nameText = cocos2d::LabelTTF::create("", "Arial", 64);
	_populText = cocos2d::LabelTTF::create("", "Arial", 64);
	_descText = cocos2d::LabelTTF::create("", "Arial", 32);

	_nameText->setPosition(cocos2d::Point(400.0f, 500.0f));
	_populText->setPosition(cocos2d::Point(400.0f, 400.0f));
	_descText->setPosition(cocos2d::Point(400.0f, 100.0f));
	_descText->setDimensions(Vector2(500.0f, 500.0f));
	_descText->setHorizontalAlignment(cocos2d::kCCTextAlignmentCenter);

	addChild(_bkg, 0);
	addChild(_nameText, 1);
	addChild(_populText, 1);
	addChild(_descText, 1);

	return true;
}
	
void TownInfoLayer::SelectTown(Town::WeakPtr town)
{
	if (!town.expired())
	{
		_town = town;
		//Town::Info info = _town._Get()->GetInfo();

		/*
#ifdef CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
		char name[64];
		sprintf_s(name, "Town %s", info.name.c_str());
		char popul[64];
		sprintf_s(popul, "Population %.3f millions", info.population);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		char name[64];
		sprintf_l(name, "Town %s", info.name.c_str());
		char popul[64];
		sprintf_l(popul, "Population %.3f millions", info.population);
#endif
		_nameText->setString(name);
		_populText->setString(popul);
		_descText->setString(info.desc.c_str());
		*/

	}
}
