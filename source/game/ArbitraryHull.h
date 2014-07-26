#ifndef ARBITRARY_HULL_H
#define ARBITRARY_HULL_H

#include <cocos2d.h>
#include <vector>
#include <pugixml.hpp>

class ArbitraryHull
{
public:
	void PushPoint(const cocos2d::CCPoint &point);
	void PopPoint(void);
	void Draw(const cocos2d::ccColor4F &color) const;
	void Draw(void) const;
	void Clear(void);
	
	int GetPointsNum(void) const;
	const std::vector<cocos2d::CCPoint>& GetPoints(void) const;

	bool Contain(const cocos2d::CCPoint &point);

	void SaveToXml(pugi::xml_document &docXml);
	bool SaveToXml(const char *xmlFilename);

private:
	std::vector<cocos2d::CCPoint> _pointsArray;
};

#endif