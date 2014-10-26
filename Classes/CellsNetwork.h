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
	CellsList _activeCells;
	CellsList _offlineCells;

	std::map<int, Cell::Ptr> _uidMapCast;

public:
	CellsNetwork(void);

	void InitAndLink(const CellsList &list);
	void UpdateToTime(Utils::GameTime time);

	void AppendCell(Cell::Ptr cell);
	void RemoveCell(Cell::Ptr cell);

	Cell::Ptr GetCellByInfo(const Cell::Info &info);
	Cell::Ptr GetCellByUid(int uid) const;
	Cell::Ptr GetRootCell(void) const;

	bool CheckActiveCellToOffline(Cell::Ptr cell);

	const CellsList& GetActiveCellsList(void) const;
	const CellsList& GetOfflineCellsList(void) const;
};

#endif