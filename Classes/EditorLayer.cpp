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
	
	_touchPos.x = 0.0f;
	_touchPos.y = 0.0f;

	cocos2d::Director *director = cocos2d::Director::getInstance();
	Vector2 screen = director->getVisibleSize();
	Vector2 origin = director->getVisibleOrigin();

	_printPos = cocos2d::LabelTTF::create("X: 0, Y: 0", "Arial", 32);
	_printPos->setPosition(Vector2(origin.x + 200, origin.y + screen.y - 100));

	_printNum = cocos2d::LabelTTF::create("Num Points: 0", "Arial", 32);
	_printNum->setPosition(Vector2(origin.x + 200, origin.y + screen.y - 150));

	_hullDrawer = cocos2d::DrawNode::create();
	addChild(_hullDrawer, 1.0f);

	{
		using namespace cocos2d;
		_btnToggle = cocos2d::MenuItemImage::create("btn-toggle-normal.png", "btn-toggle-selected.png",
			this, menu_selector(EditorLayer::_MenuInputListener));
		_btnDelete = cocos2d::MenuItemImage::create("btn-delete-normal.png", "btn-delete-selected.png",
			this, menu_selector(EditorLayer::_MenuInputListener));
		_btnSaveXml = cocos2d::MenuItemImage::create("btn-save-normal.png", "btn-save-selected.png",
			this, menu_selector(EditorLayer::_MenuInputListener));
		_btnReloadWorld = cocos2d::MenuItemImage::create("btn-reload-world-normal.png", "btn-reload-world-selected.png",
			this, menu_selector(EditorLayer::_MenuInputListener));
	}

	Vector2 pos;
	pos.x = origin.x + screen.x / 2.0f;
	pos.y = origin.y + screen.y - 100.0f;

	_btnToggle->setScale(4.0f);
	_btnToggle->setTag(MENU_ITEM_TOGGLE);
	_btnToggle->setPosition(pos - Vector2(-700.0f, 100.0f));
	_btnDelete->setScale(4.0f);
	_btnDelete->setTag(MENU_ITEM_DELETE);
	_btnDelete->setPosition(pos - Vector2(-700.0f, 300.0f));
	_btnSaveXml->setScale(4.0f);
	_btnSaveXml->setTag(MENU_ITEM_SAVE_XML);
	_btnSaveXml->setPosition(pos - Vector2(-700.0f, 500.0f));
	_btnReloadWorld->setScale(4.0f);
	_btnReloadWorld->setTag(MENU_ITEM_RELOAD_WORLD);
	_btnReloadWorld->setPosition(pos - Vector2(-700.0f, 700.0f));

	cocos2d::Menu *menu = cocos2d::Menu::create(_btnToggle, _btnDelete, _btnSaveXml, _btnReloadWorld, NULL);
	menu->setPosition(0.0f, 0.0f);
	
	addChild(_printPos);
	addChild(_printNum);
	addChild(menu);
	setTouchEnabled(true);

	this->scheduleUpdate();

	return true;
}

void EditorLayer::update(float dt)
{
	ArbitraryHull hull = _mapProjector->ProjectOnScreen(_hull1);
	std::vector<cocos2d::Point> &ps = const_cast<std::vector<cocos2d::Point> &>(hull.GetPointsArray());
	if (!ps.empty())
	{
		_hullDrawer->clear();
		_hullDrawer->drawPolygon(&(*ps.begin()), ps.size(), cocos2d::Color4F(0,0,0,0), 3.5f, cocos2d::Color4F(0.0f, 0.0f, 1.0f, 1.0f));
	}
}

void EditorLayer::onTouchesBegan(const std::vector<cocos2d::Touch* > &touches, cocos2d::Event* event)
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
}


void EditorLayer::onTouchesEnded(const std::vector<cocos2d::Touch* > &touches, cocos2d::Event* event)
{
	cocos2d::Touch *touch = touches.at(0);
	_touchPos = touch->getLocation();
}

void EditorLayer::onTouchesMoved(const std::vector<cocos2d::Touch* > &touches, cocos2d::Event* event)
{
	cocos2d::Touch *touch = touches.at(0);
	Vector2 point = touch->getLocation();

	if (!_isCreationAllowed)
	{
		_mapProjector->ShiftView(-_touchLastPoint + touch->getLocation());
		_touchLastPoint = touch->getLocation();
	}

	//UpdateHullProjection();
/*
	char string[64];
	Vector2 projected_point = _mapProjector->ProjectOnMap(point);
	sprintf_s(string, "X: %d, Y: %d", (int)projected_point.x, (int)projected_point.y);
	_printPos->setString(string);
	*/
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
		{
		_hull1.PopPoint();
		std::vector<cocos2d::Point> &ps = const_cast<std::vector<cocos2d::Point> &>(_hull1.GetPointsArray());
		_hullDrawer->clear();
		_hullDrawer->drawPolygon(&(*ps.begin()), ps.size(), cocos2d::Color4F(0,0,0,0), 1.5f, cocos2d::Color4F(0.0f, 0.0f, 1.0f, 1.0f));
		break;
		}
	case MENU_ITEM_SAVE_XML:
		//SaveHullToXml(_hull1, "../_gamedata/map/hulls.xml");
		SaveHullToXml(_hull1, "map/hulls.xml");
		break;
	case MENU_ITEM_RELOAD_WORLD:
		WorldLoader::LoadGameInfo();
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

	for (std::vector<cocos2d::CCPoint>::const_iterator it = hull._pointsArray.begin(); it != hull._pointsArray.end(); ++it)
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
