#include "Color.h"

#include <cocos2d.h>

Color::Color(float r, float g, float b, float a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

Color::Color(unsigned long color)
{
	this->a = ((color & 0xFF000000) >> 24) * 0.00390625f; // * 1.0f / 0xFF
	this->r = ((color & 0x00FF0000) >> 16) * 0.00390625f;
	this->g = ((color & 0x0000FF00) >> 4) * 0.00390625f;
	this->b = (color & 0x000000FF) * 0.00390625f;
}

Color::operator cocos2d::Color4F() const
{
	return cocos2d::Color4F(r, g, b, a);
}

Color::operator cocos2d::Color4B() const
{
	return cocos2d::Color4B(r * 255.0f, g * 255.0f, b * 255.0f, a * 255.0f);
}

Color::operator cocos2d::Color3B() const
{
	return cocos2d::Color3B(r * 255.0f, g * 255.0f, b * 255.0f);
}
