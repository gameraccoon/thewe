#ifndef INVESTIGATOR_MAP_WIDGET_H
#define INVESTIGATOR_MAP_WIDGET_H

#include "CellGameInterface.h"
#include "Investigator.h"

class CellMapWidget;

class InvestigatorMapWidget : public cocos2d::Node
{
public:
	InvestigatorMapWidget(Investigator::Ptr investigator, MapProjector *proj, WorldMapLayer *worldMapLayer);

	virtual bool init(void) override;
	virtual void update(float dt) override;

	Investigator::Ptr GetInvestigator(void) const;

	void SetProjectorUid(int uid);
	int GetProjectorUid(void) const;
	int GetInvestigatorUid(void) const;

public:
	void OnCatchInFirstCell();
	void OnUncachedInFirstCell();

	Investigator::Ptr _investigator;
	Investigator::State _lastState;
	CellMapWidget *_invesRootCellWidget;
	WorldMapLayer *_worldMapLayer;
	MapProjector *_projector;

	int _projectorUid;
	const int _investigatorUid;

	cocos2d::DrawNode *_investigationDrawer;
};

#endif
