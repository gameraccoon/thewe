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

private:
	/** Количество членов ячейки */
	int _membersCount;
	/** Денежные средства */
	float _cash;
	/** Уровень морали */
	float _moralValue;
	/** Удовлетворённость */
	float _contentment;

	/** Регион, в котором находится ячейка */
	Region::Ptr _region;

	/** географическое расположение ячейки на карте */
	Point _worldLocation;
};

#endif // CELL_H