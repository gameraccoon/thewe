#include "CellsNetwork.h"

#include "Town.h"

#include "MessageManager.h"

#include "Log.h"

CellsNetwork::CellsNetwork(void)
{
}

void CellsNetwork::InitAndLink(const CellsNetwork::Cells &cells)
{
}

void CellsNetwork::UpdateToTime(Utils::GameTime time)
{
	for (auto cell : _cells)
	{	
		if (!IsConnectedWithRoot(cell) && cell->IsState(Cell::State::READY)) {
			cell->BeginAutonomy();
		}

		if (IsConnectedWithRoot(cell) && cell->IsState(Cell::State::AUTONOMY)) {
			cell->GetInfo().state = Cell::State::READY;
			cell->GetInfo().stateBegin = 0;
			cell->GetInfo().stateDuration = 0;
		}
	}
}

void CellsNetwork::RelinkCells(Cell::WeakPtr newParent, Cell::WeakPtr child)
{
	Cell::Ptr childPtr = child.lock();
	Cell::Ptr prevParent = childPtr->GetInfo().parent.lock();
	if (prevParent) {
		prevParent->RemoveChild(child);
	}

	Cell::Ptr newParentPtr = newParent.lock();
	newParentPtr->AddChild(child);
}

void CellsNetwork::AddCell(Cell::Ptr cell)
{
	for (auto existedCell : _cells)
	{
		if (cell == existedCell)
		{
			Log::Instance().writeWarning("Trying to add duplicate cell");
			return;
		}
	}

	_cells.push_back(cell);
	_uidMapCast.insert(std::pair<int, Cell::Ptr>(cell->GetUid(), cell));
}

void CellsNetwork::RemoveCell(Cell::WeakPtr cell)
{
	Cell::Ptr cellPtr = cell.lock();
	for (Cells::iterator it = _cells.begin(); it != _cells.end(); ++it)
	{
		if ((*it) == cellPtr)
		{
			cellPtr->GetInfo().town.lock()->SetCellPresented(false);

			Cell::Ptr parent = cellPtr->GetInfo().parent.lock();
			if (parent) {
				parent->RemoveChild(cell);
			}

			cellPtr->RemoveAllChildren();

			it = _cells.erase(it);
			
			UidMap::const_iterator uidIter;
			uidIter = _uidMapCast.find(cellPtr->GetUid());
			_uidMapCast.erase(uidIter);

			break;
		}
	}
}

Cell::Ptr CellsNetwork::GetCellByUid(int uid) const
{
	std::map<int, Cell::Ptr>::const_iterator iter;
	iter = _uidMapCast.find(uid);

	if (iter != _uidMapCast.end()) {
		return iter->second;
	}
		
	return Cell::Ptr();
}

Cell::WeakPtr CellsNetwork::GetRootCell() const
{
	return _rootCell;
}

void CellsNetwork::SetRootCell(Cell::Ptr cell)
{
	_rootCell.swap(cell);
}

const CellsNetwork::Cells& CellsNetwork::GetActiveCells() const
{
	return _cells;
}

const CellsNetwork::Cells& CellsNetwork::GetOfflineCells() const
{
	return _cells;
}

bool CellsNetwork::IsCellRelinkable(Cell::WeakPtr cell) const
{
	if (!cell.expired())
	{
		Cell::Ptr cellPtr = cell.lock();

		if (!cellPtr->IsState(Cell::State::AUTONOMY)) {
			return false;
		}

		Cell::Ptr parentPtr = cellPtr->GetInfo().parent.lock();
		return parentPtr == nullptr && cellPtr != _rootCell;
	}

	return false;
}

bool CellsNetwork::IsConnectedWithRoot(Cell::WeakPtr cell) const
{
	Cell::Ptr rootCell = GetRootCell().lock();
	Cell::Ptr cellPtr = cell.lock();

	Cell::Ptr parent = cellPtr->GetInfo().parent.lock();
	if (!parent) {
		return cellPtr == rootCell;
	}

	while (parent)
	{
		Cell::Ptr current = parent;
		parent = current->GetParent().lock();
		if (!parent) {
			return current == rootCell;
		}
	}

	return false;
}
