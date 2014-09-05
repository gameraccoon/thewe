#ifndef COLOR_H
#define COLOR_H

#include <cocos2d.h>

namespace cocos2d
{
	struct Color4F;
}

struct Color
{
	Color(float r, float g, float b, float a = 1.0);
	Color(unsigned long color); // 0xRRGGBBAA

	float r;
	float g;
	float b;
	float a;
	
	/* Ќе€вные преобразовани€ в другие классы */
	operator cocos2d::Color4F() const;
};

#endif // COLOR_H