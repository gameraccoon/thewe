#include "CellTaskInfoMenu.h"

#include "Log.h"

CellTaskInfoMenu::CellTaskInfoMenu(Cell::WeakPtr cell, CellMenuSelector *selector)
	: _cell(cell)
	, _cellMenuSelector(selector)
{
}

CellTaskInfoMenu* CellTaskInfoMenu::create(Cell::WeakPtr cell, CellMenuSelector *cellMenu)
{
	CellTaskInfoMenu* ret = new CellTaskInfoMenu(cell, cellMenu);
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

bool CellTaskInfoMenu::init(void)
{
	if (!cocos2d::Layer::init())
	{
		return false;
	}

	_cellCurrentTask = _cell.lock()->getCurrentTask();
	
	cocos2d::EventListenerKeyboard *keyboard = cocos2d::EventListenerKeyboard::create();
	keyboard->onKeyReleased = CC_CALLBACK_2(CellTaskInfoMenu::KeyReleased, this);
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboard, this);

	cocos2d::Point screen = cocos2d::Director::getInstance()->getVisibleSize();
	cocos2d::Point origin = cocos2d::Director::getInstance()->getVisibleOrigin();

	cocos2d::ScaleTo *scale = cocos2d::ScaleTo::create(0.8f, 1.0f, 1.0f);
	cocos2d::FadeIn *fade = cocos2d::FadeIn::create(0.5f);
	cocos2d::EaseElasticOut *elastic_scale = cocos2d::EaseElasticOut::create(scale, 5.0f);

	_widget = dynamic_cast<cocos2d::ui::Layout *>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("ui_cell_ingame/ui_cell_current_task.ExportJson"));
	_widget->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
	_widget->setPosition(origin + screen / 2.0f);
	_widget->setScale(0.001f);
	_widget->setOpacity(0);
	_widget->runAction(cocos2d::Spawn::create(elastic_scale, fade, nullptr));

	cocos2d::ui::Widget *window = dynamic_cast<cocos2d::ui::Widget *>(_widget->getChildByName("Window"));

	cocos2d::ui::Button *btnClose = dynamic_cast<cocos2d::ui::Button *>(window->getChildByName("Close"));
	cocos2d::ui::Button *btnAbort = dynamic_cast<cocos2d::ui::Button *>(window->getChildByName("Abort"));
	_progressBar = dynamic_cast<cocos2d::ui::LoadingBar *>(window->getChildByName("TaskProgress")->getChildByName("Progress"));
	_textPercentage = dynamic_cast<cocos2d::ui::TextBMFont *>(window->getChildByName("TaskProgress")->getChildByName("Persentage"));

	if (!btnClose) {WRITE_ERR("Failed to get element with name Close from ui_cell_current_task widget"); return false;}
	if (!btnAbort) {WRITE_ERR("Failed to get element with name Abort from ui_cell_current_task widget"); return false;}
	if (!_progressBar) {WRITE_ERR("Failed to get element with name TaskProgress:Progress from ui_cell_current_task widget"); return false;}
	if (!_textPercentage) {WRITE_ERR("Failed to get element with name TaskProgress:Persentage from ui_cell_current_task widget"); return false;}

	btnClose->addTouchEventListener(CC_CALLBACK_2(CellTaskInfoMenu::OnCloseCallback, this));
	btnAbort->addTouchEventListener(CC_CALLBACK_2(CellTaskInfoMenu::OnAbortCallback, this));

	addChild(_widget);
	scheduleUpdate();

	return true;
}

void CellTaskInfoMenu::update(float dt)
{
	Task::Ptr currentTask = _cellCurrentTask.lock();
	if (currentTask)
	{
		Utils::GameTime time = Utils::GetGameTime();
		float progress = currentTask->CalculateProgress(time) * 100.0f;
		_progressBar->setPercent(progress);
		_textPercentage->setString(cocos2d::StringUtils::format("%d%%", (int)progress));
	}
	else
	{
		CloseMenu();
	}
}

void CellTaskInfoMenu::CloseMenu()
{
	cocos2d::ScaleTo *scale = cocos2d::ScaleTo::create(0.2f, 0.2f, 0.01f);
	cocos2d::EaseElasticIn *elastic_scale = cocos2d::EaseElasticIn::create(scale, 5.0f);
	cocos2d::CallFunc *func = cocos2d::CallFunc::create(CC_CALLBACK_0(CellMenuSelector::OnMenuClosed, _cellMenuSelector));

	runAction(cocos2d::Sequence::create(elastic_scale, func, nullptr));
}

void CellTaskInfoMenu::KeyReleased(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event *event)
{
	if (key == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		CloseMenu();
	}
}

void CellTaskInfoMenu::OnCloseCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType eventType)
{
	if (eventType == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		CloseMenu();
	}
}

void CellTaskInfoMenu::OnAbortCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType eventType)
{
	if (eventType == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		Task::Ptr currentTask = _cellCurrentTask.lock();
		if (currentTask)
		{
			currentTask->Abort();
		}
	}
}
