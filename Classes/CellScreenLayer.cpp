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
	
	{
		using namespace cocos2d;
		_btnBack = cocos2d::MenuItemImage::create("btn-back-arrow-normal.png", "btn-back-arrow-selected.png",
			this, menu_selector(CellScreenLayer::_MenuInputListener));
		_btnCreteChild = cocos2d::MenuItemImage::create("btn-create-child-normal.png", "btn-create-child-selected.png",
			this, menu_selector(CellScreenLayer::_MenuInputListener));
	}

	cocos2d::Director *director = cocos2d::Director::getInstance();
	Vector2 screen = director->getVisibleSize();
	Vector2 origin = director->getVisibleOrigin();

	Vector2 pos;
	pos.x = origin.x + screen.x / 2.0f;
	pos.y = origin.y + screen.y - 100.0f;

	_btnBack->setTag(MENU_ITEM_BACK);
	_btnBack->setScale(2.0f);
	_btnBack->setPosition(Vector2(70.0f, 65.0f));
	
	_btnCreteChild->setTag(MENU_ITEM_CREATE_CHILD);
	_btnCreteChild->setScale(2.0f);
	_btnCreteChild->setPosition(Vector2(260.0f, 65.0f));

	Cell::Info cell = _cell.lock()->GetInfo();

	std::stringstream ss;

	ss.str(std::string());
	ss.clear();
	ss << cell.membersCount << " members in the cell";

	_membersText = cocos2d::LabelTTF::create(ss.str().c_str(), "Arial", 30);
	_membersText->setPosition(Vector2(235.0f, screen.y - 50.0f));
	addChild(_membersText, 1);

	ss.str(std::string());
	ss.clear();
	ss << "Cash " << cell.cash; 

	_cashText = cocos2d::LabelTTF::create(ss.str().c_str(), "Aral", 30);
	_cashText->setPosition(Vector2(235.0f, screen.y - 100.0f));
	addChild(_cashText, 1);

	ss.str(std::string());
	ss.clear();
	ss << "Morale " << cell.morale << " percent";

	_moralText = cocos2d::LabelTTF::create(ss.str().c_str(), "Arial", 30);
	_moralText->setPosition(Vector2(235.0f, screen.y - 150.0f));
	addChild(_moralText, 1);

	ss.str(std::string());
	ss.clear();
	ss << "Contentment " << cell.contentment << " percent";

	_contentmentText = cocos2d::LabelTTF::create(ss.str().c_str(), "Arial", 30);
	_contentmentText->setPosition(Vector2(235.0f, screen.y - 200));
	addChild(_contentmentText, 1);

	ss.str(std::string());
	ss.clear();
	ss << "Childs " << _cell.lock()->GetChildrens().size() << " percent";

	_hasParentText = cocos2d::LabelTTF::create(ss.str().c_str(), "Arial", 30);
	_hasParentText->setPosition(Vector2(235.0f, screen.y - 250));
	addChild(_hasParentText, 1);

	_childCountText = cocos2d::LabelTTF::create(cell.parent != nullptr ? "Has parent" : "Hasn't parent", "Arial", 30);
	_childCountText->setPosition(Vector2(235.0f, screen.y - 300));
	addChild(_childCountText, 1);

	cocos2d::Menu *menu = cocos2d::Menu::create(_btnBack, _btnCreteChild, NULL);
	menu->setPosition(0.0f, 0.0f);
	addChild(menu, 2);

	_currentTaskText = cocos2d::LabelTTF::create("Not initialized", "Arial", 30);
	_currentTaskText->setPosition(Vector2(235.0f, screen.y - 350));
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
		cocos2d::LabelTTF* newLabel = cocos2d::LabelTTF::create(task->id.c_str(), "Arial", 30);
		cocos2d::MenuItemLabel* newItem;
		{
			using namespace cocos2d;
			newItem = cocos2d::MenuItemLabel::create(newLabel, this
				, menu_selector(CellScreenLayer::_TaskClickListener));
		}
		newItem->setPosition(Vector2(0.0f, tasksPosY + 40.0f * index));
		newItem->setTag(index);

		taskMenuItems.pushBack(newItem);
		index++;
	}
	
	cocos2d::Menu *tasksMenu = cocos2d::Menu::createWithArray(taskMenuItems);
	tasksMenu->setPosition(700.0f, 400.0f);
	addChild(tasksMenu, 1);

	setTouchEnabled(true);
	setKeypadEnabled(true);

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

	if (!cell->getCurrentTask().expired())
	{
		Task* task = cell->getCurrentTask().lock().get();

		std::stringstream ss;
		ss << "Task: " << task->GetInfo()->id.c_str() << " progress: "
			<< task->CalculateProgress(World::Instance().GetWorldTime()) * 100.0f << " percent";

		_currentTaskText->setString(ss.str().c_str());
	}
	else
	{
		_currentTaskText->setString("No current tasks");
	}
}

void CellScreenLayer::_MenuInputListener(cocos2d::Ref *sender)
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

	Vector2 screen = cocos2d::Director::getInstance()->getVisibleSize();
	Vector2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();

	cocos2d::Point vertices[4];
	vertices[0] = origin;
	vertices[1] = origin + cocos2d::Point(screen.x, 0.0f);
	vertices[2] = origin + cocos2d::Point(screen.x, screen.y);
	vertices[3] = origin + cocos2d::Point(0.0f, screen.y);

	Color fill(0.3f, 0.3f, 0.3f, 0.7f);
	Color border(0.6f, 0.6f, 0.6f, 0.7f);
	
	background->drawPolygon(vertices, 4, fill, 15.0f, border);
}

void CellScreenLayer::_TaskClickListener(cocos2d::Ref *sender)
{
	cocos2d::MenuItem *item = dynamic_cast<cocos2d::MenuItem*>(sender);

	int index = item->getTag();

	TaskManager::Instance().RunTask(_cell, _availableTasks[index], World::Instance().GetWorldTime());
}

void CellScreenLayer::onKeyReleased(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event *event)
{
	if (key == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		dynamic_cast<GameScene *>(getParent())->ShowMap();
	}
}