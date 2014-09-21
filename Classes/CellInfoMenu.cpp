#include "CellInfoMenu.h"

CellInfoMenu::CellInfoMenu(Cell::WeakPtr cell, CellMenuSelector *selector)
	: _cell(cell)
	, _cellMenuSelector(selector)
	, _taskProgressBar(nullptr)
{
	init();
}

bool CellInfoMenu::init(void)
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
			"cell-tasks-menu-close-pressed.png", CC_CALLBACK_1(CellInfoMenu::_OnCloseCallback, this));
	}

	cocos2d::Menu *menu = cocos2d::Menu::create(closeButton, nullptr);
	menu->setPosition(center);
	cocos2d::Sprite *background = cocos2d::Sprite::create("cell-tasks-menu.png");
	background->setPosition(center);
	cocos2d::TTFConfig ttfConfig("arial.ttf", 18);
	cocos2d::Label *labelTitle = cocos2d::Label::createWithTTF(ttfConfig, "Cell Info", cocos2d::TextHAlignment::CENTER);

	float close_x = background->getContentSize().width  / 2 - closeButton->getContentSize().width  + 5.0f;
	float close_y = background->getContentSize().height / 2 - closeButton->getContentSize().height + 5.0f;
	closeButton->setPosition(close_x, close_y);

	float title_x = center.x;
	float title_y = center.y + background->getContentSize().height / 2 - 16.0f;
	labelTitle->setPosition(title_x, title_y);
	labelTitle->setColor(cocos2d::Color3B(255, 255, 255));
	
	float info_x = center.x - background->getContentSize().width  / 2.0f + 10.0f;
	float info_y = center.y + background->getContentSize().height / 2.0f - 60.0f;

	_labelCachInfo = cocos2d::Label::createWithBMFont("futura-48.fnt", "", cocos2d::TextHAlignment::LEFT);
	_labelCachInfo->setAnchorPoint(cocos2d::Vec2(0.0f, 0.0f));
	_labelCachInfo->setPosition(info_x, info_y);
	_labelCachInfo->setScale(0.35);
	
	info_y -= 20.0f;
	
	_labelMembersInfo = cocos2d::Label::createWithBMFont("futura-48.fnt", "", cocos2d::TextHAlignment::LEFT);
	_labelMembersInfo->setAnchorPoint(cocos2d::Vec2(0.0f, 0.0f));
	_labelMembersInfo->setPosition(info_x, info_y);
	_labelMembersInfo->setScale(0.35);
		
	info_y -= 20.0f;
	
	_labelContentmentInfo = cocos2d::Label::createWithBMFont("futura-48.fnt", "", cocos2d::TextHAlignment::LEFT);
	_labelContentmentInfo->setAnchorPoint(cocos2d::Vec2(0.0f, 0.0f));
	_labelContentmentInfo->setPosition(info_x, info_y);
	_labelContentmentInfo->setScale(0.35);

	info_y -= 20.0f;
	
	_labelMoraleInfo = cocos2d::Label::createWithBMFont("futura-48.fnt", "", cocos2d::TextHAlignment::LEFT);
	_labelMoraleInfo->setAnchorPoint(cocos2d::Vec2(0.0f, 0.0f));
	_labelMoraleInfo->setPosition(info_x, info_y);
	_labelMoraleInfo->setScale(0.35);

	info_y -= 20.0f;
	
	_labelChildrensInfo = cocos2d::Label::createWithBMFont("futura-48.fnt", "", cocos2d::TextHAlignment::LEFT);
	_labelChildrensInfo->setAnchorPoint(cocos2d::Vec2(0.0f, 0.0f));
	_labelChildrensInfo->setPosition(info_x, info_y);
	_labelChildrensInfo->setScale(0.35);

	Cell::Ptr cell = _cell.lock();
	_cellCurrentTask = cell->getCurrentTask().lock();

	UpdateInfoBy(cell);

	if (_cellCurrentTask != nullptr)
	{
		float w = background->getContentSize().width - 50.0f;
		float x = center.x - background->getContentSize().width / 2.0f + 25.0f;
		float y = center.y - background->getContentSize().height / 2.0f + 25.0f;
		
		float time = World::Instance().GetWorldTime();
		float progress = _cellCurrentTask->CalculateProgress(time);

		_taskProgressBar = new ProgressBar(w, 10.0f, cocos2d::Color4F(1.0f, 0.5f, 0, 1.0f));
		_taskProgressBar->setPosition(x, y);
		_taskProgressBar->autorelease();
		_taskProgressBar->SetProgress(progress);

		std::string strTaskLabel = cocos2d::StringUtils::format("Cell is now performing %s task", _cellCurrentTask->GetInfo()->id.c_str());
		_currentTaskLabel = cocos2d::Label::createWithTTF(ttfConfig, strTaskLabel, cocos2d::TextHAlignment::CENTER);
		_currentTaskLabel->setPosition(center.x, y+25.0f);

		addChild(_taskProgressBar, 1);
		addChild(_currentTaskLabel, 1);
	}

	cocos2d::ScaleTo *scale = cocos2d::ScaleTo::create(0.8f, 1.0f, 1.0f);
	cocos2d::FadeIn *fade = cocos2d::FadeIn::create(0.5f);
	cocos2d::EaseElasticOut *elastic_scale = cocos2d::EaseElasticOut::create(scale, 5.0f);

	scheduleUpdate();
	setScale(0.01f);
	setOpacity(0);	
	runAction(elastic_scale);
	runAction(fade);
	addChild(background, 0);
	addChild(menu, 1);
	addChild(labelTitle, 1);
	addChild(_labelCachInfo, 1);
	addChild(_labelMembersInfo, 1);
	addChild(_labelContentmentInfo, 1);
	addChild(_labelMoraleInfo, 1);
	addChild(_labelChildrensInfo, 1);

	return true;
}

void CellInfoMenu::update(float dt)
{
	if (_cellCurrentTask)
	{
		if (_taskProgressBar->IsFinished())
		{
			removeChild(_taskProgressBar, true);
			removeChild(_currentTaskLabel, true);
			_cellCurrentTask = nullptr;
		}
		else
		{
			float time = World::Instance().GetWorldTime();
			float progress = _cellCurrentTask->CalculateProgress(time);
			_taskProgressBar->SetProgress(progress * 100.0f);
		}
	}
}

void CellInfoMenu::UpdateInfoBy(Cell::Ptr cell)
{
	Cell::Info info = cell->GetInfo();
	
	std::string strCashInfo = cocos2d::StringUtils::format("Cash: %.3f $", info.cash);
	std::string strMembersInfo = cocos2d::StringUtils::format("Members: %d", info.membersCount); 
	std::string strContentmentInfo = cocos2d::StringUtils::format("Contentment: %.1f %%", info.contentment);
	std::string strMoraleInfo = cocos2d::StringUtils::format("Morale: %.1f %%", info.morale);
	std::string strChildrensInfo = cocos2d::StringUtils::format("Childrens: %s",
	cocos2d::StringUtils::toString<int>(cell->GetChildrens().size()).c_str());

	_labelCachInfo->setString(strCashInfo);
	_labelMembersInfo->setString(strMembersInfo);
	_labelContentmentInfo->setString(strContentmentInfo);
	_labelMoraleInfo->setString(strMoraleInfo);
	_labelChildrensInfo->setString(strChildrensInfo);
}

void CellInfoMenu::_OnCloseCallback(cocos2d::Ref *sender)
{
	cocos2d::ScaleTo *scale = cocos2d::ScaleTo::create(0.2f, 0.2f, 0.01f);
	cocos2d::EaseElasticIn *elastic_scale = cocos2d::EaseElasticIn::create(scale, 5.0f);
	cocos2d::CallFunc *func = cocos2d::CallFunc::create(CC_CALLBACK_0(CellMenuSelector::OnCellMenuClosed, _cellMenuSelector));

	runAction(cocos2d::Sequence::create(elastic_scale, func, nullptr));
}
