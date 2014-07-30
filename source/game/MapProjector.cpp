#include "MapProjector.h"

static const Point MAP_INITIAL_SIZE = Point(1390.0f, 1003.0f);

MapProjector::MapProjector(Point spriteSize)
	: _mapSpriteSize(spriteSize)
	, _mapScale(1.0f)
	, _mapShift(0.0f, 0.0f)
{
}

void MapProjector::SetShift(Point shift)
{
	_mapShift = shift;
	if (shift.y > _mapScale * _mapSpriteSize.y / 2)
	{
		_mapShift.y = _mapScale * _mapSpriteSize.y / 2;
	}

	if (shift.y < 2 * _screenCenter.y - _mapScale * _mapSpriteSize.y / 2)
	{
		_mapShift.y = (2 * _screenCenter.y - _mapScale * _mapSpriteSize.y / 2);
	}
	
	for (const LocatedSprite& sprite : _spritesToProject)
	{
		sprite.sprite->setPosition(_mapShift);
	}
}

void MapProjector::SetScale(float scale)
{
	Point oldShift = _mapShift;
	// предотвращаем сдвиг камеры относительно центра карты
	SetShift(_screenCenter + (_mapShift - _screenCenter) * (scale / _mapScale));
	_mapScale = scale;
	
	if (_mapShift.y > _mapScale * _mapShift.y / 2 && _mapShift.y < 2 * _screenCenter.y - _mapScale * _mapSpriteSize.y / 2)
	{
		_mapScale = (_screenCenter.y * 2) / _mapSpriteSize.y;
		// хак -- предотвращение сдвига в сторону центра карты при максимальном отдалении
		SetShift(oldShift);
	}

	
	for (const LocatedSprite& sprite : _spritesToProject)
	{
		sprite.sprite->setScale(_mapScale);
	}

	// хак -- предотвращение выхода за границу карты при отдалении
	SetShift(_mapShift);
}

Point MapProjector::GetShift() const
{
	return _mapShift;
}

float MapProjector::GetScale() const
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

ArbitraryHull MapProjector::ProjectOnMap(const ArbitraryHull& screenHull) const
{
	ArbitraryHull projectedHull;
	for (auto &point : screenHull._pointsArray)
	{
		projectedHull.PushPoint(ProjectOnMap(point));
	}
	return projectedHull;
}

ArbitraryHull MapProjector::ProjectOnScreen(const ArbitraryHull& screenHull) const
{
	ArbitraryHull projectedHull;
	for (auto &point : screenHull._pointsArray)
	{
		projectedHull.PushPoint(ProjectOnScreen(point));
	}
	return projectedHull;
}

void MapProjector::SetScreenCenter(Point centerPos)
{
	_screenCenter = centerPos;	
}

void MapProjector::AddSprite(Point location, Point shift, cocos2d::CCSprite *sprite)
{
	LocatedSprite locSprite;
	locSprite.location = location;
	locSprite.shift = shift;
	locSprite.sprite = sprite;
	_spritesToProject.push_back(locSprite);
}