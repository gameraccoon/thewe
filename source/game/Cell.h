#ifndef CELL_H
#define CELL_H

#include <memory>

#include "Region.h"
#include "Point.h"

class Cell
{
public:
	struct Info
	{
		Cell *parent;
		Region::Ptr region;
		Point location;

		float cash;
		float morale;
		float contentment;

		int membersNum;
	private:

	};

	typedef std::shared_ptr<Cell> Ptr;

public:
	Cell(const Info &info);
	
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
	const std::vector<Cell::Ptr>& GetChildren() const;

	/**
	* ¬озвращает информацию о €чейке
	*/
	const Info& GetInfo(void) const;
	
	void Update(float deltatime);

private:
	/** ”становить €чейке родител€ */
	void _SetParent(Cell* cell);

	/** ѕересчитать наличные со временем */
	void _UpdateCash(float deltatime);
	/** ѕересчитать удовлетворЄнность со временем */
	void _UpdateMorale(float deltatime);
	/** ѕересчитать удовлетворЄнность со временем */
	void _UpdateContentment(float deltatime);

	/** ¬с€ важна€ информаци€ о €чейке */
	Info _info;

	/** ƒочерние €чейки */
	std::vector<Cell::Ptr> _childCells;

	/** √еографическое расположение €чейки на карте */
	Point _worldLocation;
};

#endif // CELL_H