#include "CellSpinoffMenu.h"

#include "WorldMapLayer.h"

static const unsigned int NECESSARY_MEMBERS = 0;//10;
static const unsigned int NECESSARY_CACH = 0;//10000;

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
			"cell-tasks-menu-close-pressed.png", CC_CALLBACK_1(CellSpinoffMenu::_OnCloseCallback, this));
	}

	cocos2d::Sprite *background = cocos2d::Sprite::create("cell-tasks-menu.png");
	background->setPosition(center);
	cocos2d::TTFConfig ttfConfig("arial.ttf", 18);
	cocos2d::Label *labelTitle = cocos2d::Label::createWithTTF(ttfConfig, "Cell Spinoff", cocos2d::TextHAlignment::CENTER);

	float close_x = background->getContentSize().width  / 2 - closeButton->getContentSize().width  + 5.0f;
	float close_y = background->getContentSize().height / 2 - closeButton->getContentSize().height + 5.0f;
	closeButton->setPosition(close_x, close_y);

	float title_x = center.x;
	float title_y = center.y + background->getContentSize().height / 2 - 16.0f;
	labelTitle->setPosition(title_x, title_y);
	labelTitle->setColor(cocos2d::Color3B(255, 255, 255));

	Cell::Info info = _cell.lock()->GetInfo();

	_isCellCreationPossible = info.membersCount >= NECESSARY_MEMBERS && info.cash >= NECESSARY_CACH;

	std::string strMembers = cocos2d::StringUtils::format("Members %d/%d", info.membersCount, NECESSARY_MEMBERS);
	std::string strCach = cocos2d::StringUtils::format("Cach %.1f/%d $", info.cash, NECESSARY_CACH);
	
	cocos2d::TTFConfig ttfConfigBig("arial.ttf", 24);
	_necessaryMembers = cocos2d::Label::createWithTTF(ttfConfigBig, strMembers, cocos2d::TextHAlignment::CENTER);
	_necessaryCash = cocos2d::Label::createWithTTF(ttfConfigBig, strCach, cocos2d::TextHAlignment::CENTER);

	float x1 = center.x - background->getContentSize().width / 2.0f + _necessaryMembers->getContentSize().width / 2.0f + 10.f;
	float x2 = center.x + background->getContentSize().width / 2.0f - _necessaryCash->getContentSize().width / 2.0f - 5.0f;
	_necessaryMembers->setPosition(x1, center.y + 50.0f);
	_necessaryCash->setPosition(x2, center.y + 50.0f);

	{
		using namespace cocos2d;
		_createCellButton = MenuItemImage::create("new-cell-button-active.png", "new-cell-button-pressed.png",
			"new-cell-button-disabled.png", CC_CALLBACK_1(CellSpinoffMenu::_OnCreateNewCell, this));
		_createCellButton->setPosition(0.0f, -50.0f);
		_createCellButton->setScale(0.8f);
		_createCellButton->setEnabled(_isCellCreationPossible);
	}

	cocos2d::Menu *menu = cocos2d::Menu::create(closeButton, _createCellButton, nullptr);
	menu->setPosition(center);

	cocos2d::Label *newCellButtonLabel = cocos2d::Label::createWithTTF(ttfConfigBig, "Create spinoff", cocos2d::TextHAlignment::CENTER);
	newCellButtonLabel->setPosition(_createCellButton->getContentSize() / 2.0f);
	newCellButtonLabel->setScale(1.2f);
	_createCellButton->addChild(newCellButtonLabel, 0);

	if (!_isCellCreationPossible)
	{
		newCellButtonLabel->setTextColor(cocos2d::Color4B(125, 125, 125, 255));
	}

	if (info.membersCount >= NECESSARY_MEMBERS)
	{
		_necessaryMembers->setTextColor(cocos2d::Color4B(0, 255, 0, 255));
	}
	else
	{
		_necessaryMembers->setTextColor(cocos2d::Color4B(255, 0, 0, 255));
	}

	if (info.cash >= NECESSARY_CACH)
	{
		_necessaryCash->setTextColor(cocos2d::Color4B(0, 255, 0, 255));
	}
	else
	{
		_necessaryCash->setTextColor(cocos2d::Color4B(255, 0, 0, 255));
	}

	cocos2d::ScaleTo *scale = cocos2d::ScaleTo::create(0.8f, 1.0f, 1.0f);
	cocos2d::FadeIn *fade = cocos2d::FadeIn::create(0.5f);
	cocos2d::EaseElasticOut *elastic_scale = cocos2d::EaseElasticOut::create(scale, 5.0f);

	setScale(0.01f);
	setOpacity(0);	
	runAction(elastic_scale);
	runAction(fade);
	addChild(background, 0);
	addChild(menu, 1);
	addChild(labelTitle, 1);
	addChild(_necessaryMembers, 1);
	addChild(_necessaryCash, 1);

	setKeyboardEnabled(true);

	return true;
}

void CellSpinoffMenu::UpdateSpinoffState(const Cell::Info &info)
{
}

void CellSpinoffMenu::_OnCloseCallback(cocos2d::Ref *sender)
{
	_SelfClose();
}

void CellSpinoffMenu::_OnCreateNewCell(cocos2d::Ref *sender)
{
	_worldMapLayer->SetNextCellParent(_cell);
	_SelfClose();
}

void CellSpinoffMenu::_SelfClose(void)
{
	cocos2d::ScaleTo *scale = cocos2d::ScaleTo::create(0.2f, 0.2f, 0.01f);
	cocos2d::EaseElasticIn *elastic_scale = cocos2d::EaseElasticIn::create(scale, 5.0f);
	cocos2d::CallFunc *func = cocos2d::CallFunc::create(CC_CALLBACK_0(CellMenuSelector::OnCellMenuClosed, _cellMenuSelector));

	runAction(cocos2d::Sequence::create(elastic_scale, func, nullptr));
}

void CellSpinoffMenu::onKeyReleased(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event *event)
{
	if (key == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		_SelfClose();
	}
}
