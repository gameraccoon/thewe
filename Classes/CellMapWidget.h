#ifndef CELL_MAP_WIDGET_H
#define CELL_MAP_WIDGET_H

#include "CellGameInterface.h"
#include "CellMapPopupButton.h"
#include "MessageManager.h"
#include "InvestigatorMapWidget.h"
#include "TaskRewardMapWidget.h"

class CellMapImage;

class CellMapWidget : public cocos2d::Node, public MessageReceiver
{
public:
	CellMapWidget(Cell::WeakPtr cell);
	~CellMapWidget(void);

	virtual bool init(void) override;
	virtual void update(float dt) override;

	void TouchEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);

	void AcceptMessage(const Message &message) override;
	
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
		,REWARD
		,BUTTON
	};

	typedef std::vector<TaskRewardMapWidget *> TaskRewards;

private:
	Cell::WeakPtr _cell;

	float _hitAreaBeginX, _hitAreaEndX;
	float _hitAreaBeginY, _hitAreaEndY;
	float _relinkMarkYAngle;

	int _projectorUid;
	const int _cellUid;

	cocos2d::Sprite *_relinkableMark;

	TaskRewards _taskRewardsOnMap;

	CellMapImage *_cellMapSprite;
	RoundProgressBar *_cellMapTaskProgressBar;
	RoundProgressBar *_cellCommonProgressBar;

	CellMapPopupButton *_popupCatchInvestigator;

	Cell::State _lastCellState;
};

#endif
