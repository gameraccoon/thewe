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

	if (_cell->GetState() == Cell::CONSTRUCTION)
	{
		_cellMapSprite->setVisible(false);

		_constructionProgress = new RoundProgressBar("cell.png", 0.8f);
		_constructionProgress->setPosition(0.0f, 0.0f);
		_constructionProgress->SetProgressAnimated(100.0f, _cell->GetConstructionTime());

		addChild(_constructionProgress, DrawOrder::PROGRESS);
	}

	addChild(_cellMapSprite, DrawOrder::SPRITE);
	scheduleUpdate();

	return true;
}

void CellMapWidget::update(float dt)
{
	if (_cell->GetState() == Cell::CONSTRUCTION && _constructionProgress->IsFinished())
	{
		_cellMapSprite->setVisible(true);
		_cell->SwitchState(Cell::READY);

		MessageManager::Instance().SendGameMessage("Cell created");

		removeChild(_constructionProgress);
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