#ifndef COLOR_H
#define COLOR_H

#include <cocos2d.h>

struct Color
{
	Color(float r, float g, float b, float a = 1.0);
	Color(unsigned long color); // 0xRRGGBBAA

	float r;
	float g;
	float b;
	float a;
	
	/* Ќе€вные преобразовани€ в другие классы */
	operator cocos2d::ccColor4F() const
	{
		cocos2d::ccColor4F color;
		color.r = r;
		color.g = g;
		color.b = b;
		color.a = a;
		return color;
	}
};

#endif // COLOR_H