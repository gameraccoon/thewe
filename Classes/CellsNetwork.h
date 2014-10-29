#ifndef CELLS_NETWORK_H
#define CELLS_NETWORK_H

#include <vector>
#include <map>

#include "Cell.h"
#include "MiscUtils.h"

class CellsNetwork
{
public:
	typedef std::vector<Cell::Ptr> Cells;
	typedef Cells::iterator CellsIter;
	typedef Cells::const_iterator CellsCIter;

public:
	CellsNetwork(void);

	void InitAndLink(const Cells &cells);
	void UpdateToTime(Utils::GameTime time);

	void RelinkCells(Cell::Ptr parent, Cell::Ptr child);

	void AppendCell(Cell::Ptr cell);
	void RemoveCell(Cell::Ptr cell);

	Cell::Ptr GetCellByInfo(const Cell::Info &info);
	Cell::Ptr GetCellByUid(int uid) const;

	const Cells& GetActiveCells(void) const;
	const Cells& GetOfflineCells(void) const;
	Cell::WeakPtr GetRootCell() const;
	void SetRootCell(Cell::Ptr cell);

private:
	bool IsConnectedWithRoot(Cell::Ptr cell) const;

private:
	Cells _cells;

	std::map<int, Cell::Ptr> _uidMapCast;

	Cell::Ptr _rootCell;
};

#endif
