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

private:
	Cells _cells;

	std::map<int, Cell::Ptr> _uidMapCast;

	bool IsConnectedWithRoot(Cell::Ptr cell) const;

public:
	CellsNetwork(void);

	void InitAndLink(const Cells &cells);
	void UpdateToTime(Utils::GameTime time);

	void AppendCell(Cell::Ptr cell);
	void RemoveCell(Cell::Ptr cell);

	Cell::Ptr GetCellByInfo(const Cell::Info &info);
	Cell::Ptr GetCellByUid(int uid) const;
	Cell::Ptr GetRootCell(void) const;

	const Cells& GetActiveCells(void) const;
	const Cells& GetOfflineCells(void) const;
};

#endif
