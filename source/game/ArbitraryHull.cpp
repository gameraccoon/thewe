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

bool ArbitraryHull::Contain(const cocos2d::CCPoint &point) const
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

void ArbitraryHull::SaveToXml(pugi::xml_document &docXml)
{
	pugi::xml_node root = docXml.first_child();

	pugi::xml_node node;
	node = root.append_child("Region");
	node.append_attribute("Name").set_value("Unnamed");
	
	for (std::vector<cocos2d::CCPoint>::iterator it = _pointsArray.begin(); it != _pointsArray.end(); ++it)
	{
		cocos2d::CCPoint p = (*it);

		pugi::xml_node point = node.append_child("Point");
		point.append_attribute("x").set_value(p.x);
		point.append_attribute("y").set_value(p.y);
	}
}

bool ArbitraryHull::SaveToXml(const char *xmlFilename)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(xmlFilename);
	
	if (result)
	{
		SaveToXml(doc);
		doc.save_file(xmlFilename);
		return true;
	}
	
	return false;
}