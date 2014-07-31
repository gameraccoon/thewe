#include "MapProjector.h"

static const Point MAP_INITIAL_SIZE = Point(1390.0f, 1003.0f);

MapProjector::MapProjector(Point mapSpriteSize)
	: _mapSpriteSize(mapSpriteSize)
	, _mapScale(1.0f)
	, _mapLocation(0.0f, 0.0f)
{
}

void MapProjector::SetLocation(Point worldLocation)
{
	_mapLocation = worldLocation;

	CheckBoundings();

	UpdateNodes();
}

void MapProjector::SetScale(float scale)
{
	if (scale < (_screenCenter.y * 2) / _mapSpriteSize.y)
	{
		_mapScale = (_screenCenter.y * 2) / _mapSpriteSize.y;
	}
	else
	{
		_mapScale = scale;
	}
	
	CheckBoundings();
	UpdateNodes();
}

void MapProjector::ShiftView(Point delta)
{
	SetLocation(_mapLocation + delta / _mapScale);
}

void MapProjector::CheckBoundings()
{
	if (_mapLocation.y < _screenCenter.y / _mapScale - _mapSpriteSize.y/2)
	{
		_mapLocation.y = _screenCenter.y / _mapScale - _mapSpriteSize.y/2;
	}

	if (_mapLocation.y > -_screenCenter.y / _mapScale + _mapSpriteSize.y/2)
	{
		_mapLocation.y = -_screenCenter.y / _mapScale + _mapSpriteSize.y/2;
	}
}

Point MapProjector::GetShift() const
{
	return _mapLocation;
}

float MapProjector::GetScale() const
{
	return _mapScale;
}

Point MapProjector::ProjectOnMap(Point screenPoint) const
{
	return (screenPoint - _screenCenter) / _mapScale - _mapLocation;
}

Point MapProjector::ProjectOnScreen(Point mapPoint) const
{
	return _screenCenter + (mapPoint + _mapLocation) * _mapScale;
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

void MapProjector::AddNode(Point location, Point shift, cocos2d::CCNode *node)
{
	MapPart locSprite;
	locSprite.location = location;
	locSprite.shift = shift;
	locSprite.node = node;
	_nodesToProject.push_back(locSprite);
}

void MapProjector::UpdateNodes()
{
	for (const MapPart& node : _nodesToProject)
	{
		node.node->setPosition(_screenCenter + ((node.location + node.shift) + _mapLocation) * _mapScale);
	}

	for (const MapPart& node : _nodesToProject)
	{
		node.node->setScale(_mapScale);
	}
}