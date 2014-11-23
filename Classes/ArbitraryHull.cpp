#include "ArbitraryHull.h"

#include <cocos2d.h>

struct ArbitaryHullImp
{
	std::vector<cocos2d::Vec2> pointsArray;
};

ArbitraryHull::ArbitraryHull()
{
}

ArbitraryHull::~ArbitraryHull()
{
}

void ArbitraryHull::PushPoint(const Vector2 &point)
{
	_points.push_back(point);
}

void ArbitraryHull::PopPoint(void)
{
	if (!_points.empty())
	{
		_points.erase(_points.end() - 1);
	}
}

void ArbitraryHull::Clear(void)
{
	_points.clear();
}

int ArbitraryHull::GetPointsNum(void) const
{
	return _points.size();
}

const std::vector<Vector2>& ArbitraryHull::GetPoints(void) const
{
	return _points;
}

bool ArbitraryHull::Contain(const Vector2 &point) const
{
	unsigned int i, j;
	bool result = false;

	for (i = 0, j = _points.size() - 1; i < _points.size(); j = i++)
	{
		if ((_points[i].y > point.y) != (_points[j].y > point.y) &&
			(point.x < (_points[j].x - _points[i].x) * (point.y - _points[i].y)
			/ (_points[j].y-_points[i].y) + _points[i].x))
		{
			result = !result;
		}
	}

	return result;
}
