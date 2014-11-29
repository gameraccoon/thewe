#include "CellMapWidget.h"

#include "World.h"
#include "GameInfo.h"
#include "Log.h"

class CellMapImage : public cocos2d::Node
{
public:
	CellMapImage(Cell::State state) : _state(state)
	{
		init();
		SwitchState(_state);
	}

	virtual bool init(void) override
	{
		_stateNormal = cocos2d::Sprite::create("cell.png");
		_stateNormal->setPosition(0.0f, 0.0f);
		_stateNormal->setVisible(false);

		_stateArrested = cocos2d::Sprite::create("cell_arrested.png");
		_stateArrested->setPosition(0.0f, 0.0f);
		_stateArrested->setVisible(false);

		addChild(_stateNormal, 0);
		addChild(_stateArrested, 1);

		return true;
	}

	void SwitchState(Cell::State state)
	{
		_state = state;

		switch (_state)
		{
		case Cell::State::CONSTRUCTION:
		case Cell::State::DESTRUCTION:
		case Cell::State::AUTONOMY:
		case Cell::State::READY:
			_stateNormal->setVisible(true);
			_stateArrested->setVisible(false);
			break;
		case Cell::State::ARRESTED:
			_stateNormal->setVisible(false);
			_stateArrested->setVisible(true);
			break;
		default:
			Log::Instance().writeWarning("Unknown cell state");
			break;
		}
	}

	cocos2d::Sprite* GetCurrentStateImage(void) const
	{
		switch (_state)
		{
		case Cell::State::CONSTRUCTION:
		case Cell::State::DESTRUCTION:
		case Cell::State::AUTONOMY:
		case Cell::State::READY:
			return _stateNormal;
			break;
		case Cell::State::ARRESTED:
			return _stateArrested;
			break;
		default:
			Log::Instance().writeWarning("Unknown cell state");
			return nullptr;
			break;
		}
	}

private:
	cocos2d::Sprite *_stateNormal;
	cocos2d::Sprite *_stateArrested;

	Cell::State _state;
};

CellMapWidget::CellMapWidget(Cell::WeakPtr cell)
	: _cell(cell)
	, _hitAreaBeginX(0.0f)
	, _hitAreaBeginY(0.0f)
	, _hitAreaEndX(1.0f)
	, _hitAreaEndY(1.0f)
	, _relinkMarkYAngle(0.0f)
	, _projectorUid(-1)
	, _cellUid(cell.lock()->GetUid())
	, _lastCellState(Cell::State::READY)
	, _cellMapSprite(nullptr)
	, _cellCommonProgressBar(nullptr)
{
	init();
}

CellMapWidget::~CellMapWidget(void)
{
	MessageManager::Instance().UnregisterReceiver(dynamic_cast<MessageReceiver *>(this));
}

bool CellMapWidget::init(void)
{
	if (!cocos2d::Node::init())
	{
		return false;
	}

	MessageManager::Instance().RegisterReceiver(dynamic_cast<MessageReceiver *>(this));

	cocos2d::EventListenerTouchAllAtOnce *touch = cocos2d::EventListenerTouchAllAtOnce::create();
	touch->onTouchesEnded = CC_CALLBACK_2(CellMapWidget::TouchEnded, this);
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touch, this);

	_cellMapSprite = new CellMapImage(_cell.lock()->GetInfo().state);
	_cellMapSprite->setPosition(0.0f, 0.0f);
	_cellMapSprite->setScale(1.0f);
	_cellMapSprite->autorelease();

	_cellMapTaskProgressBar = new RoundProgressBar("cell_overlay.png", 1.0f);
	_cellMapTaskProgressBar->SetProgressImmediately(0.0f);
	_cellMapTaskProgressBar->setPosition(0.0f, 0.0f);
	_cellMapTaskProgressBar->setVisible(false);
	_cellMapTaskProgressBar->ToggleReverse(true);
	_cellMapTaskProgressBar->autorelease();

	_cellCommonProgressBar = new RoundProgressBar("cell.png", 0.8f);
	_cellCommonProgressBar->setPosition(0.0f, 0.0f);
	_cellCommonProgressBar->SetProgressImmediately(0.0f);

	CellMapPopupButton::Settings s;
	s.normalStateImage = "marker_crosshair.png";
	s.pressedStateImage = "marker_crosshair_pressed.png";
	_popupCatchInvestigator = CellMapPopupButton::create(s);
	_popupCatchInvestigator->setPosition(0.0f, 0.0f);
	_popupCatchInvestigator->setScale(6.0f);

	_relinkableMark = cocos2d::Sprite::create("relink-mark.png");
	_relinkableMark->setPosition(0.0f, 155.0f);
	_relinkableMark->setScale(1.0f);
	_relinkableMark->setVisible(false);
	
	addChild(_cellMapSprite, DrawOrder::SPRITE);
	addChild(_cellMapTaskProgressBar, DrawOrder::PROGRESS);
	addChild(_cellCommonProgressBar, DrawOrder::PROGRESS);
	addChild(_popupCatchInvestigator, DrawOrder::BUTTON);
	addChild(_relinkableMark, DrawOrder::BUTTON);
	scheduleUpdate();

	return true;
}

void CellMapWidget::update(float dt)
{
	Cell::Ptr cell = _cell.lock();

	if (!cell)
	{
		return;
	}

	Utils::GameTime currentTime = Utils::GetGameTime();
	cell->UpdateToTime(currentTime);

	_relinkableMark->setVisible(World::Instance().GetCellsNetwork().IsCellRelinkable(cell));

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
		_cellMapSprite->SwitchState(Cell::State::ARRESTED);
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

		_relinkMarkYAngle += 180.0f * dt;
		_relinkableMark->setRotation3D(cocos2d::Vec3(0.0f, _relinkMarkYAngle, 0.0f));
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

void CellMapWidget::TouchEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
	for (TaskRewardMapWidget *reward : _taskRewardsOnMap)
	{
		reward->PickReward();
	}
}

void CellMapWidget::AcceptMessage(const Message &message)
{
	if (message.is("PushTaskRewardOnMap") && (unsigned int)message.variables.GetInt("CELL_UID") == _cell.lock()->GetUid())
	{
		Task::Info task_info = World::Instance().GetTaskManager().FindTaskById(message.variables.GetString("TASK_ID"));

		TaskRewardMapWidget *reward = new TaskRewardMapWidget(
			_cell,
			task_info,
			GameInfo::Instance().GetTime("TASK_REWARD_WAIT_TIME"),
			cocos2d::Vec2(0.0f, 185.0f),
			2.7f);

		reward->autorelease();
		addChild(reward);

		_taskRewardsOnMap.push_back(reward);
	}
	else if (message.is("DeleteTaskRewardWidget") && (unsigned int)message.variables.GetInt("CELL_UID") == _cell.lock()->GetUid())
	{
		for (TaskRewards::iterator it = _taskRewardsOnMap.begin(); it != _taskRewardsOnMap.end();)
		{
			std::string id = message.variables.GetString("TASK_ID");
			TaskRewardMapWidget *reward = (*it);
			
			if (reward->IsTaskId(id))
			{
				removeChild(reward);
				it = _taskRewardsOnMap.erase(it);
				break;
			}
			else
				++it;
		}
	}
}

void CellMapWidget::ShowInvestigatorLaunchButton(cocos2d::ccMenuCallback onCatchCallback)
{
	_popupCatchInvestigator->SetOnPressCallback(onCatchCallback);
	_popupCatchInvestigator->Appear(10.0f);
}

void CellMapWidget::HideInvestigatorLaunchButton(bool hideWithWarning)
{
	_popupCatchInvestigator->Disappear(hideWithWarning);
}

void CellMapWidget::SetHitArea(float beginX, float beginY, float endX, float endY)
{
	_hitAreaBeginX = beginX;
	_hitAreaEndX = endX;
	_hitAreaBeginY = beginY;
	_hitAreaEndY = endY;
}

void CellMapWidget::GetHitArea(float &beginX, float &endX, float &beginY, float &endY) const
{
	beginX = _hitAreaBeginX;
	endX = _hitAreaEndX;
	beginY = _hitAreaBeginY;
	endY = _hitAreaEndY;
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
	return _cellMapSprite->GetCurrentStateImage()->getTextureRect();
}

Cell::WeakPtr CellMapWidget::GetCell(void) const
{
	return _cell;
}
