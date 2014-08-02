#ifndef MAP_PROJECTOR_H
#define MAP_PROJECTOR_H

#include <memory>
#include <cocos2d.h>
#include "Point.h"
#include "ArbitraryHull.h"

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
	void AddMapPart(Point location, Point shift, cocos2d::CCNode *node);
	/** Добавить спрайт по имени файла */
	cocos2d::CCSprite* AddSprite(Point location, Point shift, std::string spriteName);
	
	/** Удалить и разрушить элемент для проецирования */
	void RemoveMapPart(const cocos2d::CCNode *node);

private:
	struct MapPart
	{
		/** Умный указатель с возможностью устонавливать свои правила удаления объектов */
		typedef std::shared_ptr<cocos2d::CCNode> NodePtr;
		/** Положение объекта в мире */
		Point location;
		/** Сдвиг центра объекта */
		Point shift;
		/** Указатель на объект */
		NodePtr node;
	};

	/** Проецируемые элементы */
	typedef std::vector<MapPart> MapParts;

	/** Проверить выход границ вида за границы карты и исправить положение вида при необходимости */
	void CheckBoundings();

	/** Обновить положение и масштаб всех проецируемых элементов */
	void UpdateNodes();

	float _viewScale;
	Point _viewLocation;

	Point _screenCenter;

	Point _mapSize;

	MapParts _mapParts;
};

#endif // MAP_PROJECTOR_H