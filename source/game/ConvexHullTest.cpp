#include "ConvexHullTest.h"

void ConvexHullTest::AddPoint(const cocos2d::CCPoint &point)
{
	_pointsArray.push_back(point);
}

void ConvexHullTest::Draw(const cocos2d::ccColor4F &color) const
{
	//cocos2d::ccDrawSolidPoly(&(*_pointsArray.begin()), _pointsArray.size(), color);

	cocos2d::ccDrawColor4F(color.r, color.g, color.b, color.a);
	cocos2d::ccDrawPoly(&(*_pointsArray.begin()), _pointsArray.size(), true);
}

void ConvexHullTest::Clear(void)
{
	_pointsArray.clear();
}

bool ConvexHullTest::Contain(const cocos2d::CCPoint &point)
{
	unsigned int i, j;
	bool result = false;

	for (i = 0, j = _pointsArray.size() - 1; i < _pointsArray.size(); j = i++)
	{
		// ...тада, я сам в шоке от этого кода.
		if ((_pointsArray[i].y > point.y) != (_pointsArray[j].y > point.y) &&
			(point.x < (_pointsArray[j].x - _pointsArray[i].x) * (point.y - _pointsArray[i].y) 
			/ (_pointsArray[j].y-_pointsArray[i].y) + _pointsArray[i].x))
		{
			result = !result;
		}
	}

	return result;
}