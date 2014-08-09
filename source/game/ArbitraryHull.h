#ifndef ARBITRARY_HULL_H
#define ARBITRARY_HULL_H

#include <vector>
#include <pugixml.hpp>
#include "Color.h"
#include "Point.h"

// избавляемся от необходимости включать все заголовки cocos2d
namespace cocos2d
{
	class CCpoint;
}

class ArbitraryHull
{
public:
	void PushPoint(const Point &point);
	void PopPoint(void);
	void Draw(const Color &color) const;
	void Draw(void) const;
	void Clear(void);
	
	int GetPointsNum(void) const;

	bool Contain(const Point &point) const;

	void SaveToXml(pugi::xml_document &docXml);
	bool SaveToXml(const char *xmlFilename);

private:
	std::vector<cocos2d::CCPoint> _pointsArray;

	friend class MapProjector;
};

#endif