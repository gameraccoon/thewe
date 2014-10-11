#include "Investigator.h"

Investigator::Investigator(Cell::WeakPtr investigationRoot)
	: _investigationRoot(investigationRoot)
{
}

Investigator::Ptr Investigator::Create(Cell::WeakPtr investigationRoot)
{
	return std::make_shared<Investigator>(investigationRoot);
}

void Investigator::BeginInvestigation(void)
{
}

void Investigator::AbortInvestigation(void)
{
}

void Investigator::StayInvestigation(bool stay)
{
}

void Investigator::UpdateToTime(Utils::GameTime time)
{
}

Cell::Ptr Investigator::GetInvestigationRoot(void) const
{
	return _investigationRoot.lock();
}

Investigator::BrunchBundle Investigator::GetRootBrunchBundle(void)
{
	return _brunchRoot;
}