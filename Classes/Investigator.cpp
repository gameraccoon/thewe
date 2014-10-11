#include "Investigator.h"

#include "World.h"

Investigator::Investigator(Cell::WeakPtr investigationRoot)
	: _investigationRoot(investigationRoot)
	, _uid(World::Instance().GetNewUid())
{
}

Investigator::Ptr Investigator::Create(Cell::WeakPtr investigationRoot)
{
	return std::make_shared<Investigator>(investigationRoot);
}

void Investigator::BeginInvestigation(void)
{
	_investigationRoot.lock()->GetInfo().state = Cell::ARRESTED;

	// add investigation branches here
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

int Investigator::GetUid(void) const
{
	return _uid;
}