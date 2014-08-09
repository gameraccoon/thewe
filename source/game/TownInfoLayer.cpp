#include "TownInfoLayer.h"

TownInfoLayer::TownInfoLayer(void)
{
	init();
}

bool TownInfoLayer::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	cocos2d::CCDrawNode *_bkg = cocos2d::CCDrawNode::create();

	const float width = 800.0f;
	const float height = 600.0f;

	cocos2d::CCPoint origin = cocos2d::CCDirector::sharedDirector()->getVisibleOrigin();

	cocos2d::CCPoint vertices[4];
	vertices[0] = origin;
	vertices[1] = origin + cocos2d::CCPoint(width, 0.0f);
	vertices[2] = origin + cocos2d::CCPoint(width, height);
	vertices[3] = origin + cocos2d::CCPoint(0.0f, height);

	Color fill(0.3f, 0.3f, 0.3f, 0.7f);
	Color border(0.6f, 0.6f, 0.6f, 0.7f);
	
	_bkg->drawPolygon(vertices, 4, fill, 10.0f, border);

	_nameText = cocos2d::CCLabelTTF::create("", "Arial", 64);
	_populText = cocos2d::CCLabelTTF::create("", "Arial", 64);
	_descText = cocos2d::CCLabelTTF::create("", "Arial", 32);

	_nameText->setPosition(cocos2d::CCPoint(400.0f, 500.0f));
	_populText->setPosition(cocos2d::CCPoint(400.0f, 400.0f));
	_descText->setPosition(cocos2d::CCPoint(400.0f, 100.0f));
	_descText->setDimensions(Point(500.0f, 500.0f));
	_descText->setHorizontalAlignment(cocos2d::CCTextAlignment::kCCTextAlignmentCenter);

	addChild(_bkg, 0);
	addChild(_nameText, 1);
	addChild(_populText, 1);
	addChild(_descText, 1);

	return true;
}
	
void TownInfoLayer::ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
}

void TownInfoLayer::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
}

void TownInfoLayer::ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
}

void TownInfoLayer::SelectTown(Town::Ptr town)
{
	if (town)
	{
		_town = town;
		Town::Info info = _town->GetInfo();

		char name[64];
		sprintf_s(name, "Town %s", info.name.c_str());
		char popul[64];
		sprintf_s(popul, "Population %.3f millions", info.population);

		_nameText->setString(name);
		_populText->setString(popul);
		_descText->setString(info.desc.c_str());
	}
}