#include "Investigator.h"

#include "World.h"
#include "GameInfo.h"

Investigator::Investigator(Cell::WeakPtr investigationRoot)
	: _investigationRoot(investigationRoot)
	, _catchTimeBegin(1)
	, _catchTimeEnd(1)
	, _state(State::START_CATCH_DELAY)
	, _uid(World::Instance().GetNewUid())
{
}

Investigator::Investigator(Cell::WeakPtr investigationRoot, const Investigator::BranchBundle &rootBranchBudle)
	: _branchRoot(rootBranchBudle)
	, _investigationRoot(investigationRoot)
	, _catchTimeBegin(1)
	, _catchTimeEnd(1)
	, _state(State::START_CATCH_DELAY)
	, _uid(World::Instance().GetNewUid())
{
}

Investigator::Ptr Investigator::Create(Cell::WeakPtr investigationRoot)
{
	return std::make_shared<Investigator>(investigationRoot);
}

Investigator::Ptr Investigator::Create(Cell::WeakPtr investigationRoot, const Investigator::BranchBundle &rootBranchBudle)
{
	return std::make_shared<Investigator>(investigationRoot, rootBranchBudle);
}

void Investigator::InitInvestigator(const Investigator::BranchBundle &rootBranchBudle)
{
	_branchRoot = rootBranchBudle;
}

void Investigator::BeginCatchTime(float time)
{
	_state = State::START_CATCH_DELAY;
	_catchTimeBegin = Utils::GetGameTime();
	_catchTimeEnd = _catchTimeBegin + (Utils::GameTime)time;
}

void Investigator::BeginInvestigation(void)
{
	if (World::Instance().GetTutorialState() == "WaitForUncatchedInvestigator")
	{
		World::Instance().RunTutorialFunction("FirstUncatchedInvestigator");
	}

	Cell::Ptr cell = _investigationRoot.lock();
	cell->GetInfo().state = Cell::State::ARRESTED;
	_state = State::INVESTIGATION;

	if (cell == World::Instance().GetCellsNetwork().GetRootCell().lock())
	{
		// set game over state to World
		World::Instance().SetGameOver();
		return;
	}

	Investigator::Branch branchToParent;
	branchToParent.cellFrom = cell.get();
	branchToParent.cellTo = cell->GetInfo().parent;
	branchToParent.parentBrunch = nullptr;
	branchToParent.timeDuration = GameInfo::Instance().GetFloat("INVESTIGATION_DURATION");
	branchToParent.timeBegin = Utils::GetGameTime();
	branchToParent.timeEnd = branchToParent.timeBegin + branchToParent.timeDuration;
	branchToParent.progressPercentage = 0.0f;
	_branchRoot.push_back(branchToParent);

	for (Cell::Ptr child : cell->GetChildren())
	{
		Investigator::Branch branch;
		branch.cellFrom = cell.get();
		branch.cellTo = child.get();
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
	_state = State::ABORTED;
}

void Investigator::StayInvestigation(bool stay)
{
	_state = State::STAYING;
}

void Investigator::UpdateToTime(Utils::GameTime time)
{
	UpdateBranchesRecurcively(_branchRoot, time);
}

void Investigator::UpdateBranchesRecurcively(Investigator::BranchBundle &bundle, Utils::GameTime time)
{
	if (IsStateType(State::START_CATCH_DELAY))
	{
		float allTime = _catchTimeEnd - _catchTimeBegin;
		float eta = _catchTimeEnd - time;
		float progress = 1.0f - eta / allTime;
		
		if (progress >= 1.0f)
		{
			BeginInvestigation();
		}
	}
	else if (IsStateType(State::INVESTIGATION))
	{
		for (Investigator::Branch &branch : bundle)
		{
			if (branch.progressPercentage < 100.0f)
			{
				float allTime = branch.timeEnd - branch.timeBegin;
				float eta = branch.timeEnd - time;
				branch.progressPercentage = 1.0f - eta / allTime;
				branch.progressPercentage *= 100.0f;

				if (branch.progressPercentage > 100.0f) {
					branch.progressPercentage = 100.0f;
				}
			}
			else if (branch.cellTo->GetInfo().state != Cell::State::ARRESTED)
			{
				if (branch.cellTo == World::Instance().GetCellsNetwork().GetRootCell().lock().get())
				{
					// We are in the root cell. This is GameOver condition
					World::Instance().SetGameOver();
				}
				else
				{
					branch.cellTo->GetInfo().state = Cell::State::ARRESTED;

					// trying to add bratch to parent cell
					if (branch.cellTo->GetInfo().parent != branch.cellFrom)
					{
						Investigator::Branch childBranch;
						childBranch.cellFrom = branch.cellTo;
						childBranch.cellTo = branch.cellTo->GetInfo().parent;
						childBranch.parentBrunch = nullptr;
						childBranch.timeDuration = GameInfo::Instance().GetFloat("INVESTIGATION_DURATION");
						childBranch.timeBegin = Utils::GetGameTime();
						childBranch.timeEnd = childBranch.timeBegin + childBranch.timeDuration;
						childBranch.progressPercentage = 0.0f;

						branch.childBrunches.push_back(childBranch);
					}

					// add branches to all of the children cells
					for (Cell::Ptr child : branch.cellTo->GetChildren())
					{
						// disallow to move in reverse direction
						if (child.get() == branch.cellFrom)
						{
							continue;
						}

						Investigator::Branch childBranch;
						childBranch.cellFrom = branch.cellTo;
						childBranch.cellTo = child.get();
						childBranch.parentBrunch = nullptr;
						childBranch.timeDuration = GameInfo::Instance().GetFloat("INVESTIGATION_DURATION");
						childBranch.timeBegin = Utils::GetGameTime();
						childBranch.timeEnd = childBranch.timeBegin + childBranch.timeDuration;
						childBranch.progressPercentage = 0.0f;

						branch.childBrunches.push_back(childBranch);
					}
				}
			}

			UpdateBranchesRecurcively(branch.childBrunches, time);
		}
	}
	else if (IsStateType(State::ABORTED))
	{
		// we dont need this implementation yet
	}
}

bool Investigator::IsStateType(Investigator::State state) const
{
	return _state == state;
}

bool Investigator::IsCellUnderInvestigation(Cell::Ptr cell, const BranchBundle &bundle) const
{
	for (const Branch &branch : bundle)
	{
		if (branch.progressPercentage < 100.0f && branch.cellTo == cell.get())
		{
			return true;
		}
		else
		{
			return IsCellUnderInvestigation(cell, branch.childBrunches);
		}
	}

	return false;
}

void Investigator::CancleInvestigationTo(Cell::Ptr cell, BranchBundle &bundle)
{
	for (BranchBundle::iterator it = bundle.begin(); it != bundle.end(); ++it)
	{
		Branch &branch = (*it);
		if (branch.cellTo == cell.get())
		{
			it = bundle.erase(it);
			break;
		}
		else
		{
			CancleInvestigationTo(cell, branch.childBrunches);
		}
	}
}

Cell::Ptr Investigator::GetInvestigationRoot(void) const
{
	return _investigationRoot.lock();
}

Investigator::BranchBundle& Investigator::GetRootBranchBundle(void)
{
	return _branchRoot;
}

Investigator::State Investigator::GetState(void) const
{
	return _state;
}

int Investigator::GetUid(void) const
{
	return _uid;
}
