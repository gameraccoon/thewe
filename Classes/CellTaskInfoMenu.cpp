#include "CellTaskInfoMenu.h"

CellTaskInfoMenu::CellTaskInfoMenu(Cell::WeakPtr cell, CellMenuSelector *selector)
	: _cell(cell)
	, _cellMenuSelector(selector)
{
	init();
}

bool CellTaskInfoMenu::init(void)
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
			"cell-tasks-menu-close-pressed.png", CC_CALLBACK_1(CellTaskInfoMenu::_OnCloseCallback, this));
	}

	cocos2d::Menu *menu = cocos2d::Menu::create(closeButton, nullptr);
	menu->setPosition(center);
	cocos2d::Sprite *background = cocos2d::Sprite::create("cell-tasks-menu.png");
	background->setPosition(center);
	cocos2d::TTFConfig ttfConfig("arial.ttf", 18);
	cocos2d::Label *labelTitle = cocos2d::Label::createWithTTF(ttfConfig, "Cell Current Task Menu", cocos2d::TextHAlignment::CENTER);

	float close_x = background->getContentSize().width  / 2 - closeButton->getContentSize().width  + 5.0f;
	float close_y = background->getContentSize().height / 2 - closeButton->getContentSize().height + 5.0f;
	closeButton->setPosition(close_x, close_y);

	float title_x = center.x;
	float title_y = center.y + background->getContentSize().height / 2 - 16.0f;
	labelTitle->setPosition(title_x, title_y);
	labelTitle->setColor(cocos2d::Color3B(255, 255, 255));
	
	_cellCurrentTask = _cell.lock()->getCurrentTask().lock();
	float w = background->getContentSize().width - 50.0f;
	float x = center.x - background->getContentSize().width / 2.0f + 25.0f;
	float y = center.y;
	float time = World::Instance().GetWorldTime();
	float progress = _cellCurrentTask->CalculateProgress(time);

	_taskProgressBar = new ProgressBar(w, 50.0f, cocos2d::Color4F(1.0f, 0.5f, 0, 1.0f));
	_taskProgressBar->setPosition(x, y);
	_taskProgressBar->autorelease();
	_taskProgressBar->SetProgress(progress);

	_taskProgressLabel = cocos2d::Label::createWithTTF(ttfConfig, "", cocos2d::TextHAlignment::CENTER);
	_taskProgressLabel->setPosition(center.x, y-25.0f);


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
	addChild(_taskProgressBar, 1);
	addChild(_taskProgressLabel, 1);

	return true;
}

void CellTaskInfoMenu::update(float dt)
{
	if (_cellCurrentTask)
	{
		if (_taskProgressBar->IsFinished())
		{
			removeChild(_taskProgressBar, true);
			removeChild(_taskProgressLabel, true);
			_cellCurrentTask = nullptr;
		}
		else
		{
			float time = World::Instance().GetWorldTime();
			float progress = _cellCurrentTask->CalculateProgress(time);
			_taskProgressBar->SetProgress(progress * 100.0f);

			std::string label = cocos2d::StringUtils::format("(%d %%)", (int)(progress * 100.0f));
			_taskProgressLabel->setString(label);
		}
	}
}

void CellTaskInfoMenu::_OnCloseCallback(cocos2d::Ref *sender)
{
	cocos2d::ScaleTo *scale = cocos2d::ScaleTo::create(0.2f, 0.2f, 0.01f);
	cocos2d::EaseElasticIn *elastic_scale = cocos2d::EaseElasticIn::create(scale, 5.0f);
	cocos2d::CallFunc *func = cocos2d::CallFunc::create(CC_CALLBACK_0(CellMenuSelector::OnCellMenuClosed, _cellMenuSelector));

	runAction(cocos2d::Sequence::create(elastic_scale, func, nullptr));
}