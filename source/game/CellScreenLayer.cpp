#include "CellScreenLayer.h"

#include "GameScene.h"

#include "Color.h"
#include "TaskManager.h"
#include "World.h"
#include "Log.h"

CellScreenLayer::CellScreenLayer(Cell::WeakPtr cell, WorldMapLayer *worldMapLayer)
	: _worldMapLayer(worldMapLayer)
	, _cell(cell)
{
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
		_btnCreteChild = cocos2d::CCMenuItemImage::create("btn-create-child-normal.png", "btn-create-child-selected.png",
			this, menu_selector(CellScreenLayer::_MenuInputListener));
	}

	Point pos;
	pos.x = origin.x + screen.x / 2.0f;
	pos.y = origin.y + screen.y - 100.0f;

	_btnBack->setTag(MENU_ITEM_BACK);
	_btnBack->setScale(5.0f);
	_btnBack->setPosition(pos - ccp(-820.0f, 100.0f));
	
	_btnCreteChild->setTag(MENU_ITEM_CREATE_CHILD);
	_btnCreteChild->setScale(4.0f);
	_btnCreteChild->setPosition(pos - ccp(-700.0f, 320.0f));

	_membersText = cocos2d::CCLabelTTF::create("Not initialized", "Arial", 64);
	_membersText->setPosition(Point(450.0f, screen.y - 100.0f));
	addChild(_membersText, 1);

	_cashText = cocos2d::CCLabelTTF::create("Not initialized", "Arial", 64);
	_cashText->setPosition(Point(450.0f, screen.y - 200.0f));
	addChild(_cashText, 1);

	_moralText = cocos2d::CCLabelTTF::create("Not initialized", "Arial", 64);
	_moralText->setPosition(Point(450.0f, screen.y - 300.0f));
	addChild(_moralText, 1);

	_contentmentText = cocos2d::CCLabelTTF::create("Not initialized", "Arial", 64);
	_contentmentText->setPosition(Point(450.0f, screen.y - 400.0f));
	addChild(_contentmentText, 1);

	_hasParentText = cocos2d::CCLabelTTF::create("Not initialized", "Arial", 64);
	_hasParentText->setPosition(Point(450.0f, screen.y - 500.0f));
	addChild(_hasParentText, 1);

	_childCountText = cocos2d::CCLabelTTF::create("Not initialized", "Arial", 64);
	_childCountText->setPosition(Point(450.0f, screen.y - 600.0f));
	addChild(_childCountText, 1);

	_currentTaskText = cocos2d::CCLabelTTF::create("Not initialized", "Arial", 64);
	_currentTaskText->setPosition(Point(450.0f, screen.y - 700.0f));
	addChild(_currentTaskText, 1);

	cocos2d::CCMenu *menu = cocos2d::CCMenu::create(_btnBack, _btnCreteChild, NULL);
	menu->setPosition(0.0f, 0.0f);

	_bkgDraw = cocos2d::CCDrawNode::create();
	_InitBackground(_bkgDraw);
	
	addChild(_bkgDraw, 0);
	addChild(menu, 1);
	setTouchEnabled(true);

	scheduleUpdate();

	return true;
}

void CellScreenLayer::update(float delta)
{
	Cell::Ptr cell = _cell.lock();
	
	if (!cell)
	{
		Log::Instance().writeError("Cell has been deleted on showind it info screen");
	}

	const Cell::Info& cellInfo = _cell._Get()->GetInfo();

	char membersInfo[64];
	sprintf_s(membersInfo, "%d members in the cell ", cellInfo.membersCount);
	_membersText->setString(membersInfo);

	char cashInfo[64];
	sprintf_s(cashInfo, "Cash %.1f$", cellInfo.cash);
	_cashText->setString(cashInfo);

	char moralInfo[64];
	sprintf_s(moralInfo, "Moral: %.1f percent", cellInfo.morale * 100);
	_moralText->setString(moralInfo);

	char contentmentInfo[64];
	sprintf_s(contentmentInfo, "Contentment: %.1f percent", cellInfo.contentment * 100);
	_contentmentText->setString(contentmentInfo);

	char hasParentInfo[64];
	sprintf_s(hasParentInfo, cellInfo.parent != nullptr ? "Has parent" : "Hasn't parent");
	_hasParentText->setString(hasParentInfo);

	char childCountInfo[64];
	sprintf_s(childCountInfo, "Childs: %d", cell->GetChildrens().size());
	_childCountText->setString(childCountInfo);

	
	if (!cell->getCurrentTask().expired())
	{
		Task* task = cell->getCurrentTask()._Get();
		char curentTaskInfo[64];
		sprintf_s(curentTaskInfo, "Task: %s progress: %.0f percent",
			task->GetInfo()->id.c_str(), task->CalculateProgress(World::Instance().GetWorldTime()) * 100.0f);
		_currentTaskText->setString(curentTaskInfo);
	}
	else
	{
		_currentTaskText->setString("No current tasks");
	}
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
		dynamic_cast<GameScene *>(getParent())->ShowMap();
		break;
	case MENU_ITEM_CREATE_CHILD:
		_worldMapLayer->SetNextCellParent(_cell);
		dynamic_cast<GameScene *>(getParent())->ShowMap();
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