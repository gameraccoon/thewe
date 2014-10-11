#ifndef INVESTIGATOR_MAP_WIDGET_H
#define INVESTIGATOR_MAP_WIDGET_H

#include "CellGameInterface.h"
#include "Investigator.h"

class InvestigatorMapWidget : public cocos2d::Node
{
public:
	InvestigatorMapWidget(Investigator::Ptr investigator);

	virtual bool init(void) override;
	virtual void update(float dt) override;

	Investigator::Ptr GetInvestigator(void) const;

	void SetProjectorUid(int uid);
	int GetProjectorUid(void) const;

public:
	Investigator::Ptr _investigator;

	int _projectorUid;
};

#endif