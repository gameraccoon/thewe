#include "CellTasksMenu.h"

#include "World.h"

class CellTaskMenuItem : public cocos2d::Node
{
public:
	CellTaskMenuItem(CellTasksScreen *menu, Cell::WeakPtr cell, const Task::Info *taskInfo)
		: _tasksScreen(menu)
		, _cell(cell)
		, _taskInfo(taskInfo)
	{
		init();
	}

	virtual bool init(void) override
	{
		cocos2d::MenuItemImage *startBtn;
		{
			using namespace cocos2d;
			startBtn = MenuItemImage::create("TasksStartBtn_active.png", "TasksStartBtn_pressed.png",
				CC_CALLBACK_1(CellTaskMenuItem::_OnStartPressed, this));
		}

		cocos2d::Sprite *bkg = cocos2d::Sprite::create("TasksLine.png");
		cocos2d::Menu *menu = cocos2d::Menu::create(startBtn, nullptr);
		cocos2d::Size size = bkg->getContentSize();

		menu->setPosition(0.0f, 0.0f);
		startBtn->setPosition(-size.width / 2.0 + startBtn->getContentSize().width / 2.0f, 0.0f);

		cocos2d::TTFConfig ttfConfig("arial.ttf");
		cocos2d::Label *labelStart = cocos2d::Label::createWithTTF(ttfConfig, "Start", cocos2d::TextHAlignment::CENTER);
		labelStart->setColor(cocos2d::Color3B(0, 0, 0));	
		labelStart->setPosition(startBtn->getPosition());

		cocos2d::Label *labelDesc = cocos2d::Label::createWithBMFont("futura-48.fnt", _taskInfo->title, cocos2d::TextHAlignment::LEFT);
		labelDesc->setPosition(-60.0f, 0.0f);
		labelDesc->setScale(0.35f);

		std::string stringMorale = cocos2d::StringUtils::format("Morale: %.1f", _taskInfo->moralLevel);
		cocos2d::Label *labelMorale = cocos2d::Label::createWithBMFont("futura-48.fnt", stringMorale, cocos2d::TextHAlignment::CENTER);
		labelMorale->setPosition(45.0f, 0.0f);
		labelMorale->setScale(0.35f);

		std::string stringDifficult = cocos2d::StringUtils::format("Severity: %.1f", _taskInfo->severity);
		cocos2d::Label *labelSeverity = cocos2d::Label::createWithBMFont("futura-48.fnt", stringDifficult, cocos2d::TextHAlignment::RIGHT);
		labelSeverity->setPosition(150.0f, 0.0f);
		labelSeverity->setScale(0.35f);

		setContentSize(size);
		addChild(bkg, 0);
		addChild(menu, 1);
		addChild(labelStart, 2);
		addChild(labelDesc, 1);
		addChild(labelMorale, 1);
		addChild(labelSeverity, 1);

		if (World::Instance().GetTutorialState() == "StartFirstTask")
		{
			World::Instance().RunTutorialState("AfterStartFirstTask");
		}

		return true;
	}

private:
	void _OnStartPressed(cocos2d::Ref *sender)
	{
		if (_cell.lock()->GetInfo().state == Cell::READY)
		{
			World::Instance().GetTaskManager().RunTask(_cell, _taskInfo, Utils::GetGameTime());
			_tasksScreen->CloseMenu();
		}
	}

	CellTasksScreen *_tasksScreen;
	Cell::WeakPtr _cell;
	const Task::Info *_taskInfo;
};

CellTasksScreen::CellTasksScreen(Cell::WeakPtr cell, CellMenuSelector *cellMenu)
	: _cell(cell)
	, _cellMenu(cellMenu)
	, _scrollTasksView(nullptr)
{
	init();
}

bool CellTasksScreen::init(void)
{
	if (!cocos2d::Layer::init())
	{
		return false;
	}
	
	cocos2d::Point screen = cocos2d::Director::getInstance()->getVisibleSize();
	cocos2d::Point origin = cocos2d::Director::getInstance()->getVisibleOrigin();
	cocos2d::Point center = origin + screen / 2.0f;

	cocos2d::MenuItemImage *closeButton;
	{
		using namespace cocos2d;
		closeButton = MenuItemImage::create("cell-tasks-menu-close-normal.png",
			"cell-tasks-menu-close-pressed.png", CC_CALLBACK_1(CellTasksScreen::_OnCloseCallback, this));
	}

	cocos2d::Menu *menu = cocos2d::Menu::create(closeButton, nullptr);
	menu->setPosition(center);
	cocos2d::Sprite *menuBackground = cocos2d::Sprite::create("cell-tasks-menu.png");
	menuBackground->setPosition(center);
	
	cocos2d::TTFConfig ttfConfig("arial.ttf", 18);
	cocos2d::Label *labelTitle = cocos2d::Label::createWithTTF(ttfConfig, "Cell Avaliable Tasks", cocos2d::TextHAlignment::CENTER);
	
	float close_x = menuBackground->getContentSize().width  / 2 - closeButton->getContentSize().width  + 5.0f;
	float close_y = menuBackground->getContentSize().height / 2 - closeButton->getContentSize().height + 5.0f;
	closeButton->setPosition(close_x, close_y);

	float title_x = center.x;
	float title_y = center.y + menuBackground->getContentSize().height / 2 - 16.0f;
	labelTitle->setPosition(title_x, title_y);
	labelTitle->setColor(cocos2d::Color3B(255, 255, 255));

	float offset_top = 47.0f;
	float offset_btm = 14.0f;
	cocos2d::Size menu_size = menuBackground->getContentSize();
	cocos2d::Vec2 menu_pos = center - menu_size / 2.0f;

	menu_pos.y += offset_btm;
	menu_size.height -= offset_top;

	_CreateTasksScrollViewMenu(World::Instance().GetTaskManager().GetAvailableTasks(_cell), menu_pos, menu_size);

	cocos2d::ScaleTo *scale = cocos2d::ScaleTo::create(0.8f, 1.0f, 1.0f);
	cocos2d::FadeIn *fade = cocos2d::FadeIn::create(0.5f);
	cocos2d::EaseElasticOut *elastic_scale = cocos2d::EaseElasticOut::create(scale, 5.0f);

	setScale(0.01f);
	setOpacity(0);	
	runAction(elastic_scale);
	runAction(fade);
	addChild(menuBackground, 0);
	addChild(menu, 1);
	addChild(labelTitle, 1);
	addChild(_scrollTasksView, 2);

	setKeypadEnabled(true);

	return true;
}

void CellTasksScreen::CloseMenu(void)
{
	cocos2d::ScaleTo *scale = cocos2d::ScaleTo::create(0.2f, 0.2f, 0.01f);
	cocos2d::EaseElasticIn *elastic_scale = cocos2d::EaseElasticIn::create(scale, 5.0f);
	cocos2d::CallFunc *func = cocos2d::CallFunc::create(CC_CALLBACK_0(CellMenuSelector::OnCellMenuClosed, _cellMenu));

	runAction(cocos2d::Sequence::create(elastic_scale, func, nullptr));
}

void CellTasksScreen::_OnCloseCallback(cocos2d::Ref *sender)
{
	CloseMenu();
}

void CellTasksScreen::_CreateTasksScrollViewMenu(const TaskManager::TasksList &tasksList, const cocos2d::Vec2 &pos, const cocos2d::Size &size)
{
	_scrollTasksView = cocos2d::extension::ScrollView::create(size);
	_scrollTasksView->setPosition(pos);
	_scrollTasksView->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);

	float itemOffsetY = 1.0f;
	float itemContentWidth = 75.0f;
	float contentSizeY = tasksList.size() * (itemContentWidth - itemOffsetY);

	if (contentSizeY * itemContentWidth > size.height)
	{
		_scrollTasksView->setContentSize(cocos2d::Size(size.width, contentSizeY - 3.0f));
	}
	else
	{
		_scrollTasksView->setContentSize(size);
	}

	float y = _scrollTasksView->getContentSize().height - itemContentWidth / 2.0f;

	int index = 0;
	for (TaskManager::TasksList::const_iterator it = tasksList.begin(); it != tasksList.end(); it++, ++index)
	{
		const Task::Info *info = (*it);
		CellTaskMenuItem *item = new CellTaskMenuItem(this, _cell, info);

		item->setPosition(item->getContentSize().width / 2.0f, y);
		item->autorelease();

		y -= item->getContentSize().height - itemOffsetY;

		_scrollTasksView->addChild(item);
	}

	_scrollTasksView->setContentOffset(_scrollTasksView->minContainerOffset());
}

void CellTasksScreen::onKeyReleased(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event *event)
{
	if (key == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		CloseMenu();
	}
}
