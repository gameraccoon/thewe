#include "InvestigatorMapWidget.h"

InvestigatorMapWidget::InvestigatorMapWidget(Investigator::Ptr investigator)
	: _investigator(investigator)
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