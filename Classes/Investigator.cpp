#include "Investigator.h"

#include "World.h"
#include "GameInfo.h"

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
	Cell::Ptr cell = _investigationRoot.lock();
	cell->GetInfo().state = Cell::ARRESTED;

	if (cell->GetInfo().parent == nullptr)
	{
		// set game over state to World
		return;
	}

	Investigator::Branch branchToParent;
	branchToParent.cellFrom = cell;
	branchToParent.cellTo = (Cell::Ptr)cell->GetInfo().parent;
	branchToParent.parentBrunch = nullptr;
	branchToParent.timeDuration = GameInfo::Instance().GetFloat("INVESTIGATION_DURATION");
	branchToParent.timeBegin = Utils::GetGameTime();
	branchToParent.timeEnd = branchToParent.timeBegin + branchToParent.timeDuration;
	branchToParent.progressPercentage = 0.0f;
	_branchRoot.push_back(branchToParent);

	for (Cell::Ptr child : cell->GetChildren())
	{
		Investigator::Branch branch;
		branch.cellFrom = cell;
		branch.cellTo = child;
		branch.parentBrunch = nullptr;
		branch.timeDuration = GameInfo::Instance().GetFloat("INVESTIGATION_DURATION");
		branch.timeBegin = Utils::GetGameTime();
		branch.timeEnd = branch.timeBegin + branch.timeDuration;
		branch.progressPercentage = 0.0f;

		_branchRoot.push_back(branch);
	}
}

void Investigator::AbortInvestigation(void)
{
}

void Investigator::StayInvestigation(bool stay)
{
}

void Investigator::UpdateToTime(Utils::GameTime time)
{
	for (Investigator::Branch &branch : _branchRoot)
	{
		if (branch.progressPercentage < 100.0f)
		{
			float allTime = branch.timeEnd - branch.timeBegin;
			float eta = branch.timeEnd - time;
			branch.progressPercentage = 1.0f - eta / allTime;
			branch.progressPercentage *= 100.0f;
		}
		else if (branch.cellTo->GetInfo().state != Cell::ARRESTED)
		{
			branch.cellTo->GetInfo().state = Cell::ARRESTED;
		}

		UpdateBranchRecurcively(branch.childBrunches);
	}
}

void Investigator::UpdateBranchRecurcively(Investigator::BranchBundle &branch)
{
}

Cell::Ptr Investigator::GetInvestigationRoot(void) const
{
	return _investigationRoot.lock();
}

Investigator::BranchBundle Investigator::GetRootBranchBundle(void)
{
	return _branchRoot;
}

int Investigator::GetUid(void) const
{
	return _uid;
}