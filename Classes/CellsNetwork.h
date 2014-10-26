#ifndef CELLS_NETWORK_H
#define CELLS_NETWORK_H

#include <vector>
#include <map>

#include "Cell.h"
#include "MiscUtils.h"

class CellsNetwork
{
public:
	typedef std::vector<Cell::Ptr> CellsList;
	typedef CellsList::iterator CellsIter;
	typedef CellsList::const_iterator CellsCIter;

private:
	CellsList _cells;

	std::map<int, Cell::Ptr> _uidMapCast;

	bool IsConnectedWithRoot(Cell::Ptr cell) const;

public:
	CellsNetwork(void);

	void InitAndLink(const CellsList &list);
	void UpdateToTime(Utils::GameTime time);

	void AppendCell(Cell::Ptr cell);
	void RemoveCell(Cell::Ptr cell);

	Cell::Ptr GetCellByInfo(const Cell::Info &info);
	Cell::Ptr GetCellByUid(int uid) const;
	Cell::Ptr GetRootCell(void) const;

	const CellsList& GetActiveCellsList(void) const;
	const CellsList& GetOfflineCellsList(void) const;
};

#endif