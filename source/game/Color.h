#ifndef COLOR_H
#define COLOR_H

// избавляемся от необходимости включать все заголовки cocos2d
#include <misc_nodes/ccrendertexture.h>

struct Color
{
	Color(float r, float g, float b, float a = 1.0);
	Color(unsigned long color); // 0xRRGGBBAA

	float r;
	float g;
	float b;
	float a;
	
	/* Неявные преобразования в другие классы */
	operator cocos2d::ccColor4F() const;
};

#endif // COLOR_H