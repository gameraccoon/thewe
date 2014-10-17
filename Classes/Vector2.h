#ifndef POINT_H
#define POINT_H

namespace cocos2d
{
	class Vec2;
	class Size;
}

struct Vector2
{
	float x;
	float y;

	Vector2();
	Vector2(float x, float y);
	Vector2(const Vector2& point);
	Vector2(const cocos2d::Vec2& p);
	Vector2(const cocos2d::Size& size);
	~Vector2(void);

	/** Calculate vector length */
	float Size() const;

	/** Calculate quarter of the vector length (faster than Size) */
	float QSize(void) const;

	/** Calculate normalized vector (length = 1 unit) */
	Vector2 Ort(void) const;
	/** Calculate vector mirrored to OX axis */
	Vector2 MirrorH() const;
	/** Calculate vector mirrored to OX axis */
	Vector2 MirrorV() const;
	/** Calculate vector normal */
	//Vector2 Normal() const;

	/** Calculate projection to the vector "base" */
	Vector2 Project(Vector2 base) const;
	
	/* Implicit convertations to other classes */
	operator cocos2d::Vec2() const;
	operator cocos2d::Size() const;

	/* Ariphmetic operation for two-dimentioned vectors */

	friend bool operator==(const Vector2& left, const Vector2& right);
	friend bool operator!=(const Vector2& left, const Vector2& right);

	friend Vector2 operator-(const Vector2& vector);

	friend Vector2 operator+(const Vector2& left, const Vector2& right);
	friend Vector2 operator+=(Vector2& left, const Vector2& right);

	friend Vector2 operator-(const Vector2& left, const Vector2& right);
	friend Vector2 operator-=(Vector2& left, const Vector2& right);

	friend Vector2 operator*(const Vector2& vector, float scalar);
	friend Vector2 operator*(float scalar, const Vector2& vector);
	friend Vector2 operator*=(Vector2& vector, float scalar);
	friend Vector2 operator*=(float scalar, Vector2& vector);

	friend Vector2 operator/(const Vector2& vector, float scalar);
	friend Vector2 operator/=(Vector2& vector, float scalar);

	friend float DotProduct(const Vector2& left, const Vector2& right);
};

#endif // POINT_H
