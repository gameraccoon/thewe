#include "Point.h"

#include <cocos2d.h>
#include <math.h>

const float VECTOR_ERROR = 0.0001f;

Point::Point()
{
	this->x = 0.0f;
	this->y = 0.0f;
}
Point::Point(float x, float y)
{
	this->x = x;
	this->y = y;
}

Point::Point(const Point& vector)
{
	this->x = vector.x;
	this->y = vector.y;
}

Point::Point(const cocos2d::CCPoint& point)
{
	this->x = point.x;
	this->y = point.y;
}

Point::Point(const cocos2d::CCSize& size)
{
	this->x = size.width;
	this->y = size.height;
}

Point::~Point(void)
{
}

float Point::Size(void) const
{
	return sqrt((this->x * this->x) + (this->y * this->y));
}

float Point::QSize(void) const
{
	return (this->x * this->x) + (this->y * this->y);
}

Point Point::Ort(void) const
{
	if (this->x == 0.0f && this->y == 0.0f)
		return Point(0.0f, 0.0f);

	return (*this) / this->Size();
}

Point Point::MirrorH() const
{
	return Point(-this->x, this->y);
}

Point Point::MirrorV() const
{
	return Point(this->x, -this->y);
}

Point Point::Project(Point base) const
{
	float qSize = base.QSize();
	Point result(0.0f, 0.0f);

	if (qSize != 0.0f)
	{
		float dProduct = DotProduct(base, (*this));
		result = base * dProduct / qSize;
	}

	return result;
}

Point::operator cocos2d::CCPoint() const
{
	return cocos2d::CCPoint(x, y);
}

Point::operator cocos2d::CCSize() const
{
	return cocos2d::CCSize(x, y);
}

Point operator-(const Point& vector)
{
	Point newVect(vector);
	newVect.x = -vector.x;
	newVect.y = -vector.y;
	return newVect;
}

bool operator==(const Point& left, const Point& right)
{
	float vError = 0.0f;
	vError += abs(left.x - right.x);
	vError += abs(left.y - right.y);

	return vError <= VECTOR_ERROR;
}

bool operator!=(const Point& left, const Point& right)
{
	return !(left == right);
}

Point operator+(const Point& left, const Point& right)
{
	Point newVect(left);
	
	newVect.x += right.x;
	newVect.y += right.y;

	return newVect;
}

Point operator+=(Point& left, const Point& right)
{
	left.x += right.x;
	left.y += right.y;

	return Point(left);
}

Point operator-(const Point& left, const Point& right)
{
	Point newVect(left);
	
	newVect.x -= right.x;
	newVect.y -= right.y;

	return newVect;
}

Point operator-=(Point& left, const Point& right)
{
	left.x -= right.x;
	left.y -= right.y;

	return Point(left);
}

Point operator*(const Point& vect, float scalar)
{
	Point newVect(vect);
	
	newVect.x *= scalar;
	newVect.y *= scalar;

	return newVect;
}

Point operator*(float scalar, const Point& vector)
{
	Point newVect(vector);
	
	newVect.x *= scalar;
	newVect.y *= scalar;

	return newVect;
}

Point operator*=(Point& vector, float scalar)
{
	vector.x *= scalar;
	vector.y *= scalar;

	return Point(vector);
}

Point operator*=(float scalar, Point& vector)
{
	vector.x *= scalar;
	vector.y *= scalar;

	return Point(vector);
}

Point operator/(const Point& vector, float scalar)
{
	Point newVect(vector);
	
	newVect.x /= scalar;
	newVect.y /= scalar;

	return newVect;
}

Point operator/=(Point& vector, float scalar)
{
	vector.x /= scalar;
	vector.y /= scalar;

	return Point(vector);
}

float DotProduct(const Point& left, const Point& right)
{
	return left.x * right.x + left.y * right.y;
}
