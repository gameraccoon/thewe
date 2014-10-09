#ifndef CELL_MAP_WIDGET_H
#define CELL_MAP_WIDGET_H

#include "CellGameInterface.h"

class CellMapWidget : public cocos2d::Node
{
public:
	CellMapWidget(Cell::Ptr cell);

	virtual bool init(void) override;
	virtual void update(float dt) override;

	void SetHitArea(float beginX, float beginY, float endX, float endY);
	void GetHitArea(float &beginX, float &beginY, float &endX, float &endY) const;

	void SetProjectorUid(int uid);
	int GetProjectorUid(void) const;

	const cocos2d::Rect& GetCellRect(void) const;
	Cell::Ptr GetCell(void) const;

private:
	enum DrawOrder
	{
		SPRITE = 0,
		PROGRESS,
	};

private:
	Cell::Ptr _cell;

	float _hitAreaBeginX, _hitAreaEndX;
	float _hitAreaBeginY, _hitAreaEndY;

	int _projectorUid;

	cocos2d::Sprite *_cellMapSprite;
	RoundProgressBar *_cellMapTaskProgressBar;
	RoundProgressBar *_constructionProgress;

	Cell::State _lastCellState;
};

#endif
