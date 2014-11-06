#include "CellInfoMenu.h"

CellInfoMenu::CellInfoMenu(Cell::WeakPtr cell, CellMenuSelector *selector)
	: _cell(cell)
	, _cellMenuSelector(selector)
	, _taskProgressBar(nullptr)
{
	init();
}

cocos2d::Label *CreateTTFLabel(cocos2d::Node *parent,
							   const std::string text,
							   Vector2 position,
							   Vector2 anchorPoint = cocos2d::Vec2(0.0f, 0.0f),
							   cocos2d::TextHAlignment aligment = cocos2d::TextHAlignment::LEFT)
{
	cocos2d::Label *label = cocos2d::Label::createWithBMFont("arial-26-en-ru.fnt", text, aligment);
	label->setAnchorPoint(anchorPoint);
	label->setPosition(position);
	parent->addChild(label, 1);
	return label;
}

SquareProgressBar *CreateProgressBar(cocos2d::Node *parent,
									 Vector2 position,
									 Vector2 size,
									 Color color,
									 float progress)
{
	SquareProgressBar *progressBar = new SquareProgressBar(size.x, size.y, color);
	progressBar->setPosition(position);
	progressBar->autorelease();
	progressBar->SetProgressPercentage(progress * 100.0f);
	parent->addChild(progressBar, 1);
	return progressBar;
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

	float close_x = background->getContentSize().width  / 2 - closeButton->getContentSize().width  + 23.0f;
	float close_y = background->getContentSize().height / 2 - closeButton->getContentSize().height + 17.0f;
	closeButton->setAnchorPoint(Vector2(0.5f, 0.5f));
	closeButton->setPosition(close_x, close_y);

	float textPadding = 27.0f;

	float title_x = center.x;
	float title_y = center.y + background->getContentSize().height / 2 - 16.0f;
	labelTitle->setPosition(title_x, title_y);
	labelTitle->setColor(cocos2d::Color3B(255, 255, 255));
	
	float info_x = center.x - background->getContentSize().width  / 2.0f + textPadding;
	float info_y = center.y + background->getContentSize().height / 2.0f - 75.0f;

	_levelProgressBar = CreateProgressBar(this, Vector2(center.x - 150.0f, info_y - 2.0f),
										  Vector2(300.0f, 30.0f), Color(0.0f, 0.5f, 0.0f, 1.0f),
										  0.0f);
	_labelLevelInfo = CreateTTFLabel(this, "", Vector2(center.x, info_y), Vector2(0.5f, 0.0f));

	info_y -= 45.0f;
	_labelMembersInfo = CreateTTFLabel(this, "", Vector2(info_x, info_y));
	_labelCashInfo = CreateTTFLabel(this, "",
									Vector2(info_x + background->getContentSize().width - textPadding * 2, info_y),
									Vector2(1.0f, 0.0f));

	info_y -= 37.0f;
	_labelPursuedLevelInfo = CreateTTFLabel(this, "", Vector2(center.x, info_y), Vector2(0.5f, 0.0f));

	info_y -= 40.0f;
	CreateTTFLabel(this, "Morale", Vector2(info_x + 170.0f, info_y), Vector2(1.0f, 0.0f));
	_moraleProgressBar = CreateProgressBar(this, Vector2(info_x + 180.0f, info_y + 4.0f),
										  Vector2(200.0f, 15.0f), Color(0.0f, 0.5f, 0.0f, 1.0f),
										  0.0f);
	_moraleProgressBar->SetBorderColor(Color(0.5f, 0.5f, 0.5f, 1.0f));

	info_y -= 30.0f;
	CreateTTFLabel(this, "Devotion", Vector2(info_x + 170.0f, info_y), Vector2(1.0f, 0.0f));
	_devotionProgressBar = CreateProgressBar(this, Vector2(info_x + 180.0f, info_y + 4.0f),
										  Vector2(200.0f, 15.0f), Color(0.0f, 0.5f, 0.0f, 1.0f),
										  0.0f);
	_devotionProgressBar->SetBorderColor(Color(0.5f, 0.5f, 0.5f, 1.0f));

	/*info_y -= 30.0f;
	CreateTTFLabel(this, "Town influence", Vector2(info_x + 170.0f, info_y), Vector2(1.0f, 0.0f));
	_townInfluenceProgressBar = CreateProgressBar(this, Vector2(info_x + 180.0f, info_y + 4.0f),
										  Vector2(200.0f, 15.0f), Color(0.0f, 0.5f, 0.0f, 1.0f),
										  0.0f);
	_townInfluenceProgressBar->SetBorderColor(Color(0.5f, 0.5f, 0.5f, 1.0f));*/

	Cell::Ptr cell = _cell.lock();
	_cellCurrentTask = cell->getCurrentTask();

	UpdateInfoBy(cell);
	Task::Ptr currentTask = _cellCurrentTask.lock();
	if (currentTask)
	{
		float w = background->getContentSize().width - 50.0f;
		float x = center.x - background->getContentSize().width / 2.0f + 25.0f;
		float y = center.y - background->getContentSize().height / 2.0f + 25.0f;
		
		Utils::GameTime time = Utils::GetGameTime();
		float progress = currentTask->CalculateProgress(time);

		_taskProgressBar = CreateProgressBar(this, Vector2(x, y), Vector2(w, 10.0f), Color(1.0f, 0.5f, 0, 1.0f), progress);

		std::string strTaskLabel = cocos2d::StringUtils::format("Current task: %s", currentTask->GetInfo()->title.c_str());
		_currentTaskLabel = cocos2d::Label::createWithTTF(ttfConfig, strTaskLabel, cocos2d::TextHAlignment::CENTER);
		_currentTaskLabel->setPosition(center.x, y+25.0f);

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

	return true;
}

void CellInfoMenu::update(float dt)
{
	Task::Ptr currentTask = _cellCurrentTask.lock();
	if (currentTask)
	{
		Utils::GameTime time = Utils::GetGameTime();
		float progress = currentTask->CalculateProgress(time);
		_taskProgressBar->SetProgressPercentage(progress * 100.0f);
	}
	else
	{
		if (_taskProgressBar)
		{
			removeChild(_taskProgressBar, true);
			removeChild(_currentTaskLabel, true);
			_taskProgressBar = nullptr;
			_currentTaskLabel = nullptr;
		}
	}
}

void CellInfoMenu::UpdateInfoBy(Cell::Ptr cell)
{
	Cell::Info info = cell->GetInfo();

	int level = World::Instance().GetLevelFromExperience(info.experience);
	std::string strLevelInfo = cocos2d::StringUtils::format("Level %d", level);
	_labelLevelInfo->setString(strLevelInfo);

	int currentLevelExp = World::Instance().GetExperienceForLevel(level);
	int expBetweenLevels = World::Instance().GetExperienceForLevel(level + 1) - currentLevelExp;
	int earnedLevelExp = info.experience - currentLevelExp;
	float levelProgress = 1.0f * earnedLevelExp / expBetweenLevels;
	_levelProgressBar->SetProgressPercentage(levelProgress * 100.0f);

	std::string strCashInfo = cocos2d::StringUtils::format("Cash: %d $", info.cash);
	_labelCashInfo->setString(strCashInfo);

	std::string strMembersInfo = cocos2d::StringUtils::format("Members: %d", info.membersCount);
	_labelMembersInfo->setString(strMembersInfo);

	float pursuedLevel = World::Instance().GetCellPursuedLevel(cell.get());
	if (pursuedLevel < 33.0f)
	{
		_labelPursuedLevelInfo->setString("Low pursued level");
	}
	else if (pursuedLevel < 66.0f)
	{
		_labelPursuedLevelInfo->setString("Middle pursued level");
	}
	else
	{
		_labelPursuedLevelInfo->setString("High pursued level");
	}

	_moraleProgressBar->SetProgressPercentage(info.morale * 100.0f);
	_devotionProgressBar->SetProgressPercentage(info.devotion * 100.0f);
	//_townInfluenceProgressBar->SetProgressPercentage(info.townInfluence * 100.0f);
}

void CellInfoMenu::_OnCloseCallback(cocos2d::Ref *sender)
{
	_CloseMenu();
}

void CellInfoMenu::_CloseMenu()
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
		_CloseMenu();
	}
}
