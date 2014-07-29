#ifndef ARBITRARY_HULL_H
#define ARBITRARY_HULL_H

#include <cocos2d.h>
#include <vector>
#include <pugixml.hpp>

#include "Point.h"

class ArbitraryHull
{
public:
	void PushPoint(const Point &point);
	void PopPoint(void);
	void Draw(const cocos2d::ccColor4F &color) const;
	void Draw(void) const;
	void Clear(void);
	
	int GetPointsNum(void) const;
	const std::vector<Point>& GetPoints(void) const;

	bool Contain(const Point &point) const;

	void SaveToXml(pugi::xml_document &docXml);
	bool SaveToXml(const char *xmlFilename);

private:
	std::vector<Point> _pointsArray;
};

#endif