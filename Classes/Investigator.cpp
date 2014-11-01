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

Investigator::Investigator(unsigned int uid)
	: _catchTimeBegin(1)
	, _catchTimeEnd(1)
	, _state(State::START_CATCH_DELAY)
	, _uid(uid)
{

}

Investigator::Ptr Investigator::Create(Cell::WeakPtr investigationRoot)
{
	return std::make_shared<Investigator>(investigationRoot);
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
	branchToParent.cellFrom = cell;
	branchToParent.cellTo = cell->GetInfo().parent;
	branchToParent.timeDuration = GameInfo::Instance().GetFloat("INVESTIGATION_DURATION");
	branchToParent.timeBegin = Utils::GetGameTime();
	_activeBranches.push_back(branchToParent);

	for (Cell::WeakPtr child : cell->GetChildren())
	{
		Investigator::Branch branch;
		branch.cellFrom = cell;
		branch.cellTo = child;
		branch.timeDuration = GameInfo::Instance().GetFloat("INVESTIGATION_DURATION");
		branch.timeBegin = Utils::GetGameTime();

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

			if (time >= (branch.timeBegin + branch.timeDuration) && branch.cellTo.lock()->GetInfo().state != Cell::State::ARRESTED)
			{
				if (branch.cellTo.lock() == World::Instance().GetCellsNetwork().GetRootCell().lock())
				{
					// We are in the root cell. This is GameOver condition
					World::Instance().SetGameOver();
					break;
				}
				else
				{
					Cell::Ptr cell = branch.cellFrom.lock();
					World::Instance().GetCellsNetwork().RemoveCell(cell);
					MessageManager::Instance().PutMessage(Message("DeleteCellWidget", branch.cellFrom.lock()->GetUid()));
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

void Investigator::CaptureCell(Cell::WeakPtr cellTarget, Cell::WeakPtr cellFrom)
{
	Cell::Ptr cellTargetPtr = cellTarget.lock();
	cellTargetPtr->GetInfo().state = Cell::State::ARRESTED;

	// trying to add bratch to parent cell
	if (cellTargetPtr->GetInfo().parent.lock() != cellFrom.lock())
	{
		Investigator::Branch childBranch;
		childBranch.cellFrom = cellTargetPtr;
		childBranch.cellTo = cellTargetPtr->GetInfo().parent;
		childBranch.timeDuration = GameInfo::Instance().GetFloat("INVESTIGATION_DURATION");
		childBranch.timeBegin = Utils::GetGameTime();

		_activeBranches.push_back(childBranch);
	}

	// add branches to all of the children cells
	for (Cell::WeakPtr child : cellTargetPtr->GetChildren())
	{
		// disallow to move in reverse direction
		if (child.lock() == cellFrom.lock())
		{
			continue;
		}

		Investigator::Branch childBranch;
		childBranch.cellFrom = cellTargetPtr;
		childBranch.cellTo = child;
		childBranch.timeDuration = GameInfo::Instance().GetFloat("INVESTIGATION_DURATION");
		childBranch.timeBegin = Utils::GetGameTime();

		_activeBranches.push_back(childBranch);
	}
}

bool Investigator::IsStateType(Investigator::State state) const
{
	return _state == state;
}

bool Investigator::IsCellUnderInvestigation(Cell::WeakPtr cell) const
{
	for (const Branch &branch : _activeBranches) {
		if (branch.cellTo.lock() == cell.lock()) {
			return true;
		}
	}

	return false;
}

void Investigator::CancelInvestigationTo(Cell::WeakPtr cell)
{
	for (Branches::iterator it = _activeBranches.begin(); it != _activeBranches.end(); ++it)
	{
		Branch &branch = (*it);
		if (branch.cellTo.lock() == cell.lock())
		{
			it = _activeBranches.erase(it);
			break;
		}
	}
}

Cell::WeakPtr Investigator::GetInvestigationRoot(void) const
{
	return _investigationRoot;
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
