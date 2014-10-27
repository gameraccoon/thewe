#include "CellsNetwork.h"

#include "Town.h"

CellsNetwork::CellsNetwork(void)
{
}

void CellsNetwork::InitAndLink(const CellsNetwork::CellsList &list)
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

Cell::Ptr CellsNetwork::GetRootCell(void) const
{
	for (CellsCIter it = _cells.begin(); it != _cells.end(); ++it) {
		if ((*it)->IsRoot()) {
			return (*it);
		}
	}

	return Cell::Ptr();
}

const CellsNetwork::CellsList& CellsNetwork::GetActiveCellsList(void) const
{
	return _cells;
}

const CellsNetwork::CellsList& CellsNetwork::GetOfflineCellsList(void) const
{
	return _cells;
}

bool CellsNetwork::IsConnectedWithRoot(Cell::Ptr cell) const
{
	Cell *parent = cell->GetInfo().parent;
	if (!cell->GetInfo().parent) {
		return cell->IsRoot();
	}

	while (parent)
	{
		Cell *next = parent->GetInfo().parent;
		if (!next) {
			return parent->IsRoot();
		}

		parent = next;
	}

	return false;
}
