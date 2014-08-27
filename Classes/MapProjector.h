#ifndef MAP_PROJECTOR_H
#define MAP_PROJECTOR_H

#include <memory>
#include <string>
#include "Vector2.h"
#include "ArbitraryHull.h"
#include "Drawable.h"

class MapProjector
{
public:
	MapProjector(Vector2 mapSize);

	/** Спроецировать точку из координат экрана в координаты карты */
	Vector2 ProjectOnMap(Vector2 screenPoint) const;
	/** Спроецировать точку из координат карты в координаты экрана */
	Vector2 ProjectOnScreen(Vector2 mapPoint) const;

	/** Спроецировать полигон из координат экрана в координаты карты */
	ArbitraryHull ProjectOnMap(const ArbitraryHull& screenHull) const;
	/** Спроецировать полигон из координат карты в координаты экрана */
	ArbitraryHull ProjectOnScreen(const ArbitraryHull& mapHull) const;

	/** Установить новое положение камеры на карте */
	void SetLocation(Vector2 mapLocation);
	/** Установить приближение вида */
	void SetScale(float scale);

	/** Сдвинуть вид в координатах экрана */
	void ShiftView(Vector2 screenDelta);

	/** Получить положение камеры в мире */
	Vector2 GetLocation() const;
	/** Получить текущее значение приближения вида */
	float GetScale() const;

	/** Сказать в какой точке экрана находится центер области вывода */
	void SetScreenCenter(Vector2 centerPos);
	
	/** Добавить новый элемент, который будет проецироваться на карту */
	void AddMapPart(Drawable::Ptr node, Vector2 location, Vector2 shift, float scale, bool dontScale = false);
	
	/** Удалить и разрушить элемент для проецирования */
	void RemoveMapPart(const Drawable::Ptr node);

	/** Обновить положение всех элементов (например после добавления) */
	void Update();

private:
	struct MapPart
	{
		/** Положение объекта в мире */
		Vector2 location;
		/** Сдвиг центра объекта */
		Vector2 shift;
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
	Vector2 _viewLocation;

	Vector2 _screenCenter;

	Vector2 _mapSize;

	MapParts _mapParts;
};

#endif // MAP_PROJECTOR_H