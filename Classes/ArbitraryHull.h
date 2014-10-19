#ifndef ARBITRARY_HULL_H
#define ARBITRARY_HULL_H

#include <vector>
#include "Color.h"
#include "Vector2.h"

class ArbitraryHull
{
public:
	ArbitraryHull();
	~ArbitraryHull();

	void PushPoint(const Vector2 &point);
	void PopPoint(void);
	void Clear(void);
	
	int GetPointsNum(void) const;
	const std::vector<Vector2>& GetPoints(void) const;

	bool Contain(const Vector2 &point) const;

private:
	std::vector<Vector2> _points;
};

#endif
