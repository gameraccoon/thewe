#ifndef ARBITRARY_HULL_H
#define ARBITRARY_HULL_H

#include <vector>
#include "Color.h"
#include "Vector2.h"

// избавляемся от необходимости включать все заголовки cocos2d
namespace cocos2d
{
	class CCpoint;
}

class ArbitraryHull
{
public:
	void PushPoint(const Vector2 &point);
	void PopPoint(void);
	void Draw(const Color &color) const;
	void Draw(void) const;
	void Clear(void);
	
	int GetPointsNum(void) const;
	const std::vector<cocos2d::Point>& GetPointsArray(void) const;

	bool Contain(const Vector2 &point) const;

private:
	std::vector<cocos2d::CCPoint> _pointsArray;

	friend class MapProjector;
	friend class EditorLayer;
};

#endif