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
	typedef std::map<int, Cell::Ptr> UidMap;

public:
	CellsNetwork(void);

	void InitAndLink(const Cells &cells);
	void UpdateToTime(Utils::GameTime time);

	void RelinkCells(Cell::WeakPtr newParent, Cell::WeakPtr child);

	void AddCell(Cell::Ptr cell);
	void RemoveCell(Cell::WeakPtr cell);

	Cell::Ptr GetCellByUid(int uid) const;

	const Cells& GetActiveCells(void) const;
	const Cells& GetOfflineCells(void) const;
	Cell::WeakPtr GetRootCell() const;
	void SetRootCell(Cell::Ptr cell);

private:
	bool IsConnectedWithRoot(Cell::WeakPtr cell) const;

private:
	Cells _cells;

	UidMap _uidMapCast;

	Cell::Ptr _rootCell;
};

#endif
