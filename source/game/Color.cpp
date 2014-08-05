#include "Color.h"

Color::Color(float r, float g, float b, float a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

Color::Color(unsigned long color)
{
	this->a = (color & 0xFF000000) * 0.00390625f; // 0xFF
	this->r = (color & 0x00FF0000) * 0.00390625f;
	this->g = (color & 0x0000FF00) * 0.00390625f;
	this->b = (color & 0x000000FF) * 0.00390625f;
}
