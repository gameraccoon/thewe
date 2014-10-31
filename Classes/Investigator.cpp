#include "Investigator.h"

#include "World.h"
#include "GameInfo.h"
#include "MessageManager.h"

Investigator::Investigator(Cell::WeakPtr investigationRoot)
	: _investigationRoot(investigationRoot)
	, _catchTimeBegin(1)
	, _catchTimeEnd(1)
	, _state(State::START_CATCH_DELAY)
	, _uid(World::Instance().GetNewUid())
{
}

Investigator::Investigator(Cell::WeakPtr investigationRoot, const Investigator::Branches &branches)
	: _activeBranches(branches)
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

Investigator::Ptr Investigator::Create(Cell::WeakPtr investigationRoot, const Investigator::Branches &branches)
{
	return std::make_shared<Investigator>(investigationRoot, branches);
}

void Investigator::InitInvestigator(const Investigator::Branches &branches)
{
	_activeBranches = branches;
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
	branchToParent.parentBranch = nullptr;
	branchToParent.timeDuration = GameInfo::Instance().GetFloat("INVESTIGATION_DURATION");
	branchToParent.timeBegin = Utils::GetGameTime();
	branchToParent.timeEnd = branchToParent.timeBegin + branchToParent.timeDuration;
	branchToParent.progressPercentage = 0.0f;
	_activeBranches.push_back(branchToParent);

	for (Cell::Ptr child : cell->GetChildren())
	{
		Investigator::Branch branch;
		branch.cellFrom = cell.get();
		branch.cellTo = child.get();
		branch.parentBranch = nullptr;
		branch.timeDuration = GameInfo::Instance().GetFloat("INVESTIGATION_DURATION");
		branch.timeBegin = Utils::GetGameTime();
		branch.timeEnd = branch.timeBegin + branch.timeDuration;
		branch.progressPercentage = 0.0f;

		_activeBranches.push_back(branch);
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
	if (IsStateType(State::START_CATCH_DELAY))
	{
		float allTime = _catchTimeEnd - _catchTimeBegin;
		float eta = _catchTimeEnd - time;
		float progress = 1.0f - eta / allTime;
		
		if (progress >= 1.0f)
		{
			BeginInvestigation();
			MessageManager::Instance().PutMessage(Message("SaveGame", 0));
		}
	}
	else if (IsStateType(State::INVESTIGATION))
	{
		for (std::size_t index = 0; index < _activeBranches.size(); ++index)
		{
			Branch &branch = _activeBranches[index];

			float allTime = branch.timeEnd - branch.timeBegin;
			float eta = branch.timeEnd - time;
			branch.progressPercentage = 1.0f - eta / allTime;
			branch.progressPercentage *= 100.0f;

			if (branch.progressPercentage > 100.0f) {
				branch.progressPercentage = 100.0f;
			}

			if (branch.progressPercentage >= 100.0f && branch.cellTo->GetInfo().state != Cell::State::ARRESTED)
			{
				if (branch.cellTo == World::Instance().GetCellsNetwork().GetRootCell().lock().get())
				{
					// We are in the root cell. This is GameOver condition
					World::Instance().SetGameOver();
					break;
				}
				else
				{
					Cell::Ptr c = World::Instance().GetCellsNetwork().GetCellByUid(branch.cellFrom->GetUid());
					World::Instance().GetCellsNetwork().RemoveCell(c);
					MessageManager::Instance().PutMessage(Message("DeleteCellWidget", branch.cellFrom->GetUid()));
					CaptureCell(branch.cellTo, branch.cellFrom);

					_activeBranches.erase(_activeBranches.begin() + index);
				}

				MessageManager::Instance().PutMessage(Message("SaveGame", 0));
			}
		}
	}
	else if (IsStateType(State::ABORTED))
	{
		// we dont need this implementation
	}
}

void Investigator::CaptureCell(Cell *cellTarget, Cell *cellFrom)
{
	cellTarget->GetInfo().state = Cell::State::ARRESTED;

	// trying to add bratch to parent cell
	if (cellTarget->GetInfo().parent != cellFrom)
	{
		Investigator::Branch childBranch;
		childBranch.cellFrom = cellTarget;
		childBranch.cellTo = cellTarget->GetInfo().parent;
		childBranch.parentBranch = nullptr;
		childBranch.timeDuration = GameInfo::Instance().GetFloat("INVESTIGATION_DURATION");
		childBranch.timeBegin = Utils::GetGameTime();
		childBranch.timeEnd = childBranch.timeBegin + childBranch.timeDuration;
		childBranch.progressPercentage = 0.0f;

		_activeBranches.push_back(childBranch);
	}

	// add branches to all of the children cells
	for (Cell::Ptr child : cellTarget->GetChildren())
	{
		// disallow to move in reverse direction
		if (child.get() == cellFrom)
		{
			continue;
		}

		Investigator::Branch childBranch;
		childBranch.cellFrom = cellTarget;
		childBranch.cellTo = child.get();
		childBranch.parentBranch = nullptr;
		childBranch.timeDuration = GameInfo::Instance().GetFloat("INVESTIGATION_DURATION");
		childBranch.timeBegin = Utils::GetGameTime();
		childBranch.timeEnd = childBranch.timeBegin + childBranch.timeDuration;
		childBranch.progressPercentage = 0.0f;

		_activeBranches.push_back(childBranch);
	}
}

bool Investigator::IsStateType(Investigator::State state) const
{
	return _state == state;
}

bool Investigator::IsCellUnderInvestigation(Cell::Ptr cell) const
{
	for (const Branch &branch : _activeBranches) {
		if (branch.progressPercentage < 100.0f && branch.cellTo == cell.get()) {
			return true;
		}
	}

	return false;
}

void Investigator::CancelInvestigationTo(Cell::Ptr cell)
{
	for (Branches::iterator it = _activeBranches.begin(); it != _activeBranches.end(); ++it)
	{
		Branch &branch = (*it);
		if (branch.cellTo == cell.get())
		{
			it = _activeBranches.erase(it);
			break;
		}
	}
}

Cell::Ptr Investigator::GetInvestigationRoot(void) const
{
	return _investigationRoot.lock();
}

const Investigator::Branches& Investigator::GetBranches(void) const
{
	return _activeBranches;
}

Investigator::State Investigator::GetState(void) const
{
	return _state;
}

int Investigator::GetUid(void) const
{
	return _uid;
}
