#include "Vector2.h"

#include <cocos2d.h>
#include <math.h>

const float VECTOR_ERROR = 0.0001f;

Vector2::Vector2()
{
	this->x = 0.0f;
	this->y = 0.0f;
}
Vector2::Vector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

Vector2::Vector2(const Vector2& vector)
{
	this->x = vector.x;
	this->y = vector.y;
}

Vector2::Vector2(const cocos2d::Point& p)
{
	this->x = p.x;
	this->y = p.y;
}

Vector2::Vector2(const cocos2d::Size& size)
{
	this->x = size.width;
	this->y = size.height;
}

Vector2::~Vector2(void)
{
}

float Vector2::Size(void) const
{
	return sqrt((this->x * this->x) + (this->y * this->y));
}

float Vector2::QSize(void) const
{
	return (this->x * this->x) + (this->y * this->y);
}

Vector2 Vector2::Ort(void) const
{
	if (this->x == 0.0f && this->y == 0.0f)
		return Vector2(0.0f, 0.0f);

	return (*this) / this->Size();
}

Vector2 Vector2::MirrorH() const
{
	return Vector2(-this->x, this->y);
}

Vector2 Vector2::MirrorV() const
{
	return Vector2(this->x, -this->y);
}

Vector2 Vector2::Project(Vector2 base) const
{
	float qSize = base.QSize();
	Vector2 result(0.0f, 0.0f);

	if (qSize != 0.0f)
	{
		float dProduct = DotProduct(base, (*this));
		result = base * dProduct / qSize;
	}

	return result;
}

Vector2::operator cocos2d::Point() const
{
	return cocos2d::Point(x, y);
}

Vector2::operator cocos2d::Size() const
{
	return cocos2d::Size(x, y);
}

Vector2 operator-(const Vector2& vector)
{
	Vector2 newVect(vector);
	newVect.x = -vector.x;
	newVect.y = -vector.y;
	return newVect;
}

bool operator==(const Vector2& left, const Vector2& right)
{
	float vError = 0.0f;
	vError += abs(left.x - right.x);
	vError += abs(left.y - right.y);

	return vError <= VECTOR_ERROR;
}

bool operator!=(const Vector2& left, const Vector2& right)
{
	return !(left == right);
}

Vector2 operator+(const Vector2& left, const Vector2& right)
{
	Vector2 newVect(left);
	
	newVect.x += right.x;
	newVect.y += right.y;

	return newVect;
}

Vector2 operator+=(Vector2& left, const Vector2& right)
{
	left.x += right.x;
	left.y += right.y;

	return Vector2(left);
}

Vector2 operator-(const Vector2& left, const Vector2& right)
{
	Vector2 newVect(left);
	
	newVect.x -= right.x;
	newVect.y -= right.y;

	return newVect;
}

Vector2 operator-=(Vector2& left, const Vector2& right)
{
	left.x -= right.x;
	left.y -= right.y;

	return Vector2(left);
}

Vector2 operator*(const Vector2& vect, float scalar)
{
	Vector2 newVect(vect);
	
	newVect.x *= scalar;
	newVect.y *= scalar;

	return newVect;
}

Vector2 operator*(float scalar, const Vector2& vector)
{
	Vector2 newVect(vector);
	
	newVect.x *= scalar;
	newVect.y *= scalar;

	return newVect;
}

Vector2 operator*=(Vector2& vector, float scalar)
{
	vector.x *= scalar;
	vector.y *= scalar;

	return Vector2(vector);
}

Vector2 operator*=(float scalar, Vector2& vector)
{
	vector.x *= scalar;
	vector.y *= scalar;

	return Vector2(vector);
}

Vector2 operator/(const Vector2& vector, float scalar)
{
	Vector2 newVect(vector);
	
	newVect.x /= scalar;
	newVect.y /= scalar;

	return newVect;
}

Vector2 operator/=(Vector2& vector, float scalar)
{
	vector.x /= scalar;
	vector.y /= scalar;

	return Vector2(vector);
}

float DotProduct(const Vector2& left, const Vector2& right)
{
	return left.x * right.x + left.y * right.y;
}
