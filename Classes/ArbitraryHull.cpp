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

void ArbitraryHull::Draw(const Color &color) const
{
	std::vector<cocos2d::Vec2> points;
	for (auto& point : _points)
	{
		points.push_back(point);
	}

	if (_points.empty())
	{
		return;
	}

	cocos2d::ccDrawColor4F(color.r, color.g, color.b, color.a);
	cocos2d::ccDrawPoly(&(*points.begin()), points.size(), true);
}

void ArbitraryHull::Draw(void) const
{
	Color white(0xFFFFFFFF);
	Draw(white);
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
