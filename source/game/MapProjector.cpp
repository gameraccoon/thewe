#include "MapProjector.h"

static const Point MAP_INITIAL_SIZE = Point(1390.0f, 1003.0f);

MapProjector::MapProjector(Point mapSize)
	: _mapSize(mapSize)
	, _viewScale(1.0f)
	, _viewLocation(0.0f, 0.0f)
{
}

void MapProjector::SetLocation(Point worldLocation)
{
	_viewLocation = worldLocation;

	// исправляем если нужно выходы за границы
	_CheckBoundings();
	// обновляем положения спрайтов
	_UpdateNodes();
}

void MapProjector::SetScale(float scale)
{
	// если выд выходит за границы карты
	if (scale < (_screenCenter.y * 2) / _mapSize.y || scale < (_screenCenter.x * 2) / _mapSize.x)
	{
		// равняем его по самой узкой границе
		_viewScale = 2 * std::max(_screenCenter.y / _mapSize.y, _screenCenter.x / _mapSize.x);
	}
	else
	{
		_viewScale = scale;
	}
	
	// исправляем если нужно выходы за границы
	_CheckBoundings();
	// обновляем положения спрайтов
	_UpdateNodes();
}

void MapProjector::ShiftView(Point delta)
{
	SetLocation(_viewLocation + delta / _viewScale);
}

void MapProjector::_CheckBoundings()
{
	// выход за верхнюю границу
	if (_viewLocation.y < _screenCenter.y / _viewScale - _mapSize.y/2)
	{
		_viewLocation.y = _screenCenter.y / _viewScale - _mapSize.y/2;
	}

	// выход за нижнюю границу
	if (_viewLocation.y > -_screenCenter.y / _viewScale + _mapSize.y/2)
	{
		_viewLocation.y = -_screenCenter.y / _viewScale + _mapSize.y/2;
	}

	// выход за правую границу
	if (_viewLocation.x < _screenCenter.x / _viewScale - _mapSize.x/2)
	{
		_viewLocation.x = _screenCenter.x / _viewScale - _mapSize.x/2;
	}

	// выход за лувую границу
	if (_viewLocation.x > -_screenCenter.x / _viewScale + _mapSize.x/2)
	{
		_viewLocation.x = -_screenCenter.x / _viewScale + _mapSize.x/2;
	}
}

Point MapProjector::GetLocation() const
{
	return _viewLocation;
}

float MapProjector::GetScale() const
{
	return _viewScale;
}

Point MapProjector::ProjectOnMap(Point screenPoint) const
{
	return (screenPoint - _screenCenter) / _viewScale - _viewLocation;
}

Point MapProjector::ProjectOnScreen(Point mapPoint) const
{
	return _screenCenter + (mapPoint + _viewLocation) * _viewScale;
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

ArbitraryHull MapProjector::ProjectOnScreen(const ArbitraryHull& mapHull) const
{
	ArbitraryHull projectedHull;
	for (auto &point : mapHull._pointsArray)
	{
		projectedHull.PushPoint(ProjectOnScreen(point));
	}
	return projectedHull;
}

void MapProjector::SetScreenCenter(Point centerPos)
{
	_screenCenter = centerPos;	
}

void MapProjector::AddMapPart(Drawable::Ptr node, Point location, Point shift, float scale, bool dontScale)
{
	MapPart locSprite;
	locSprite.location = location;
	locSprite.initialScale = scale;
	locSprite.shift = shift;
	locSprite.node = node;
	locSprite.isScalable = !dontScale;
	_mapParts.push_back(locSprite);
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

		node.node->SetPosition(_screenCenter + (node.location + _viewLocation) * _viewScale + node.shift * screenScale);
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

void MapProjector::Update()
{
	SetScale(_viewScale);
}