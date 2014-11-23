#include "EditorLayer.h"

#include <cocos2d.h>
#include "GameScene.h"

EditorLayer::EditorLayer(MapProjector* projector)
	: _isCreationAllowed(false)
	, _mapProjector(projector)
{
	init();
}

bool EditorLayer::init(void)
{
	if (!cocos2d::Layer::init())
	{
		return false;
	}
	
	cocos2d::EventListenerTouchAllAtOnce *touches = cocos2d::EventListenerTouchAllAtOnce::create();
	touches->onTouchesBegan = CC_CALLBACK_2(EditorLayer::TouchesBegan, this);
	touches->onTouchesMoved = CC_CALLBACK_2(EditorLayer::TouchesMoved, this);
	touches->onTouchesEnded = CC_CALLBACK_2(EditorLayer::TouchesEnded, this);
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touches, this);

	_touchPos.x = 0.0f;
	_touchPos.y = 0.0f;

	cocos2d::Director *director = cocos2d::Director::getInstance();
	Vector2 screen = director->getVisibleSize();
	Vector2 origin = director->getVisibleOrigin();

	cocos2d::TTFConfig ttfConfig("arial.ttf", 18);

	_printPos = cocos2d::Label::createWithTTF(ttfConfig, "X: 0, Y: 0", cocos2d::TextHAlignment::CENTER);
	_printPos->setPosition(Vector2(origin.x + 50, origin.y + screen.y - 150));
	_printPos->setAnchorPoint(Vector2(0.0f, 0.0f));

	_printNum = cocos2d::Label::createWithTTF(ttfConfig, "Num Points: 0", cocos2d::TextHAlignment::CENTER);
	_printNum->setPosition(Vector2(origin.x + 50, origin.y + screen.y - 190));
	_printNum->setAnchorPoint(Vector2(0.0f, 0.0f));

	_hullDrawer = cocos2d::DrawNode::create();
	addChild(_hullDrawer, 1.0f);

	{
		using namespace cocos2d;
		_btnToggle = cocos2d::MenuItemImage::create("btn-toggle-normal.png", "btn-toggle-selected.png",
			CC_CALLBACK_1(EditorLayer::_MenuInputListener, this));
		_btnDelete = cocos2d::MenuItemImage::create("btn-delete-normal.png", "btn-delete-selected.png",
			CC_CALLBACK_1(EditorLayer::_MenuInputListener, this));
		_btnSaveXml = cocos2d::MenuItemImage::create("btn-save-normal.png", "btn-save-selected.png",
			CC_CALLBACK_1(EditorLayer::_MenuInputListener, this));
		_btnReloadWorld = cocos2d::MenuItemImage::create("btn-reload-world-normal.png", "btn-reload-world-selected.png",
			CC_CALLBACK_1(EditorLayer::_MenuInputListener, this));
	}

	Vector2 pos;
	pos.x = origin.x + screen.x / 2.0f;
	pos.y = origin.y + screen.y - 100.0f;

	_btnToggle->setTag(MENU_ITEM_TOGGLE);
	_btnToggle->setPosition(pos - Vector2(-300.0f, 100.0f));
	_btnDelete->setTag(MENU_ITEM_DELETE);
	_btnDelete->setPosition(pos - Vector2(-300.0f, 150.0f));
	_btnSaveXml->setTag(MENU_ITEM_SAVE_XML);
	_btnSaveXml->setPosition(pos - Vector2(-300.0f, 200.0f));
	_btnReloadWorld->setTag(MENU_ITEM_RELOAD_WORLD);
	_btnReloadWorld->setPosition(pos - Vector2(-300.0f, 250.0f));

	cocos2d::Menu *menu = cocos2d::Menu::create(_btnToggle, _btnDelete, _btnSaveXml, _btnReloadWorld, NULL);
	menu->setPosition(0.0f, 0.0f);
	
	addChild(_printPos);
	addChild(_printNum);
	addChild(menu);

	scheduleUpdate();

	return true;
}

void EditorLayer::update(float dt)
{
}

void EditorLayer::TouchesBegan(const std::vector<cocos2d::Touch* > &touches, cocos2d::Event* event)
{
	cocos2d::Touch *touch = touches.at(0);

	if (_isCreationAllowed)
	{
		_hull1.PushPoint(_mapProjector->ProjectOnMap(touch->getLocation()));
	}
	else
	{
		cocos2d::Touch *touch = touches.at(0);
		_touchLastPoint = touch->getLocation();
	}

	/*
	char string[64];
	sprintf_s(string, "Num Points: %d", _hull1.GetPointsNum());
	_printNum->setString(string);
	*/

	Vector2 projected_point = _mapProjector->ProjectOnMap(touch->getLocation());
	_printPos->setString(cocos2d::StringUtils::format("X: %f, Y: %f", projected_point.x, projected_point.y));
}


void EditorLayer::TouchesEnded(const std::vector<cocos2d::Touch* > &touches, cocos2d::Event* event)
{
	cocos2d::Touch *touch = touches.at(0);
	_touchPos = touch->getLocation();

	Vector2 projected_point = _mapProjector->ProjectOnMap(touch->getLocation());
	_printPos->setString(cocos2d::StringUtils::format("X: %f, Y: %f", projected_point.x, projected_point.y));
}

void EditorLayer::TouchesMoved(const std::vector<cocos2d::Touch* > &touches, cocos2d::Event* event)
{
	cocos2d::Touch *touch = touches.at(0);
	Vector2 point = touch->getLocation();

	if (!_isCreationAllowed)
	{
		_mapProjector->ShiftView(-_touchLastPoint + touch->getLocation());
		_touchLastPoint = touch->getLocation();
	}

	//UpdateHullProjection();

	Vector2 projected_point = _mapProjector->ProjectOnMap(point);
	_printPos->setString(cocos2d::StringUtils::format("X: %f, Y: %f", projected_point.x, projected_point.y));
}

void EditorLayer::_MenuInputListener(cocos2d::Ref *sender)
{
	cocos2d::MenuItemImage *item = dynamic_cast<cocos2d::MenuItemImage*>(sender);

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
		SaveHullToXml(_hull1, "map/hulls.xml");
		break;
	default: break;
	}
}

void EditorLayer::SaveHullToXml(const ArbitraryHull& hull, pugi::xml_document &docXml)
{
	pugi::xml_node root = docXml.first_child();

	pugi::xml_node node;
	node = root.append_child("Region");
	node.append_attribute("Name").set_value("Unnamed");

	std::vector<Vector2> pointsArray(hull.GetPoints());
	for (std::vector<Vector2>::const_iterator it = pointsArray.begin(); it != pointsArray.end(); ++it)
	{
		Vector2 p = (*it);

		pugi::xml_node point = node.append_child("Vector2");
		point.append_attribute("x").set_value(p.x);
		point.append_attribute("y").set_value(p.y);
	}
}

bool EditorLayer::SaveHullToXml(const ArbitraryHull& hull, const char *xmlFilename)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(xmlFilename);
	
	if (result)
	{
		SaveHullToXml(hull, doc);
		doc.save_file(xmlFilename);
		return true;
	}
	
	return false;
}
