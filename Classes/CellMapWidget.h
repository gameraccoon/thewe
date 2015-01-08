#ifndef CELL_MAP_WIDGET_H
#define CELL_MAP_WIDGET_H

#include "CellGameInterface.h"
#include "ProgressTapWidget.h"
#include "MessageManager.h"
#include "InvestigatorMapWidget.h"
#include "TaskRewardMapWidget.h"
#include "MultipleImageSprite.h"
#include "BonusMapWidget.h"

class CellMapWidget : public cocos2d::Node, public MessageReceiver
{
public:
	CellMapWidget(WorldMapLayer *worldMapLayer, MapProjector *projector, Cell::WeakPtr cell);
	~CellMapWidget(void);

	virtual bool init(void) override;
	virtual void update(float dt) override;

	void TouchEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);

	void AcceptMessage(const Message &message) override;

	void ShowInvestigatorLaunchButton(ProgressTapWidget::Callback sucessCallback, ProgressTapWidget::Callback failureCallback);
	void HideInvestigatorLaunchButton();

	void SetHitArea(const cocos2d::Rect& hitArea);
	cocos2d::Rect GetHitArea() const;

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
		,REWARD
		,BUTTON
		,RELINK_WIDGET
	};

	typedef std::vector<BonusMapWidget *> Bonuses;

private:
	WorldMapLayer *_worldMapLayer;
	MapProjector *_projector;
	Cell::WeakPtr _cell;

	cocos2d::Rect _hitArea;
	float _relinkMarkYAngle;

	int _projectorUid;
	const int _cellUid;
	bool _isRelinkMode;

	MultipleImageSprite *_cellMapSprite;
	RoundProgressBar *_cellMapTaskProgressBar;
	RoundProgressBar *_cellCommonProgressBar;
	ProgressTapWidget *_popupCatchInvestigator;

	Cell::State _lastCellState;

	Bonuses _bonuses;
};

#endif
