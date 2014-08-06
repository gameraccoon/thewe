#ifndef MAP_PROJECTOR_H
#define MAP_PROJECTOR_H

#include <memory>
#include <string>
#include "Point.h"
#include "ArbitraryHull.h"
#include "Drawable.h"

class MapProjector
{
public:
	MapProjector(Point mapSize);

	/** Спроецировать точку из координат экрана в координаты карты */
	Point ProjectOnMap(Point screenPoint) const;
	/** Спроецировать точку из координат карты в координаты экрана */
	Point ProjectOnScreen(Point mapPoint) const;

	/** Спроецировать полигон из координат экрана в координаты карты */
	ArbitraryHull ProjectOnMap(const ArbitraryHull& screenHull) const;
	/** Спроецировать полигон из координат карты в координаты экрана */
	ArbitraryHull ProjectOnScreen(const ArbitraryHull& mapHull) const;

	/** Установить новое положение камеры на карте */
	void SetLocation(Point mapLocation);
	/** Установить приближение вида */
	void SetScale(float scale);

	/** Сдвинуть вид в координатах экрана */
	void ShiftView(Point screenDelta);

	/** Получить положение камеры в мире */
	Point GetLocation() const;
	/** Получить текущее значение приближения вида */
	float GetScale() const;

	/** Сказать в какой точке экрана находится центер области вывода */
	void SetScreenCenter(Point centerPos);
	
	/** Добавить новый элемент, который будет проецироваться на карту */
	void AddMapPart(Drawable::Ptr node, Point location, Point shift, float scale, bool dontScale = false);
	
	/** Удалить и разрушить элемент для проецирования */
	void RemoveMapPart(const Drawable::Ptr node);

private:
	struct MapPart
	{
		/** Положение объекта в мире */
		Point location;
		/** Сдвиг центра объекта */
		Point shift;
		/** Масштаб объекта по умолчанию */
		float initialScale;
		/** Нужно ли масштабировать объект вместе с картой */
		bool isScalable;
		/** Указатель на графический объект */
		Drawable::Ptr node;
	};

	/** Проецируемые элементы */
	typedef std::vector<MapPart> MapParts;

private:
	/** Проверить выход границ вида за границы карты и исправить положение вида при необходимости */
	void _CheckBoundings();

	/** Обновить положение и масштаб всех проецируемых элементов */
	void _UpdateNodes();

	float _viewScale;
	Point _viewLocation;

	Point _screenCenter;

	Point _mapSize;

	MapParts _mapParts;
};

#endif // MAP_PROJECTOR_H