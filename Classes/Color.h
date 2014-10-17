#ifndef COLOR_H
#define COLOR_H

namespace cocos2d
{
	struct Color3B;
	struct Color4F;
	struct Color4B;
}

struct Color
{
	Color(float r, float g, float b, float a = 1.0);
	Color(unsigned long color); // 0xAARRGGBB

	float r;
	float g;
	float b;
	float a;
	
	/* implicit conversation to other classes */
	operator cocos2d::Color3B() const;
	operator cocos2d::Color4F() const;
	operator cocos2d::Color4B() const;
};

#endif // COLOR_H
