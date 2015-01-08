#include "CellTasksMenu.h"

#include "World.h"
#include "Localization.h"
#include "Log.h"

CellTasksScreen::CellTasksScreen(Cell::WeakPtr cell, CellMenuSelector *cellMenu)
	: _cell(cell)
	, _cellMenu(cellMenu)
{
	init();
}

CellTasksScreen* CellTasksScreen::create(Cell::WeakPtr cell, CellMenuSelector *cellMenu)
{
	CellTasksScreen* ret = new CellTasksScreen(cell, cellMenu);
	if (ret && ret->init())
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

bool CellTasksScreen::init(void)
{
	if (!cocos2d::Layer::init())
	{
		return false;
	}

	cocos2d::EventListenerKeyboard *keyboard = cocos2d::EventListenerKeyboard::create();
	keyboard->onKeyReleased = CC_CALLBACK_2(CellTasksScreen::KeyReleased, this);
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboard, this);
	
	cocos2d::Point screen = cocos2d::Director::getInstance()->getVisibleSize();
	cocos2d::Point origin = cocos2d::Director::getInstance()->getVisibleOrigin();

	cocos2d::ScaleTo *scale = cocos2d::ScaleTo::create(0.8f, 1.0f, 1.0f);
	cocos2d::FadeIn *fade = cocos2d::FadeIn::create(0.5f);
	cocos2d::EaseElasticOut *elastic_scale = cocos2d::EaseElasticOut::create(scale, 5.0f);

	_widget = dynamic_cast<cocos2d::ui::Layout *>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("ui_cell_ingame/ui_cell_select_task.ExportJson"));
	_widget->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
	_widget->setPosition(origin + screen / 2.0f);
	_widget->setScale(0.01f);
	_widget->setOpacity(0);
	_widget->runAction(cocos2d::Spawn::create(elastic_scale, fade, nullptr));
	
	cocos2d::ui::Text *txtHeader = dynamic_cast<cocos2d::ui::Text *>(_widget->getChildByName("Header"));
	cocos2d::ui::Button *btnClose = dynamic_cast<cocos2d::ui::Button *>(_widget->getChildByName("Close"));
	cocos2d::ui::ScrollView *scroller = dynamic_cast<cocos2d::ui::ScrollView *>(_widget->getChildByName("Scroller"));
	
	if (!txtHeader) {WRITE_WARN("Failed to get widget with Header name from UICellTaskSelect widget"); return false;}
	if (!btnClose) {WRITE_WARN("Failed to get widget with Close name from UICellTaskSelect widget"); return false;}
	if (!scroller) {WRITE_WARN("Failed to get widget with Scroller name from UICellTaskSelect widget"); return false;}

	txtHeader->setString(LocalizationManager::Instance().getText("CellTaskMenu_Header"));
	btnClose->addTouchEventListener(CC_CALLBACK_2(CellTasksScreen::OnCloseCallback, this));
	scroller->setInertiaScrollEnabled(true);
	scroller->setBounceEnabled(true);
	
	_avaliableTasks = World::Instance().GetTaskManager().GetAvailableTasks(_cell);
	float item_h = 0.0f, total_h = 0.0f;
	
	for (std::size_t index = 0; index < _avaliableTasks.size(); ++index)
	{
		cocos2d::ui::Widget *item = CreateScrollerItem(_avaliableTasks[index]);
		item_h = item->getContentSize().height;
		total_h = item_h * index;

		item->setPosition(cocos2d::Vec2(0.0f, total_h));
		scroller->addChild(item);
	}

	cocos2d::Size size = scroller->getInnerContainerSize();
	scroller->setInnerContainerSize(cocos2d::Size(size.width, item_h * _avaliableTasks.size()));

	addChild(_widget);

	if (World::Instance().GetTutorialManager().IsTutorialStateAvailable("StartFirstTask"))
	{
		World::Instance().GetTutorialManager().RemoveCurrentTutorial();
		World::Instance().GetTutorialManager().RunTutorialFunction("BeforeStartFirstTask");
	}

	return true;
}

void CellTasksScreen::CloseMenu(void)
{
	// we don't want to close menu then tutorial in the screen.
	if (World::Instance().GetTutorialManager().IsTutorialStateAvailable("WaitingForStartFirstTask")) {
		return;
	}

	cocos2d::ScaleTo *scale = cocos2d::ScaleTo::create(0.2f, 0.2f, 0.01f);
	cocos2d::EaseElasticIn *elastic_scale = cocos2d::EaseElasticIn::create(scale, 5.0f);
	cocos2d::CallFunc *func = cocos2d::CallFunc::create(CC_CALLBACK_0(CellMenuSelector::OnMenuClosed, _cellMenu));

	runAction(cocos2d::Sequence::create(elastic_scale, func, nullptr));
}

cocos2d::ui::Widget* CellTasksScreen::CreateScrollerItem(const Task::Info *info)
{
	cocos2d::ui::Widget *widget = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("ui_cell_ingame/ui_cell_task_item.ExportJson");

	cocos2d::ui::TextBMFont *taskTitle = dynamic_cast<cocos2d::ui::TextBMFont *>(widget->getChildByName("TaskTitle"));
	cocos2d::ui::TextBMFont *taskDuration = dynamic_cast<cocos2d::ui::TextBMFont *>(widget->getChildByName("Duration"));
	cocos2d::ui::TextBMFont *taskChanse = dynamic_cast<cocos2d::ui::TextBMFont *>(widget->getChildByName("Chanse"));
	cocos2d::ui::Button *startBtn = dynamic_cast<cocos2d::ui::Button *>(widget->getChildByName("StartBtn"));

	if (!taskTitle) {WRITE_WARN("Failed to get widget with TaskTitle name from ScrollTaskItem widget"); return widget;}
	if (!taskDuration) {WRITE_WARN("Failed to get widget with Duration name from ScrollTaskItem widget"); return widget;}
	if (!taskChanse) {WRITE_WARN("Failed to get widget with Chanse name from ScrollTaskItem widget"); return widget;}
	if (!startBtn) {WRITE_WARN("Failed to get widget with StartBtn name from ScrollTaskItem widget"); return widget;}

	startBtn->setUserData((void *)info);
	startBtn->addTouchEventListener(CC_CALLBACK_2(CellTasksScreen::OnStartTaskCallback, this));
	startBtn->setTitleText(LocalizationManager::Instance().getText("CellTaskMenuItem_Start"));

	taskTitle->setString(LocalizationManager::Instance().getText(std::string("Task_").append(info->id)));
	taskDuration->setString(cocos2d::StringUtils::format("%s %.1f", LocalizationManager::Instance().getText("CellTaskMenuItem_Duration").c_str(), info->duration));
	taskChanse->setString(cocos2d::StringUtils::format("%s %.1f",
		LocalizationManager::Instance().getText("CellTaskMenuItem_Chanse").c_str(),
		World::Instance().GetTaskManager().CalcTaskSuccessChance(_cell.lock()->GetInfo(), info)));

	return widget;
}

void CellTasksScreen::OnCloseCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType eventType)
{
	if (eventType == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		CloseMenu();
	}
}

void CellTasksScreen::OnStartTaskCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType eventType)
{
	if (eventType == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		if (_cell.lock()->GetInfo().state == Cell::State::READY)
		{
			if (World::Instance().GetTutorialManager().IsTutorialStateAvailable("WaitingForStartFirstTask"))
			{
				World::Instance().GetTutorialManager().RemoveCurrentTutorial();
				World::Instance().GetTutorialManager().RunTutorialFunction("StartingFirstTask");
			}
			else if (World::Instance().GetTutorialManager().IsTutorialStateAvailable("ReadyToFirstRealWork"))
			{
				World::Instance().GetTutorialManager().RemoveCurrentTutorial();
				World::Instance().GetTutorialManager().RunTutorialFunction("StartingFirstRealWork");
			}

			cocos2d::ui::Button *btn = dynamic_cast<cocos2d::ui::Button *>(sender);
			if (!btn) {
				WRITE_WARN("Failed to cast from sender to button type in StartTaskCallback.");
				return;
			}
			const Task::Info *task_info = (const Task::Info *)btn->getUserData();
			if (!task_info) {
				WRITE_WARN("Failed to get pointer to a task info from sender object in StartTaskCallback");
				return;
			}

			World::Instance().GetTaskManager().RunTask(_cell, task_info, Utils::GetGameTime());
			CloseMenu();
			_cellMenu->DisappearWithAnimation();
		}
	}
}

void CellTasksScreen::KeyReleased(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event *event)
{
	if (key == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		CloseMenu();
	}
}
