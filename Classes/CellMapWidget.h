#ifndef CELL_MAP_WIDGET_H
#define CELL_MAP_WIDGET_H

#include "CellGameInterface.h"
#include "CellMapPopupButton.h"
#include "InvestigatorMapWidget.h"

class CellMapImage;

class CellMapWidget : public cocos2d::Node
{
public:
	CellMapWidget(Cell::WeakPtr cell);
	~CellMapWidget(void);

	virtual bool init(void) override;
	virtual void update(float dt) override;
	
	void ShowInvestigatorLaunchButton(cocos2d::ccMenuCallback onCatchCallback);
	void HideInvestigatorLaunchButton(bool hideWithWarning);

	void SetHitArea(float beginX, float beginY, float endX, float endY);
	void GetHitArea(float &beginX, float &beginY, float &endX, float &endY) const;

	void SetProjectorUid(int uid);
	int GetProjectorUid(void) const;
	int GetCellUid(void) const;

	const cocos2d::Rect& GetCellRect(void) const;
	Cell::WeakPtr GetCell(void) const;

private:
	enum DrawOrder
	{
		SPRITE = 0
		,PROGRESS
		,BUTTON
	};

private:
	Cell::WeakPtr _cell;

	float _hitAreaBeginX, _hitAreaEndX;
	float _hitAreaBeginY, _hitAreaEndY;

	int _projectorUid;
	const int _cellUid;

	CellMapImage *_cellMapSprite;
	RoundProgressBar *_cellMapTaskProgressBar;
	RoundProgressBar *_constructionProgress;

	CellMapPopupButton *_popupCatchInvestigator;

	Cell::State _lastCellState;
};

#endif
