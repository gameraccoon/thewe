#include "CellInfoMenu.h"

#include "Log.h"
#include "Localization.h"

CellInfoMenu::CellInfoMenu(Cell::WeakPtr cell, CellMenuSelector *selector)
	: _cell(cell)
	, _cellMenuSelector(selector)
{
	init();
}

bool CellInfoMenu::init(void)
{
	if (!cocos2d::Layer::init())
	{
		return false;
	}

	cocos2d::EventListenerKeyboard *keyboard = cocos2d::EventListenerKeyboard::create();
	keyboard->onKeyReleased = CC_CALLBACK_2(CellInfoMenu::KeyReleased, this);
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboard, this);
	
	cocos2d::Point screen = cocos2d::Director::getInstance()->getVisibleSize();
	cocos2d::Point origin = cocos2d::Director::getInstance()->getVisibleOrigin();
	cocos2d::Point center = origin + screen / 2.0f;

	cocos2d::ScaleTo *scale = cocos2d::ScaleTo::create(0.8f, 1.0f, 1.0f);
	cocos2d::FadeIn *fade = cocos2d::FadeIn::create(0.5f);
	cocos2d::EaseElasticOut *elastic_scale = cocos2d::EaseElasticOut::create(scale, 5.0f);

	_widget = dynamic_cast<cocos2d::ui::Layout *>(cocostudio::GUIReader::getInstance()->
		widgetFromJsonFile("ui_cell_ingame/ui_cell_info.ExportJson"));
	_widget->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
	_widget->setPosition(origin + screen / 2.0f);
	_widget->setScale(0.01f);
	_widget->setOpacity(0);
	_widget->runAction(cocos2d::Spawn::create(elastic_scale, fade, nullptr));
	
	cocos2d::ui::Widget *window = dynamic_cast<cocos2d::ui::Widget *>(_widget->getChildByName("Window"));

	cocos2d::ui::Button *btnClose = dynamic_cast<cocos2d::ui::Button *>(window->getChildByName("Close"));
	cocos2d::ui::Text *txtHeader = dynamic_cast<cocos2d::ui::Text *>(window->getChildByName("Header"));
	_txtMembers = dynamic_cast<cocos2d::ui::TextBMFont *>(window->getChildByName("Members"));
	_txtCach = dynamic_cast<cocos2d::ui::TextBMFont *>(window->getChildByName("Cach"));
	_txtPursuedLevel = dynamic_cast<cocos2d::ui::TextBMFont *>(window->getChildByName("PursuedLevel"));
	_txtMorale = dynamic_cast<cocos2d::ui::TextBMFont *>(window->getChildByName("MoraleLabel"));
	_txtDevotion = dynamic_cast<cocos2d::ui::TextBMFont *>(window->getChildByName("DevotionLabel"));
	_txtLevel = dynamic_cast<cocos2d::ui::TextBMFont *>(window->getChildByName("LevelProgress")->getChildByName("Level"));
	_levelProgressBar = dynamic_cast<cocos2d::ui::LoadingBar *>(window->getChildByName("LevelProgress")->getChildByName("Progress"));
	_moraleProgressBar = dynamic_cast<cocos2d::ui::LoadingBar *>(window->getChildByName("MoraleLevel")->getChildByName("Progress"));
	_devotionProgressBar = dynamic_cast<cocos2d::ui::LoadingBar *>(window->getChildByName("DevotionLevel")->getChildByName("Progress"));
	
	if (!btnClose) {WRITE_WARN("CellInfo: Failed to get widget with name Close"); return false;}
	if (!txtHeader) {WRITE_WARN("CellInfo: Failed to get widget with name Header"); return false;}
	if (!_txtMembers) {WRITE_WARN("CellInfo: Failed to get widget with name Members"); return false;}
	if (!_txtCach) {WRITE_WARN("CellInfo: Failed to get widget with name Cach"); return false;}
	if (!_txtPursuedLevel) {WRITE_WARN("CellInfo: Failed to get widget with name PursuedLevel"); return false;}
	if (!_txtMorale) {WRITE_WARN("CellInfo: Failed to get widget with name MoraleLabel"); return false;}
	if (!_txtDevotion) {WRITE_WARN("CellInfo: Failed to get widget with name DevotionLabel"); return false;}
	if (!_txtLevel) {WRITE_WARN("CellInfo: Failed to get widget with name LevelProgress:Label"); return false;}
	if (!_levelProgressBar) {WRITE_WARN("CellInfo: Failed to get widget with name MoraleLevel:Progress"); return false;}
	if (!_moraleProgressBar) {WRITE_WARN("CellInfo: Failed to get widget with name MoraleLevel:Progress"); return false;}
	if (!_devotionProgressBar) {WRITE_WARN("CellInfo: Failed to get widget with name DevotionLevel:Progress"); return false;}

	btnClose->addTouchEventListener(CC_CALLBACK_2(CellInfoMenu::OnCloseCallback, this));
	txtHeader->setString(LocalizationManager::Instance().getText("CellInfoMenu_Header"));

	UpdateInfoBy(_cell.lock());

	addChild(_widget);

	return true;
}

void CellInfoMenu::update(float dt)
{
	UpdateInfoBy(_cell.lock());
}

void CellInfoMenu::UpdateInfoBy(Cell::Ptr cell)
{
	Cell::Info info = cell->GetInfo();

	int level = World::Instance().GetLevelFromExperience(info.experience);
	int currentLevelExp = World::Instance().GetExperienceForLevel(level);
	int expBetweenLevels = World::Instance().GetExperienceForLevel(level + 1) - currentLevelExp;
	int earnedLevelExp = info.experience - currentLevelExp;
	float levelProgress = 1.0f * earnedLevelExp / expBetweenLevels;
	_levelProgressBar->setPercent(levelProgress * 100.0f);
	_moraleProgressBar->setPercent(info.morale * 100.0f);
	_devotionProgressBar->setPercent(info.devotion * 100.0f);

	_txtMembers->setString(cocos2d::StringUtils::format("%s %d",
		LocalizationManager::Instance().getText("CellInfoMenu_Members").c_str(), info.membersCount));
	_txtCach->setString(cocos2d::StringUtils::format("%s %d",
		LocalizationManager::Instance().getText("CellInfoMenu_Cach").c_str(), info.cash));
	_txtLevel->setString(cocos2d::StringUtils::format("%s %d",
		LocalizationManager::Instance().getText("CellInfoMenu_Level").c_str(), level));
	_txtMorale->setString(LocalizationManager::Instance().getText("CellInfoMenu_Morale"));
	_txtDevotion->setString(LocalizationManager::Instance().getText("CellInfoMenu_Devotion"));

	float pursuedLevel = World::Instance().GetCellPursuedLevel(cell.get());
	if (pursuedLevel < 33.0f)
	{
		_txtPursuedLevel->setString(LocalizationManager::Instance().getText("CellInfoMenu_PursuedLow"));
	}
	else if (pursuedLevel < 66.0f)
	{
		_txtPursuedLevel->setString(LocalizationManager::Instance().getText("CellInfoMenu_PursuedMid"));
	}
	else
	{
		_txtPursuedLevel->setString(LocalizationManager::Instance().getText("CellInfoMenu_PursuedHigh"));
	}
}

void CellInfoMenu::OnCloseCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType eventType)
{
	if (eventType == cocos2d::ui::Widget::TouchEventType::ENDED) {
		CloseMenu();
	}
}

void CellInfoMenu::CloseMenu()
{
	cocos2d::ScaleTo *scale = cocos2d::ScaleTo::create(0.2f, 0.2f, 0.01f);
	cocos2d::EaseElasticIn *elastic_scale = cocos2d::EaseElasticIn::create(scale, 5.0f);
	cocos2d::CallFunc *func = cocos2d::CallFunc::create(CC_CALLBACK_0(CellMenuSelector::OnMenuClosed, _cellMenuSelector));

	runAction(cocos2d::Sequence::create(elastic_scale, func, nullptr));
}

void CellInfoMenu::KeyReleased(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event *event)
{
	if (key == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		CloseMenu();
	}
}
