#ifndef POINT_H
#define POINT_H

#include <cocos2d.h>

struct Point
{
	float x;
	float y;

	Point();
	Point(float x, float y);
	Point(const Point& point);
	Point(const cocos2d::CCPoint& point);
	Point(const cocos2d::CCSize& size);
	~Point(void);

	/** Получить длину вектора */
	float Size() const;

	/** Получить квадрат длины вектора (работает быстрее чем Size) */
	float QSize(void) const;

	/** Получить вектор единичной длины, сонаправленный с данным */
	Point Ort(void) const;
	/** Получить вектор отражённый от оси OY */
	Point MirrorH() const;
	/** Получить вектор отражённый от оси OX */
	Point MirrorV() const;
	/** Получить проекцию вектора */
	//Point Normal() const;

	/** Получить проекцию вектора на вектор base */
	Point Project(Point base) const;
	
	/* Неявные преобразования в другие классы */
	operator cocos2d::CCPoint() const { return cocos2d::CCPoint(x, y); }
	operator cocos2d::CCSize() const { return cocos2d::CCSize(x, y); }

	/* Арифметические операции, доступные для двумерных векторов */

	friend bool operator==(const Point& left, const Point& right);
	friend bool operator!=(const Point& left, const Point& right);

	friend Point operator-(const Point& vector);

	friend Point operator+(const Point& left, const Point& right);
	friend Point operator+=(Point& left, const Point& right);

	friend Point operator-(const Point& left, const Point& right);
	friend Point operator-=(Point& left, const Point& right);

	friend Point operator*(const Point& vector, float scalar);
	friend Point operator*(float scalar, const Point& vector);
	friend Point operator*=(Point& vector, float scalar);
	friend Point operator*=(float scalar, Point& vector);

	friend Point operator/(const Point& vector, float scalar);
	friend Point operator/=(Point& vector, float scalar);

	friend float DotProduct(const Point& left, const Point& right);
};

#endif // POINT_H