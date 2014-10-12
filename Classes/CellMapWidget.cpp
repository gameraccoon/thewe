#include "CellMapWidget.h"

#include "World.h"
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
		case Cell::CONSTRUCTION:
		case Cell::READY:
			_stateNormal->setVisible(true);
			_stateArrested->setVisible(false);
			break;

		case Cell::ARRESTED:
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
		case Cell::CONSTRUCTION:
		case Cell::READY:
			return _stateNormal;
			break;

		case Cell::ARRESTED:
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

CellMapWidget::CellMapWidget(Cell::Ptr cell)
	: _cell(cell)
	, _hitAreaBeginX(0.0f)
	, _hitAreaBeginY(0.0f)
	, _hitAreaEndX(1.0f)
	, _hitAreaEndY(1.0f)
	, _projectorUid(-1)
	, _lastCellState(Cell::READY)
	, _cellMapSprite(nullptr)
	, _constructionProgress(nullptr)
{
	init();
}

bool CellMapWidget::init(void)
{
	_cellMapSprite = new CellMapImage(_cell->GetInfo().state);
	_cellMapSprite->setPosition(0.0f, 0.0f);
	_cellMapSprite->setScale(1.0f);
	_cellMapSprite->autorelease();

	_cellMapTaskProgressBar = new RoundProgressBar("cell_overlay.png", 1.0f);
	_cellMapTaskProgressBar->SetProgressImmediately(0.0f);
	_cellMapTaskProgressBar->setPosition(0.0f, 0.0f);
	_cellMapTaskProgressBar->setVisible(false);
	_cellMapTaskProgressBar->ToggleReverse(true);
	_cellMapTaskProgressBar->autorelease();

	_constructionProgress = new RoundProgressBar("cell.png", 0.8f);
	_constructionProgress->setPosition(0.0f, 0.0f);
	_constructionProgress->SetProgressImmediately(0.0f);
	
	addChild(_cellMapSprite, DrawOrder::SPRITE);
	addChild(_cellMapTaskProgressBar, DrawOrder::PROGRESS);
	addChild(_constructionProgress, DrawOrder::PROGRESS);
	scheduleUpdate();

	return true;
}

void CellMapWidget::update(float dt)
{
	Utils::GameTime currentTime = Utils::GetGameTime();
	_cell->UpdateToTime(currentTime);
	if (_cell->GetInfo().state == Cell::CONSTRUCTION)
	{
		_constructionProgress->SetProgressImmediately(_cell->GetConstructionProgress(currentTime) * 100.0f);
		_constructionProgress->setVisible(true);
		_cellMapSprite->setVisible(false);
		_lastCellState = Cell::CONSTRUCTION;
	}
	else if (_cell->GetInfo().state == Cell::READY && _lastCellState == Cell::CONSTRUCTION)
	{
		World::Instance().GetMessageManager().SendGameMessage("Cell created");
		removeChild(_constructionProgress);
		_constructionProgress->setVisible(false);
		_cellMapSprite->setVisible(true);
		_lastCellState = Cell::READY;
	}
	else if (_cell->GetInfo().state == Cell::ARRESTED && _lastCellState == Cell::READY)
	{
		_cellMapSprite->SwitchState(Cell::ARRESTED);
		_lastCellState = Cell::ARRESTED;
	}

	if (_cell->IsCurrentTaskExists())
	{
		Task::Ptr task = _cell->getCurrentTask().lock();
		
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

const cocos2d::Rect& CellMapWidget::GetCellRect(void) const
{
	return _cellMapSprite->GetCurrentStateImage()->getTextureRect();
}

Cell::Ptr CellMapWidget::GetCell(void) const
{
	return _cell;
}
