#ifndef CONVEX_HULL_H
#define CONVEX_HULL_H

#include "cocos2d.h"
#include <vector>

class ConvexHullTest
{
public:
	void AddPoint(const cocos2d::CCPoint &point);
	void Draw() const;
	void Draw(const cocos2d::ccColor4F &color) const;
	void Clear(void);
	
	int GetPointsNum(void) const;

	bool Contain(const cocos2d::CCPoint &point);

	const std::vector<cocos2d::CCPoint>& GetPoints() const;
private:
	std::vector<cocos2d::CCPoint> _pointsArray;
};

#endif