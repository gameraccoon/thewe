#include "MapProjector.h"

MapProjector::MapProjector()
	: _mapSize(0.0f, 0.0f)
	, _viewScale(1.0f)
	, _viewLocation(0.0f, 0.0f)
{
}

void MapProjector::SetLocation(Vector2 worldLocation)
{
	_viewLocation = worldLocation;

	// check and fix getting out of screen
	_CheckBoundings();
	// update position of nodes
	_UpdateNodes();
}

void MapProjector::SetScale(float scale)
{
	// if view is out of screen borders
	if (scale < (_screenCenter.y * 2) / _mapSize.y || scale < (_screenCenter.x * 2) / _mapSize.x)
	{
		// align view to smaller border
#ifndef WIN32
		_viewScale = 2 * std::max(_screenCenter.y / _mapSize.y, _screenCenter.x / _mapSize.x);
#else
		_viewScale = 2 * max(_screenCenter.y / _mapSize.y, _screenCenter.x / _mapSize.x);
#endif
	}
	else
	{
		_viewScale = scale;
	}
	
	// check and fix getting out of screen
	_CheckBoundings();
	// update position of nodes
	_UpdateNodes();
}

void MapProjector::ShiftView(Vector2 delta)
{
	SetLocation(_viewLocation - delta / _viewScale);
}

void MapProjector::_CheckBoundings()
{
	// out of the top border
	if (_viewLocation.y < _screenCenter.y / _viewScale - _mapSize.y/2)
	{
		_viewLocation.y = _screenCenter.y / _viewScale - _mapSize.y/2;
	}

	// out of the bottom border
	if (_viewLocation.y > -_screenCenter.y / _viewScale + _mapSize.y/2)
	{
		_viewLocation.y = -_screenCenter.y / _viewScale + _mapSize.y/2;
	}

	// out of the right border
	if (_viewLocation.x < _screenCenter.x / _viewScale - _mapSize.x/2)
	{
		_viewLocation.x = _screenCenter.x / _viewScale - _mapSize.x/2;
	}

	// out of the left border
	if (_viewLocation.x > -_screenCenter.x / _viewScale + _mapSize.x/2)
	{
		_viewLocation.x = -_screenCenter.x / _viewScale + _mapSize.x/2;
	}
}

unsigned int MapProjector::_GetNewPartUid(void) const
{
	static unsigned int uid = 0;
	return uid++;
}

Vector2 MapProjector::GetLocation() const
{
	return _viewLocation;
}

float MapProjector::GetScale() const
{
	return _viewScale;
}

Vector2 MapProjector::ProjectOnMap(Vector2 screenPoint) const
{
	return (screenPoint - _screenCenter) / _viewScale + _viewLocation;
}

Vector2 MapProjector::ProjectOnScreen(Vector2 mapPoint) const
{
	return _screenCenter + (mapPoint - _viewLocation) * _viewScale;
}

ArbitraryHull MapProjector::ProjectOnMap(const ArbitraryHull& screenHull) const
{
	ArbitraryHull projectedHull;
	for (auto &point : screenHull.GetPoints())
	{
		projectedHull.PushPoint(ProjectOnMap(point));
	}
	return projectedHull;
}

ArbitraryHull MapProjector::ProjectOnScreen(const ArbitraryHull& mapHull) const
{
	ArbitraryHull projectedHull;
	for (auto &point : mapHull.GetPoints())
	{
		projectedHull.PushPoint(ProjectOnScreen(point));
	}
	return projectedHull;
}

void MapProjector::SetScreenCenter(Vector2 centerPos)
{
	_screenCenter = centerPos;	
}

void MapProjector::SetMapSize(const Vector2& mapSize)
{
	_mapSize = mapSize;
}

int MapProjector::AddMapPart(Drawable::Ptr node, Vector2 location, Vector2 shift, float scale, bool dontScale)
{
	MapPart locSprite;
	locSprite.location = location;
	locSprite.initialScale = scale;
	locSprite.shift = shift;
	locSprite.node = node;
	locSprite.isScalable = !dontScale;
	locSprite.uid = _GetNewPartUid();
	_mapParts.push_back(locSprite);

	return locSprite.uid;
}

void MapProjector::_UpdateNodes()
{
	for (const MapPart& node : _mapParts)
	{
		float screenScale;

		if (node.isScalable)
		{
			screenScale = _viewScale * node.initialScale;
		}
		else
		{
			screenScale = node.initialScale;
		}

		node.node->SetPosition(_screenCenter + (node.location - _viewLocation) * _viewScale + node.shift * screenScale);
		node.node->SetScale(screenScale);
	}
}

void MapProjector::RemoveMapPart(const Drawable::Ptr node)
{
	auto iterator = _mapParts.begin(), iEnd = _mapParts.end();
	while (iterator != iEnd)
	{
		const MapPart currentPart = (*iterator);	

		if (currentPart.node == node)
		{
			_mapParts.erase(iterator);
			return;
		}

		iterator++;
	}
}

void MapProjector::RemoveMapPart(unsigned int uid)
{
	auto iterator = _mapParts.begin(), iEnd = _mapParts.end();
	while (iterator != iEnd)
	{
		const MapPart currentPart = (*iterator);	

		if (currentPart.uid == uid)
		{
			_mapParts.erase(iterator);
			return;
		}

		iterator++;
	}
}

void MapProjector::Update()
{
	SetScale(_viewScale);
}
