#include "Investigator.h"

#include "World.h"
#include "GameInfo.h"
#include "MessageManager.h"
#include "Log.h"

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

void Investigator::BeginCatchTime(void)
{
	_state = State::START_CATCH_DELAY;
}

void Investigator::BeginInvestigation(void)
{
	if (World::Instance().GetTutorialManager().IsTutorialStateAvailable("WaitForUncatchedInvestigator"))
	{
		World::Instance().GetTutorialManager().RunTutorialFunction("FirstUncatchedInvestigator");
	}

	Cell::Ptr cell = _investigationRoot.lock();
	cell->GetInfo().state = Cell::State::ARRESTED;
	_state = State::INVESTIGATION;

	Investigator::Branch branchToParent;
	branchToParent.cellFrom = cell;
	branchToParent.cellTo = cell->GetInfo().parent;
	branchToParent.timeDuration = GameInfo::Instance().GetTime("INVESTIGATION_DURATION");
	branchToParent.timeBegin = Utils::GetGameTime();
	_activeBranches.push_back(branchToParent);

	for (Cell::WeakPtr child : cell->GetChildren())
	{
		Investigator::Branch branch;
		branch.cellFrom = cell;
		branch.cellTo = child;
		branch.timeDuration = GameInfo::Instance().GetTime("INVESTIGATION_DURATION");
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
	if (IsStateType(State::INVESTIGATION))
	{
		for (std::size_t index = 0; index < _activeBranches.size(); ++index)
		{
			Branch &branch = _activeBranches[index];

			if (branch.cellTo.expired())
			{
				if (GetCountOfCellUsageInBranches(branch.cellFrom) <= 1)
				{
					Message message("DeleteCellWidget");
					message.variables.SetInt("UID", branch.cellFrom.lock()->GetUid());
					MessageManager::Instance().PutMessage(message);

					World::Instance().GetCellsNetwork().RemoveCell(branch.cellFrom);
				}

				_activeBranches.erase(_activeBranches.begin() + index);
				continue;
			}

			if (time >= (branch.timeBegin + branch.timeDuration) && branch.cellTo.lock()->GetInfo().state != Cell::State::ARRESTED)
			{
				if (GetCountOfCellUsageInBranches(branch.cellFrom) <= 1)
				{					
					Message message("DeleteCellWidget");
					message.variables.SetInt("UID", branch.cellFrom.lock()->GetUid());
					MessageManager::Instance().PutMessage(message);

					MessageManager::Instance().PutMessage(message);
					World::Instance().GetCellsNetwork().RemoveCell(branch.cellFrom);
				}
					
				int createdBranchesCount = CaptureCellAndReturnNewBranchesCount(branch.cellTo, branch.cellFrom);
				if (createdBranchesCount == 0)
				{
					Message message("DeleteCellWidget");
					message.variables.SetInt("UID", branch.cellFrom.lock()->GetUid());
					MessageManager::Instance().PutMessage(message);

					MessageManager::Instance().PutMessage(message);
					World::Instance().GetCellsNetwork().RemoveCell(branch.cellTo);
				}

				_activeBranches.erase(_activeBranches.begin() + index);

				MessageManager::Instance().PutMessage(Message("SaveGame"));
			}
		}

		if (_activeBranches.empty())
		{
			_state = State::FINISHED;
		}
	}
	else if (IsStateType(State::ABORTED))
	{
		// we dont need this implementation
	}
}

int Investigator::CaptureCellAndReturnNewBranchesCount(Cell::WeakPtr cellTarget, Cell::WeakPtr cellFrom)
{
	int count = 0;

	Cell::Ptr cellTargetPtr = cellTarget.lock();
	cellTargetPtr->GetInfo().state = Cell::State::ARRESTED;

	// trying to add bratch to parent cell
	if (cellTargetPtr->GetInfo().parent.lock() != cellFrom.lock())
	{
		Investigator::Branch childBranch;
		childBranch.cellFrom = cellTargetPtr;
		childBranch.cellTo = cellTargetPtr->GetInfo().parent;
		childBranch.timeDuration = GameInfo::Instance().GetTime("INVESTIGATION_DURATION");
		childBranch.timeBegin = Utils::GetGameTime();

		_activeBranches.push_back(childBranch);
		++count;
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
		childBranch.timeDuration = GameInfo::Instance().GetTime("INVESTIGATION_DURATION");
		childBranch.timeBegin = Utils::GetGameTime();

		_activeBranches.push_back(childBranch);
		++count;
	}

	return count;
}

int Investigator::GetCountOfCellUsageInBranches(Cell::WeakPtr cell) const
{
	const Cell::Ptr cellPtr = cell.lock();
	if (!cellPtr) {
		WRITE_WARN("GetCountOfCellUsageInBranches: dead cell pointer in param.");
		return 0;
	}

	int counter = 0;
	for (const Branch &branch : _activeBranches) {
		if (branch.cellFrom.lock() == cellPtr) {
			++counter;
		}
	}

	return counter;
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

unsigned int Investigator::GetCatchBeginTime() const
{
	return _catchTimeBegin;
}

unsigned int Investigator::GetCatchEndTime() const
{
	return _catchTimeEnd;
}
