#include "CellMapWidget.h"

#include "World.h"
#include "GameInfo.h"
#include "Log.h"
#include "RelinkDragAndDrop.h"
#include "WorldMapLayer.h"

CellMapWidget::CellMapWidget(WorldMapLayer *worldMapLayer, MapProjector *projector, Cell::WeakPtr cell)
	: _worldMapLayer(worldMapLayer)
	, _projector(projector)
	, _cell(cell)
	, _hitArea(0.0f, 0.0f, 1.0f, 1.0f)
	, _relinkMarkYAngle(0.0f)
	, _projectorUid(-1)
	, _isRelinkMode(false)
	, _cellUid(cell.lock()->GetUid())
	, _lastCellState(Cell::State::READY)
	, _cellMapSprite(nullptr)
	, _cellCommonProgressBar(nullptr)
{
	MessageManager::Instance().RegisterReceiver(this, "ShowBonus");
	MessageManager::Instance().RegisterReceiver(this, "PushTaskRewardOnMap");
	MessageManager::Instance().RegisterReceiver(this, "CellLevelUp");
	init();
}

CellMapWidget::~CellMapWidget(void)
{
}

bool CellMapWidget::init(void)
{
	if (!cocos2d::Node::init())
	{
		return false;
	}

	cocos2d::EventListenerTouchAllAtOnce *touch = cocos2d::EventListenerTouchAllAtOnce::create();
	touch->onTouchesBegan = CC_CALLBACK_2(CellMapWidget::TouchBegan, this);
	touch->onTouchesEnded = CC_CALLBACK_2(CellMapWidget::TouchEnded, this);
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touch, this);

	_cellMapSprite = MultipleImageSprite::create();
	_cellMapSprite->setPosition(0.0f, 0.0f);
	_cellMapSprite->setScale(1.0f);
	_cellMapSprite->AddImage("normal", "cell.png");
	_cellMapSprite->AddImage("arrested", "cell_arrested.png");
	_cellMapSprite->SetCurrentImage("normal");

	_cellMapTaskProgressBar = RoundProgressBar::create("cell_overlay.png", 1.0f);
	_cellMapTaskProgressBar->SetProgressImmediately(0.0f);
	_cellMapTaskProgressBar->setPosition(0.0f, 0.0f);
	_cellMapTaskProgressBar->setVisible(false);
	_cellMapTaskProgressBar->ToggleReverse(true);

	_cellCommonProgressBar = RoundProgressBar::create("cell.png", 0.8f);
	_cellCommonProgressBar->setPosition(0.0f, 0.0f);
	_cellCommonProgressBar->SetProgressImmediately(0.0f);

	_popupCatchInvestigator = InvestigatorTapButton::create();
	_popupCatchInvestigator->setPosition(0.0f, 0.0f);
	_popupCatchInvestigator->setScale(8.5f);

	addChild(_cellMapSprite, DrawOrder::SPRITE);
	addChild(_cellMapTaskProgressBar, DrawOrder::PROGRESS);
	addChild(_cellCommonProgressBar, DrawOrder::PROGRESS);
	addChild(_popupCatchInvestigator, DrawOrder::BUTTON);
	scheduleUpdate();

	setContentSize(_cellMapTaskProgressBar->getContentSize());

	return true;
}

void CellMapWidget::update(float dt)
{
	Cell::Ptr cell = _cell.lock();

	if (!cell)
	{
		return;
	}

	if (_popupCatchInvestigator)
	{
		_popupCatchInvestigator->update(dt);
	}

	Utils::GameTime currentTime = Utils::GetGameTime();
	cell->UpdateToTime(currentTime);

	if (!_isRelinkMode && World::Instance().GetCellsNetwork().IsCellRelinkable(_cell))
	{
		RelinkDragAndDrop *relink = new RelinkDragAndDrop(this, _worldMapLayer, _projector, _cell);
		relink->setPosition(0.0f, 0.0f);
		relink->autorelease();
		_worldMapLayer->AddEffectGameField(relink);
		_isRelinkMode = true;
	}

	if (cell->IsState(Cell::State::CONSTRUCTION))
	{
		_cellCommonProgressBar->SetProgressImmediately(cell->GetStateProgress(currentTime) * 100.0f);
		_cellCommonProgressBar->setVisible(true);
		_cellMapSprite->setVisible(false);
		_lastCellState = Cell::State::CONSTRUCTION;
	}
	else if (cell->IsState(Cell::State::READY) && _lastCellState == Cell::State::CONSTRUCTION)
	{
		World::Instance().GetMessageManager().SendGameMessage("Cell created");
		_cellCommonProgressBar->setVisible(false);
		_cellMapSprite->setVisible(true);
		_lastCellState = Cell::State::READY;
	}
	else if (cell->IsState(Cell::State::ARRESTED) && _lastCellState == Cell::State::READY)
	{
		_cellMapSprite->SetCurrentImage("arrested");
		_lastCellState = Cell::State::ARRESTED;
	}
	else if (cell->IsState(Cell::State::AUTONOMY))
	{
		float progress = abs(1.0f - cell->GetStateProgress(currentTime));
		if (progress <= 1.0f)
		{
			_cellMapTaskProgressBar->SetProgressImmediately(progress * 100.0f);
			_cellMapTaskProgressBar->setVisible(true);
		}
	}
	else if (cell->IsState(Cell::State::DESTRUCTION))
	{
		float progress = abs(1.0f - cell->GetStateProgress(currentTime));
		if (progress <= 1.0f)
		{
			_cellCommonProgressBar->SetProgressImmediately(progress * 100.0f);
			_cellCommonProgressBar->setVisible(true);
			_cellMapTaskProgressBar->setVisible(false);
			_cellMapSprite->setVisible(false);
		}
	}

	if (cell->IsState(Cell::State::READY))
	{
		if (_isRelinkMode) {
			_isRelinkMode = false;
		}

		if (cell->IsCurrentTaskExists())
		{
			Task::Ptr task = cell->getCurrentTask().lock();
		
			Utils::GameTime time = Utils::GetGameTime();
			float progress = task->CalculateProgress(time);
			_cellMapTaskProgressBar->SetProgressImmediately(100.0f - progress * 100.0f);
			_cellMapTaskProgressBar->setVisible(true);
		}
		else
		{
			_cellMapTaskProgressBar->setVisible(false);
		}
	}
}

void CellMapWidget::TouchBegan(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
	Vector2 location = convertTouchToNodeSpace(touches.at(0));
	if (_cellMapSprite->GetCurrentImage()->getBoundingBox().containsPoint(location)) {
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("tap-single.wav");

		// test code
		/*
		PopUpTextWithIcon::ConstructionInfo info;
		info.icon = "task_reward.png";
		info.text = "Very loooooooong text test...";
		info.position = _cell.lock()->GetInfo().location;
		info.overralScale = 1.0f;
		info.iconScale = 0.3f;
		info.font = "EuropeNormal.ttf";
		info.fontSize = 25;
		PopUpTextWithIcon *eff = new PopUpTextWithIcon(info, _projector, 1);
		_worldMapLayer->AddEffectGameField(eff);
		*/
	}
}

void CellMapWidget::TouchEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
	Vector2 location = convertTouchToNodeSpace(touches.at(0));
	if (_cellMapSprite->GetCurrentImage()->getBoundingBox().containsPoint(location)) {
		/*for (TaskRewardMapWidget *reward : _taskRewardsOnMap)
		{
			reward->PickReward();
		}*/
	}
}

void CellMapWidget::AcceptMessage(const Message &message)
{
	if (message.is("ShowBonus") && (unsigned int)message.variables.GetInt("CELL_UID") == _cell.lock()->GetUid())
	{
		BonusMapWidget *bonus = new BonusMapWidget(this, _cell,
												  Vector2(0.0f, 50.0f),
												  GameInfo::Instance().GetTime("BONUS_LIVE_TIME"));
		bonus->SetBonusBehavior(World::Instance().GetBonusCallback(_cell));
		bonus->autorelease();
		_worldMapLayer->AddEffectGameField(bonus);
	}
	else if (message.is("PushTaskRewardOnMap") && (unsigned int)message.variables.GetInt("CELL_UID") == _cell.lock()->GetUid())
	{
		Task::Info task_info = World::Instance().GetTaskManager().FindTaskById(message.variables.GetString("TASK_ID"));
		TaskRewardMapWidget *reward = new TaskRewardMapWidget(
			this,
			_cell,
			task_info,
			message.variables.GetTime("WAIT_TIME"),
			cocos2d::Vec2(0.0f, 185.0f),
			2.7f);
		reward->autorelease();
		_worldMapLayer->AddEffectGameField(reward);
	}
	else if (message.is("CellLevelUp") && _cell.lock()->GetUid() == message.variables.GetInt("cellUid"))
	{
		PopUpTextWithIcon::ConstructionInfo info;
		info.icon = "";
		info.text = cocos2d::StringUtils::format("Level %d Reached", message.variables.GetInt("levelAfter"));
		info.position = _cell.lock()->GetInfo().location;
		info.overralScale = 1.0f;
		info.iconScale = 0.3f;
		info.font = "EuropeNormal.ttf";
		info.fontSize = 20;
		PopUpTextWithIcon *eff = new PopUpTextWithIcon(info, _projector, true, 1);
		_worldMapLayer->AddEffectGameField(eff);
	}
}

void CellMapWidget::ShowInvestigatorLaunchButton(InvestigatorTapButton::Callback sucessCallback, InvestigatorTapButton::Callback failureCallback)
{
	float tickChange;
	float tapChange;
	tickChange = GameInfo::Instance().GetFloat("INVESTIGATOR_CATCH_TICK_CHANGE");
	tapChange = GameInfo::Instance().GetFloat("INVESTIGATOR_CATCH_TAP_CHANGE");
	
	if (World::Instance().GetTutorialManager().IsTutorialStateAvailable("WaitForFirstInvestigator"))
	{
		World::Instance().GetTutorialManager().RunTutorialFunction("FirstInvestigationStarted");
		tickChange = GameInfo::Instance().GetFloat("INVESTIGATOR_TUTORIAL_CATCH_TICK_CHANGE");
	}

	_popupCatchInvestigator->setSuccessCallback(sucessCallback);
	_popupCatchInvestigator->setFailureCallback(failureCallback);
	_popupCatchInvestigator->appear(10.0f);
	_popupCatchInvestigator->initRounding(tickChange, tapChange);
	_popupCatchInvestigator->startRounding(1.0f);
}

void CellMapWidget::HideInvestigatorLaunchButton()
{
	_popupCatchInvestigator->hide();
}

void CellMapWidget::SetHitArea(const cocos2d::Rect& hitArea)
{
	_hitArea = hitArea;
}

cocos2d::Rect CellMapWidget::GetHitArea() const
{
	return _hitArea;
}

void CellMapWidget::SetProjectorUid(int uid)
{
	_projectorUid = uid;
}

int CellMapWidget::GetProjectorUid(void) const
{
	return _projectorUid;
}

int CellMapWidget::GetCellUid(void) const
{
	return _cellUid;
}

const cocos2d::Rect& CellMapWidget::GetCellRect(void) const
{
	return _cellMapSprite->GetCurrentImage()->getTextureRect();
}

Cell::WeakPtr CellMapWidget::GetCell(void) const
{
	return _cell;
}
