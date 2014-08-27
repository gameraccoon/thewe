#include "ArbitraryHull.h"

#include <cocos2d.h>

void ArbitraryHull::PushPoint(const Vector2 &point)
{
	_pointsArray.push_back(point);
}

void ArbitraryHull::PopPoint(void)
{
	if (!_pointsArray.empty())
	{
		_pointsArray.erase(_pointsArray.end() - 1);
	}
}

void ArbitraryHull::Draw(const Color &color) const
{
	if (_pointsArray.empty())
	{
		return;
	}

	cocos2d::ccDrawColor4F(color.r, color.g, color.b, color.a);
	cocos2d::ccDrawPoly(&(*_pointsArray.begin()), _pointsArray.size(), true);
}

void ArbitraryHull::Draw(void) const
{
	Color white(0xFFFFFFFF);
	Draw(white);
}

void ArbitraryHull::Clear(void)
{
	_pointsArray.clear();
}

int ArbitraryHull::GetPointsNum(void) const
{
	return _pointsArray.size();
}

const std::vector<cocos2d::Point>& ArbitraryHull::GetPointsArray(void) const
{
	return _pointsArray;
}

bool ArbitraryHull::Contain(const Vector2 &point) const
{
	unsigned int i, j;
	bool result = false;

	for (i = 0, j = _pointsArray.size() - 1; i < _pointsArray.size(); j = i++)
	{
		if ((_pointsArray[i].y > point.y) != (_pointsArray[j].y > point.y) &&
			(point.x < (_pointsArray[j].x - _pointsArray[i].x) * (point.y - _pointsArray[i].y) 
			/ (_pointsArray[j].y-_pointsArray[i].y) + _pointsArray[i].x))
		{
			result = !result;
		}
	}

	return result;
}