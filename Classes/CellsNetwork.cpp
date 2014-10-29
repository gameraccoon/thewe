#include "CellsNetwork.h"

#include "Town.h"

#include "MessageManager.h"

CellsNetwork::CellsNetwork(void)
{
}

void CellsNetwork::InitAndLink(const CellsNetwork::Cells &cells)
{
}

void CellsNetwork::UpdateToTime(Utils::GameTime time)
{
	for (CellsIter it = _cells.begin(); it != _cells.end(); ++it)
	{
		Cell::Ptr cell = (*it);		
		if (!IsConnectedWithRoot(cell) && cell->IsState(Cell::State::READY)) {
			cell->BeginAutonomy();
		}
	}
}

void CellsNetwork::RelinkCells(Cell::Ptr parent, Cell::Ptr child)
{
	parent->AddChild(child);
}

void CellsNetwork::AppendCell(Cell::Ptr cell)
{
	for (CellsIter it = _cells.begin(); it != _cells.end(); ++it) {
		if ((*it) == cell) {
			return;
		}
	}

	_cells.push_back(cell);
	_uidMapCast.insert(std::pair<int, Cell::Ptr>(cell->GetUid(), cell));
}

void CellsNetwork::RemoveCell(Cell::Ptr cell)
{	
	for (CellsIter it = _cells.begin(); it != _cells.end(); ++it)
	{
		if ((*it) == cell)
		{
			cell->GetInfo().town.lock()->SetCellPresented(false);

			Cell *parent = cell->GetInfo().parent;
			if (parent) {
				parent->RemoveChild(cell);
			}

			cell->RemoveAllChildren();

			it = _cells.erase(it);
			
			std::map<int, Cell::Ptr>::const_iterator uidIter;
			uidIter = _uidMapCast.find(cell->GetUid());
			_uidMapCast.erase(uidIter);

			break;
		}
	}
}

Cell::Ptr CellsNetwork::GetCellByInfo(const Cell::Info &info)
{
	Cell *parent = info.parent;
	Town::Ptr town = info.town.lock();

	for (Cell::Ptr cell : _cells) {
		if (cell->GetInfo().parent == parent &&
			cell->GetInfo().town.lock() == town)
		{
			return cell;
		}
	}

	return Cell::Ptr();
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

bool CellsNetwork::IsConnectedWithRoot(Cell::Ptr cell) const
{
	Cell* rootCell = GetRootCell().lock().get();

	Cell *parent = cell->GetInfo().parent;
	if (!cell->GetInfo().parent) {
		return cell.get() == rootCell;
	}

	while (parent)
	{
		Cell *next = parent->GetInfo().parent;
		if (!next) {
			return parent == rootCell;
		}

		parent = next;
	}

	return false;
}
