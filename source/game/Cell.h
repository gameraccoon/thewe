#ifndef CELL_H
#define CELL_H

#include <memory>

#include "Region.h"
#include "Point.h"

class Cell
{
public:
	typedef std::shared_ptr<Cell> Ptr;
public:
	Cell(Point location);

	int GetMembersCount() const;
	float GetCash() const;
	float GetMoralValue() const;
	float GetContentment() const;

	Point GetLocation();
	
	Cell* GetParent() const;
	
	/**
	 * ƒобавл€ет €чейке новую дочернюю €чейку
	 * ячейка сразу прописываетс€ как родительска€ к дочерней €чейке
	 */
	void AddChild(Cell::Ptr cell);

	/**
	 * ”дал€ет дочернюю €чейку
	 * —тавит в nullptr значение родительской €чейки у дочерней
	 */
	void RemoveChild(Cell::Ptr cell);

	/** ¬озвращает все дочерние €чейки */
	const std::vector<Cell::Ptr>& GetChildren();
private:
	/** ”становить €чейке родител€ */
	void _SetParent(Cell* cell);
	/**  оличество членов €чейки */
	int _membersCount;
	/** ƒенежные средства */
	float _cash;
	/** ”ровень морали */
	float _moralValue;
	/** ”довлетворЄнность */
	float _contentment;

	/** –егион, в котором находитс€ €чейка */
	Region::Ptr _region;

	/** –одительска€ €чейка (может отсутствовать значение) */
	Cell *_parentCell;

	/** ƒочерние €чейки */
	std::vector<Cell::Ptr> _childCells;

	/** √еографическое расположение €чейки на карте */
	Point _worldLocation;
};

#endif // CELL_H