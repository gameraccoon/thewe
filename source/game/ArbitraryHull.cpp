#include "ArbitraryHull.h"

void ArbitraryHull::PushPoint(const cocos2d::CCPoint &point)
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

void ArbitraryHull::Draw(const cocos2d::ccColor4F &color) const
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
	cocos2d::ccColor4F white;
	white.r = 1.0f;
	white.g = 1.0f;
	white.b = 1.0f;
	white.a = 1.0f;

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

const std::vector<cocos2d::CCPoint>& ArbitraryHull::GetPoints(void) const
{
	return _pointsArray;
}

bool ArbitraryHull::Contain(const cocos2d::CCPoint &point)
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

void ArbitraryHull::SaveToNewXml(const char *xmlFilename)
{
}

void ArbitraryHull::SaveToExistingXml(const char *xmlFilename)
{
}

bool ArbitraryHull::CreateFromXml(const char *xmlFilename)
{
	return false;
}