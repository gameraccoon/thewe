#ifndef POINT_H
#define POINT_H

#include <cocos2d.h>

struct Vector2
{
	float x;
	float y;

	Vector2();
	Vector2(float x, float y);
	Vector2(const Vector2& point);
	Vector2(const cocos2d::Point& p);
	Vector2(const cocos2d::Size& size);
	~Vector2(void);

	/** Получить длину вектора */
	float Size() const;

	/** Получить квадрат длины вектора (работает быстрее чем Size) */
	float QSize(void) const;

	/** Получить вектор единичной длины, сонаправленный с данным */
	Vector2 Ort(void) const;
	/** Получить вектор отражённый от оси OY */
	Vector2 MirrorH() const;
	/** Получить вектор отражённый от оси OX */
	Vector2 MirrorV() const;
	/** Получить проекцию вектора */
	//Vector2 Normal() const;

	/** Получить проекцию вектора на вектор base */
	Vector2 Project(Vector2 base) const;
	
	/* Неявные преобразования в другие классы */
	operator cocos2d::Point() const;
	operator cocos2d::Size() const;

	/* Арифметические операции, доступные для двумерных векторов */

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