#include "MapProjector.h"

static const Point MAP_INITIAL_SIZE = Point(1390.0f, 1003.0f);

MapProjector::MapProjector(Point shift, float scale)
{
	_mapSprite = nullptr;
	// ToDo: вынести инициализацию спрайтов в отдельный класс
	_mapSprite = cocos2d::CCSprite::create("WorldMap.png");

	SetShift(shift);
	SetScale(scale);
}

void MapProjector::SetShift(Point shift)
{
	Point spriteSize = GetSprite()->getContentSize();
	
	_mapShift = shift;
	if (shift.y > _mapScale * spriteSize.y / 2)
	{
		_mapShift.y = _mapScale * spriteSize.y / 2;
	}

	if (shift.y < 2 * _screenCenter.y - _mapScale * spriteSize.y / 2)
	{
		_mapShift.y = (2 * _screenCenter.y - _mapScale * spriteSize.y / 2);
	}

	if (_mapSprite)
	{
		_mapSprite->setPosition(_mapShift);
	}
}

void MapProjector::SetScale(float scale)
{
	Point oldShift = _mapShift;
	// предотвращаем сдвиг камеры относительно центра карты
	SetShift(_screenCenter + (_mapShift - _screenCenter) * (scale / _mapScale));
	_mapScale = scale;
	
	Point spriteSize = GetSprite()->getContentSize();
	if (_mapShift.y > _mapScale * _mapShift.y / 2 && _mapShift.y < 2 * _screenCenter.y - _mapScale * spriteSize.y / 2)
	{
		_mapScale = (_screenCenter.y * 2) / spriteSize.y;
		// хак -- предотвращение сдвига в сторону центра карты при максимальном отдалении
		SetShift(oldShift);
	}

	if (_mapSprite)
	{
		_mapSprite->setScale(_mapScale);
	}

	// хак -- предотвращение выхода за границу карты при отдалении
	SetShift(_mapShift);
}

Point MapProjector::GetShift()
{
	return _mapShift;
}

float MapProjector::GetScale()
{
	return _mapScale;
}

Point MapProjector::ProjectOnMap(Point screenPoint) const
{
	return (screenPoint - _mapShift) / _mapScale;
}

Point MapProjector::ProjectOnScreen(Point mapPoint) const
{
	return (mapPoint * _mapScale) + _mapShift;
}

cocos2d::CCSprite* MapProjector::GetSprite() const
{
	return _mapSprite;
}

void MapProjector::SetScreenCenter(Point centerPos)
{
	_screenCenter = centerPos;	
}