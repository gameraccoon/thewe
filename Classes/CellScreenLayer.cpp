#include "CellScreenLayer.h"

#include "GameScene.h"

#include "Color.h"
#include "TaskManager.h"
#include "World.h"
#include "Log.h"

#include "stdio.h"

CellScreenLayer::CellScreenLayer(Cell::WeakPtr cell, WorldMapLayer *worldMapLayer)
	: _worldMapLayer(worldMapLayer)
	, _cell(cell)
{
	init();
}

bool CellScreenLayer::init()
{
	if (!cocos2d::Layer::init())
	{
		return false;
	}

	cocos2d::Director *director = cocos2d::Director::sharedDirector();
	Vector2 screen = director->getVisibleSize();
	Vector2 origin = director->getVisibleOrigin();

	{
		using namespace cocos2d;
		_btnBack = cocos2d::MenuItemImage::create("btn-back-arrow-normal.png", "btn-back-arrow-selected.png",
			this, menu_selector(CellScreenLayer::_MenuInputListener));
		_btnCreteChild = cocos2d::MenuItemImage::create("btn-create-child-normal.png", "btn-create-child-selected.png",
			this, menu_selector(CellScreenLayer::_MenuInputListener));
	}

	Vector2 pos;
	pos.x = origin.x + screen.x / 2.0f;
	pos.y = origin.y + screen.y - 100.0f;

	_btnBack->setTag(MENU_ITEM_BACK);
	_btnBack->setScale(5.0f);
	_btnBack->setPosition(pos - Vector2(-820.0f, 100.0f));
	
	_btnCreteChild->setTag(MENU_ITEM_CREATE_CHILD);
	_btnCreteChild->setScale(4.0f);
	_btnCreteChild->setPosition(pos - Vector2(-700.0f, 320.0f));

/*
#ifdef CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	_membersText = cocos2d::LabelTTF::create("Not initialized", "Arial", 64);
	_membersText->setPosition(Vector2(450.0f, screen.y - 100.0f));
	addChild(_membersText, 1);
#elif CC_PLATFORM_ANDROID
	char membersInfo[64];
	sprintf_l(membersInfo, "%d members in the cell ", cell.membersNum);
	char cashInfo[64];
	sprintf_l(cashInfo, "Cash %.1f$", cell.cash);
	char moralInfo[64];
	sprintf_l(moralInfo, "Moral: %.1f percent", cell.morale * 100);
	char contentmentInfo[64];
	sprintf_l(contentmentInfo, "Contentment: %.1f percent", cell.contentment * 100);
	char hasParentInfo[64];
	sprintf_l(hasParentInfo, cell.parent != nullptr ? "Has parent" : "Hasn't parent");
	char childCountInfo[64];
	sprintf_l(childCountInfo, "Childs: %d", _cell->GetChildrens().size());
#endif

	_cashText = cocos2d::LabelTTF::create("Not initialized", "Arial", 64);
	_cashText->setPosition(Vector2(450.0f, screen.y - 200.0f));
	addChild(_cashText, 1);
*/
	_moralText = cocos2d::LabelTTF::create("Not initialized", "Arial", 64);
	_moralText->setPosition(Vector2(450.0f, screen.y - 300.0f));
	addChild(_moralText, 1);

	_contentmentText = cocos2d::LabelTTF::create("Not initialized", "Arial", 64);
	_contentmentText->setPosition(Vector2(450.0f, screen.y - 400.0f));
	addChild(_contentmentText, 1);

	_hasParentText = cocos2d::LabelTTF::create("Not initialized", "Arial", 64);
	_hasParentText->setPosition(Vector2(450.0f, screen.y - 500.0f));
	addChild(_hasParentText, 1);

	_childCountText = cocos2d::LabelTTF::create("Not initialized", "Arial", 64);
	_childCountText->setPosition(Vector2(450.0f, screen.y - 600.0f));
	addChild(_childCountText, 1);

	cocos2d::Menu *menu = cocos2d::Menu::create(_btnBack, _btnCreteChild, NULL);
	menu->setPosition(0.0f, 0.0f);
	_currentTaskText = cocos2d::LabelTTF::create("Not initialized", "Arial", 64);
	_currentTaskText->setPosition(Vector2(450.0f, screen.y - 700.0f));
	addChild(_currentTaskText, 1);

	_bkgDraw = cocos2d::DrawNode::create();
	_InitBackground(_bkgDraw);
	addChild(_bkgDraw, 0);

	_availableTasks = TaskManager::Instance().GetAvailableTasks(_cell);

	cocos2d::Vector<cocos2d::MenuItem*> taskMenuItems;
	float tasksPosY = 0.0f;
	int index = 0;
	for (const Task::Info* task : _availableTasks)
	{
		cocos2d::LabelTTF* newLabel = cocos2d::LabelTTF::create(task->id.c_str(), "Arial", 64);
		cocos2d::MenuItemLabel* newItem;
		{
			using namespace cocos2d;
			newItem = cocos2d::MenuItemLabel::create(newLabel, this
				, menu_selector(CellScreenLayer::_TaskClickListener));
		}
		newItem->setPosition(Vector2(0.0f, screen.y - (tasksPosY + 100.0f * index)));
		newItem->setTag(index);

		taskMenuItems.pushBack(newItem);
		index++;
	}
	
	cocos2d::Menu *tasksMenu = cocos2d::Menu::createWithArray(taskMenuItems);
	tasksMenu->setPosition(1700.0f, -600.0f);
	addChild(tasksMenu, 1);

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
/*
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
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
	sprintf_s(childCountInfo, "Childs: %d", _cell->GetChildrens().size());
#else CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	char membersInfo[64];
	sprintf_l(membersInfo, "%d members in the cell ", cell.membersNum);
	char cashInfo[64];
	sprintf_l(cashInfo, "Cash %.1f$", cell.cash);
	char moralInfo[64];
	sprintf_l(moralInfo, "Moral: %.1f percent", cell.morale * 100);
	char contentmentInfo[64];
	sprintf_l(contentmentInfo, "Contentment: %.1f percent", cell.contentment * 100);
	char hasParentInfo[64];
	sprintf_l(hasParentInfo, cell.parent != nullptr ? "Has parent" : "Hasn't parent");
	char childCountInfo[64];
	sprintf_l(childCountInfo, "Childs: %d", _cell->GetChildrens().size());
#endif

	_membersText->setString(membersInfo);
	_cashText->setString(cashInfo);
	_moralText->setString(moralInfo);
	_contentmentText->setString(contentmentInfo);
	_hasParentText->setString(hasParentInfo);
	_childCountText->setString(childCountInfo);
	*/

	if (!cell->getCurrentTask().expired())
	{
		//Task* task = cell->getCurrentTask()._Get();
		//char curentTaskInfo[64];
		//sprintf_s(curentTaskInfo, "Task: %s progress: %.0f percent",
			//task->GetInfo()->id.c_str(), task->CalculateProgress(World::Instance().GetWorldTime()) * 100.0f);
		//_currentTaskText->setString(curentTaskInfo);
	}
	else
	{
		_currentTaskText->setString("No current tasks");
	}
}

void CellScreenLayer::_MenuInputListener(cocos2d::Object *sender)
{
	cocos2d::MenuItem *item = dynamic_cast<cocos2d::MenuItem*>(sender);

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

void CellScreenLayer::_InitBackground(cocos2d::DrawNode *background) const
{
	if (!background)
	{
		return;
	}

	Vector2 screen = cocos2d::Director::sharedDirector()->getVisibleSize();
	Vector2 origin = cocos2d::Director::sharedDirector()->getVisibleOrigin();

	cocos2d::CCPoint vertices[4];
	vertices[0] = origin;
	vertices[1] = origin + cocos2d::Point(screen.x, 0.0f);
	vertices[2] = origin + cocos2d::Point(screen.x, screen.y);
	vertices[3] = origin + cocos2d::Point(0.0f, screen.y);

	Color fill(0.3f, 0.3f, 0.3f, 0.7f);
	Color border(0.6f, 0.6f, 0.6f, 0.7f);
	
	background->drawPolygon(vertices, 4, fill, 50.0f, border);
}

void CellScreenLayer::_TaskClickListener(cocos2d::Object *sender)
{
	cocos2d::MenuItem *item = dynamic_cast<cocos2d::MenuItem*>(sender);

	int index = item->getTag();

	TaskManager::Instance().RunTask(_cell, _availableTasks[index], World::Instance().GetWorldTime());
}
