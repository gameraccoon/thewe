#include "InvestigatorMapWidget.h"

InvestigatorMapWidget::InvestigatorMapWidget(Investigator::Ptr investigator)
	: _investigator(investigator)
	, _projectorUid(-1)
{
}

bool InvestigatorMapWidget::init(void)
{
	scheduleUpdate();

	return true;
}

void InvestigatorMapWidget::update(float dt)
{
}

Investigator::Ptr InvestigatorMapWidget::GetInvestigator(void) const
{
	return _investigator;
}

void InvestigatorMapWidget::SetProjectorUid(int uid)
{
	_projectorUid = uid;
}

int InvestigatorMapWidget::GetProjectorUid(void) const
{
	return _projectorUid;
}