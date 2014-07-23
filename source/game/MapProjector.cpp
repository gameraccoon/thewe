#include "MapProjector.h"


static const cocos2d::CCPoint MAP_INITIAL_SIZE = cocos2d::CCPoint(1390.0f, 1003.0f);

MapProjector::MapProjector(cocos2d::CCPoint shift, float scale)
{
	// ToDo: вынести инициализацию спрайтов в отдельный класс
	_mapSprite = cocos2d::CCSprite::create("../_gamedata/WorldMap.png");

	SetShift(shift);
	SetScale(scale);
}

void MapProjector::SetShift(cocos2d::CCPoint shift)
{
	_mapShift = shift;
	GetSprite()->setPosition(shift);
}

void MapProjector::SetScale(float scale)
{
	SetShift(_screenCenter + (_mapShift - _screenCenter) * (scale / _mapScale));
	_mapScale = scale;
	GetSprite()->setScale(scale);
}

cocos2d::CCPoint MapProjector::GetShift()
{
	return _mapShift;
}

float MapProjector::GetScale()
{
	return _mapScale;
}

cocos2d::CCPoint MapProjector::ProjectOnMap(cocos2d::CCPoint screenPoint) const
{
	return (screenPoint - _mapShift) / _mapScale;
}

cocos2d::CCPoint MapProjector::ProjectOnScreen(cocos2d::CCPoint mapPoint) const
{
	return (mapPoint * _mapScale) + _mapShift;
}

cocos2d::CCSprite* MapProjector::GetSprite() const
{
	return _mapSprite;
}

void MapProjector::SetScreenCenter(cocos2d::CCPoint centerPos)
{
	_screenCenter = centerPos;	
}