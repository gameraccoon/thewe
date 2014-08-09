#ifndef POINT_H
#define POINT_H

// ����������� �� ������������� �������� ��� ��������� cocos2d
namespace cocos2d
{
	class CCPoint;
	class CCSize;
}

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

	/** �������� ����� ������� */
	float Size() const;

	/** �������� ������� ����� ������� (�������� ������� ��� Size) */
	float QSize(void) const;

	/** �������� ������ ��������� �����, �������������� � ������ */
	Point Ort(void) const;
	/** �������� ������ ��������� �� ��� OY */
	Point MirrorH() const;
	/** �������� ������ ��������� �� ��� OX */
	Point MirrorV() const;
	/** �������� �������� ������� */
	//Point Normal() const;

	/** �������� �������� ������� �� ������ base */
	Point Project(Point base) const;
	
	/* ������� �������������� � ������ ������ */
	operator cocos2d::CCPoint() const;
	operator cocos2d::CCSize() const;

	/* �������������� ��������, ��������� ��� ��������� �������� */

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