#include "CellSpinoffMenu.h"

#include "WorldMapLayer.h"
#include "GameInfo.h"
#include "Log.h"

CellSpinoffMenu::CellSpinoffMenu(Cell::WeakPtr cell, CellMenuSelector *selector, WorldMapLayer *worldMap)
	: _cell(cell)
	, _cellMenuSelector(selector)
	, _worldMapLayer(worldMap)
	, _isCellCreationPossible(false)
{
	init();
}

bool CellSpinoffMenu::init(void)
{
	if (!cocos2d::Layer::init()) {
		return false;
	}

	cocos2d::EventListenerKeyboard *keyboard = cocos2d::EventListenerKeyboard::create();
	keyboard->onKeyReleased = CC_CALLBACK_2(CellSpinoffMenu::KeyReleased, this);
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboard, this);
	
	cocos2d::Point screen = cocos2d::Director::getInstance()->getVisibleSize();
	cocos2d::Point origin = cocos2d::Director::getInstance()->getVisibleOrigin();

	cocos2d::ScaleTo *scale = cocos2d::ScaleTo::create(0.8f, 1.0f, 1.0f);
	cocos2d::FadeIn *fade = cocos2d::FadeIn::create(0.5f);
	cocos2d::EaseElasticOut *elastic_scale = cocos2d::EaseElasticOut::create(scale, 5.0f);
	cocos2d::Spawn *motion = cocos2d::Spawn::create(elastic_scale, fade, nullptr);

	_widget = dynamic_cast<cocos2d::ui::Layout *>
		(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("ui_cell_ingame/ui_cell_spinoff.ExportJson"));
	_widget->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
	_widget->setPosition(origin + screen / 2.0f);
	_widget->setScale(0.01f);
	_widget->setOpacity(0);
	_widget->runAction(motion);

	_cachTextWidget = dynamic_cast<cocos2d::ui::Text *>(_widget->getChildByName("TextCach"));
	_membersTextWidget = dynamic_cast<cocos2d::ui::Text *>(_widget->getChildByName("TextMembers"));
	cocos2d::ui::Button *btnClose = dynamic_cast<cocos2d::ui::Button *>(_widget->getChildByName("BtnClose"));
	cocos2d::ui::Button *btnSpinoff = dynamic_cast<cocos2d::ui::Button *>(_widget->getChildByName("BtnSpinoff"));
	
	if (!_cachTextWidget) {
		Log::Instance().writeWarning("UICellSpinoff: Failed to get widget with name TextCach");
		return false;
	}
	if (!_membersTextWidget) {
		Log::Instance().writeWarning("UICellSpinoff: Failed to get widget with name TextMembers");
		return false;
	}

	btnClose->addTouchEventListener(CC_CALLBACK_2(CellSpinoffMenu::OnCloseCallback, this));
	btnSpinoff->addTouchEventListener(CC_CALLBACK_2(CellSpinoffMenu::OnSpinoffCallback, this));

	Cell::Info info = _cell.lock()->GetInfo();
	bool isMembersEnough = info.membersCount >= GameInfo::Instance().GetInt("CELL_SPINOFF_MEMBERS_PRICE") * 2;
	bool isCashEnough = info.cash >= GameInfo::Instance().GetInt("CELL_SPINOFF_CASH_PRICE");
	_isCellCreationPossible = isMembersEnough && isCashEnough;
	std::string strMembers = cocos2d::StringUtils::format("Members %d / %d", info.membersCount, GameInfo::Instance().GetInt("CELL_SPINOFF_MEMBERS_PRICE") * 2);
	std::string strCach = cocos2d::StringUtils::format("Cash %d / %d $", info.cash, GameInfo::Instance().GetInt("CELL_SPINOFF_CASH_PRICE"));
	
	_cachTextWidget->setString(strCach);
	_membersTextWidget->setString(strMembers);

	if (!_isCellCreationPossible) {
		btnSpinoff->setBright(false);
		btnSpinoff->setEnabled(false);
	}

	if (isMembersEnough) {
		_membersTextWidget->setColor(cocos2d::Color3B(0, 255, 0));
	} else {
		_membersTextWidget->setColor(cocos2d::Color3B(255, 0, 0));
	}

	if (isCashEnough) {
		_cachTextWidget->setColor(cocos2d::Color3B(0, 255, 0));
	} else {
		_cachTextWidget->setColor(cocos2d::Color3B(255, 0, 0));
	}

	addChild(_widget);

	return true;
}

void CellSpinoffMenu::UpdateSpinoffState(const Cell::Info &info)
{
}

void CellSpinoffMenu::OnCloseCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType eventType)
{
	if (eventType == cocos2d::ui::Widget::TouchEventType::ENDED) {
		SelfClose();
	}
}

void CellSpinoffMenu::OnSpinoffCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType eventType)
{
	if (eventType == cocos2d::ui::Widget::TouchEventType::ENDED) {
		if (_cell.lock()->GetInfo().state == Cell::State::READY)
		{
			_worldMapLayer->SetNextCellParent(_cell);
			SelfClose();

			if (World::Instance().IsTutorialStateAvailable("ReadyToCreateSpinoff"))
			{
				World::Instance().RunTutorialFunction("OnReadyToCreateFirstSpinoff");
			}
		}
	}
}

void CellSpinoffMenu::SelfClose(void)
{
	cocos2d::ScaleTo *scale = cocos2d::ScaleTo::create(0.2f, 0.2f, 0.01f);
	cocos2d::EaseElasticIn *elastic_scale = cocos2d::EaseElasticIn::create(scale, 5.0f);
	cocos2d::CallFunc *func = cocos2d::CallFunc::create(CC_CALLBACK_0(CellMenuSelector::OnMenuClosed, _cellMenuSelector));

	runAction(cocos2d::Sequence::create(elastic_scale, func, nullptr));
}

void CellSpinoffMenu::KeyReleased(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event *event)
{
	if (key == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		SelfClose();
	}
}
