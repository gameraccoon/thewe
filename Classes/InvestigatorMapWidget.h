#ifndef INVESTIGATOR_MAP_WIDGET_H
#define INVESTIGATOR_MAP_WIDGET_H

#include "CellGameInterface.h"
#include "Investigator.h"

class InvestigatorMapWidget : public cocos2d::Node
{
public:
	InvestigatorMapWidget(Investigator::Ptr investigator, MapProjector *proj);

	virtual bool init(void) override;
	virtual void update(float dt) override;

	Investigator::Ptr GetInvestigator(void) const;

	void SetProjectorUid(int uid);
	int GetProjectorUid(void) const;

public:
	void UpdateInvestigationMap(const Investigator::BranchBundle &bundle);

	Investigator::Ptr _investigator;
	MapProjector *_projector;

	int _projectorUid;

	cocos2d::DrawNode *_investigationDrawer;
};

#endif