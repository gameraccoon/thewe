#include "CellScreenLayer.h"

#include "GameScene.h"

#include "Color.h"

CellScreenLayer::CellScreenLayer(Cell::Ptr cell)
{
	_cell = cell;
	init();
}

bool CellScreenLayer::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	cocos2d::CCDirector *director = cocos2d::CCDirector::sharedDirector();
	Point screen = director->getVisibleSize();
	Point origin = director->getVisibleOrigin();

	{
		using namespace cocos2d;
		_btnBack = cocos2d::CCMenuItemImage::create("btn-back-arrow-normal.png", "btn-back-arrow-selected.png",
			this, menu_selector(CellScreenLayer::_MenuInputListener));
	}

	Point pos;
	pos.x = origin.x + screen.x / 2.0f;
	pos.y = origin.y + screen.y - 100.0f;

	_btnBack->setTag(MENU_ITEM_BACK);
	_btnBack->setScale(5.0f);
	_btnBack->setPosition(pos - ccp(-820.0f, 100.0f));

	char membersInfo[64];
	sprintf_s(membersInfo, "%d members in the cell ", _cell->GetMembersCount());
	char cashInfo[64];
	sprintf_s(cashInfo, "Cash %.1f$", _cell->GetCash());
	char moralInfo[64];
	sprintf_s(moralInfo, "Moral: %.1f percent", _cell->GetMoralValue() * 100);
	char contentmentInfo[64];
	sprintf_s(contentmentInfo, "Contentment: %.1f percent", _cell->GetContentment() * 100);
	char childCountInfo[64];
	sprintf_s(childCountInfo, "Childs: %d", _cell->GetChildren().size());
	char hasParentInfo[64];
	sprintf_s(hasParentInfo, _cell->GetParent() != nullptr ? "Has parent" : "Hasn't parent");

	_membersText = cocos2d::CCLabelTTF::create(membersInfo, "Arial", 64);
	_cashText = cocos2d::CCLabelTTF::create(cashInfo, "Arial", 64);
	_moralText = cocos2d::CCLabelTTF::create(moralInfo, "Arial", 64);
	_contentmentText = cocos2d::CCLabelTTF::create(contentmentInfo, "Arial", 64);
	_childCountText = cocos2d::CCLabelTTF::create(childCountInfo, "Arial", 64);
	_hasParentText = cocos2d::CCLabelTTF::create(hasParentInfo, "Arial", 64);

	_membersText->setPosition(Point(450.0f, screen.y - 100.0f));
	_cashText->setPosition(Point(450.0f, screen.y - 200.0f));
	_moralText->setPosition(Point(450.0f, screen.y - 300.0f));
	_contentmentText->setPosition(Point(450.0f, screen.y - 400.0f));
	_childCountText->setPosition(Point(450.0f, screen.y - 500.0f));
	_hasParentText->setPosition(Point(450.0f, screen.y - 600.0f));

	cocos2d::CCMenu *menu = cocos2d::CCMenu::create(_btnBack, NULL);
	menu->setPosition(0.0f, 0.0f);

	_bkgDraw = cocos2d::CCDrawNode::create();
	_InitBackground(_bkgDraw);
	
	addChild(_bkgDraw, 0);
	addChild(menu, 1);
	addChild(_membersText, 1);
	addChild(_cashText, 1);
	addChild(_moralText, 1);
	addChild(_contentmentText, 1);
	addChild(_childCountText, 1);
	addChild(_hasParentText, 1);
	setTouchEnabled(true);

	return true;
}

void CellScreenLayer::ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	cocos2d::CCLayer::ccTouchesBegan(touches, event);
}

void CellScreenLayer::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	cocos2d::CCLayer::ccTouchesEnded(touches, event);
}

void CellScreenLayer::ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	cocos2d::CCLayer::ccTouchesMoved(touches, event);
}

void CellScreenLayer::_MenuInputListener(cocos2d::CCObject *sender)
{
	cocos2d::CCMenuItemImage *item = dynamic_cast<cocos2d::CCMenuItemImage*>(sender);

	int tag = item->getTag();

	switch (tag)
	{
	case MENU_ITEM_BACK:
		dynamic_cast<GameScene*>(getParent())->ShowMap();
		break;
	default: break;
	}
}

void CellScreenLayer::_InitBackground(cocos2d::CCDrawNode *background) const
{
	if (!background)
	{
		return;
	}

	cocos2d::CCPoint screen = cocos2d::CCDirector::sharedDirector()->getVisibleSize();
	cocos2d::CCPoint origin = cocos2d::CCDirector::sharedDirector()->getVisibleOrigin();

	cocos2d::CCPoint vertices[4];
	vertices[0] = origin;
	vertices[1] = origin + cocos2d::CCPoint(screen.x, 0.0f);
	vertices[2] = origin + cocos2d::CCPoint(screen.x, screen.y);
	vertices[3] = origin + cocos2d::CCPoint(0.0f, screen.y);

	Color fill(0.3f, 0.3f, 0.3f, 0.7f);
	Color border(0.6f, 0.6f, 0.6f, 0.7f);
	
	background->drawPolygon(vertices, 4, fill, 50.0f, border);
}