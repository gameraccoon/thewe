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

	cocos2d::DrawNode *_bkg = cocos2d::DrawNode::create();

	const float width = 400;
	const float height = 200.0f;

	cocos2d::Point origin = cocos2d::Director::getInstance()->getVisibleOrigin();

	cocos2d::Point vertices[4];
	vertices[0] = origin;
	vertices[1] = origin + cocos2d::Point(width, 0.0f);
	vertices[2] = origin + cocos2d::Point(width, height);
	vertices[3] = origin + cocos2d::Point(0.0f, height);

	Color fill(0.3f, 0.3f, 0.3f, 0.7f);
	Color border(0.6f, 0.6f, 0.6f, 0.7f);
	
	_bkg->drawPolygon(vertices, 4, fill, 10.0f, border);

	_nameText = cocos2d::LabelTTF::create("", "Arial", 25);
	_populText = cocos2d::LabelTTF::create("", "Arial", 25);
	_descText = cocos2d::LabelTTF::create("", "Arial", 15);

	_nameText->setPosition(cocos2d::Point(200.0f, 167.0f));
	_populText->setPosition(cocos2d::Point(200.0f, 135.0f));
	_descText->setPosition(cocos2d::Point(200.0f, 50.0f));
	_descText->setDimensions(Vector2(370.0f, 100.0f));
	_descText->setHorizontalAlignment(cocos2d::TextHAlignment::CENTER);

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
		Town::Info info = _town.lock()->GetInfo();

		std::stringstream ss;

		ss.str(std::string());
		ss.clear();
		ss << "Town " << info.name.c_str();
		_nameText->setString(ss.str().c_str());
	
		ss.str(std::string());
		ss.clear();
		ss << "Population " << info.population << " millions";
		_populText->setString(ss.str().c_str());

		ss.str(std::string());
		ss.clear();
		ss << info.desc.c_str();
		_descText->setString(ss.str().c_str());
	}
}
