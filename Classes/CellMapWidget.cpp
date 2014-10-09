#include "CellMapWidget.h"

#include "MessageManager.h"

CellMapWidget::CellMapWidget(Cell::Ptr cell)
	: _cell(cell)
	, _hitAreaBeginX(0.0f)
	, _hitAreaBeginY(0.0f)
	, _hitAreaEndX(1.0f)
	, _hitAreaEndY(1.0f)
	, _projectorUid(-1)
	, _cellMapSprite(nullptr)
	, _constructionProgress(nullptr)
{
	init();
}

bool CellMapWidget::init(void)
{
	_cellMapSprite = cocos2d::Sprite::create("cell.png");
	_cellMapSprite->setPosition(0.0f, 0.0f);
	_cellMapSprite->setScale(1.0f);

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
		MessageManager::Instance().SendGameMessage("Cell created");
		removeChild(_constructionProgress);
		_constructionProgress->setVisible(false);
		_cellMapSprite->setVisible(true);
		_lastCellState = Cell::READY;
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
	return _cellMapSprite->getTextureRect();
}

Cell::Ptr CellMapWidget::GetCell(void) const
{
	return _cell;
}
