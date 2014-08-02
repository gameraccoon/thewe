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

	CheckBoundings();

	UpdateNodes();
}

void MapProjector::SetScale(float scale)
{
	if (scale < (_screenCenter.y * 2) / _mapSize.y)
	{
		_viewScale = (_screenCenter.y * 2) / _mapSize.y;
	}
	else
	{
		_viewScale = scale;
	}
	
	CheckBoundings();
	UpdateNodes();
}

void MapProjector::ShiftView(Point delta)
{
	SetLocation(_viewLocation + delta / _viewScale);
}

void MapProjector::CheckBoundings()
{
	if (_viewLocation.y < _screenCenter.y / _viewScale - _mapSize.y/2)
	{
		_viewLocation.y = _screenCenter.y / _viewScale - _mapSize.y/2;
	}

	if (_viewLocation.y > -_screenCenter.y / _viewScale + _mapSize.y/2)
	{
		_viewLocation.y = -_screenCenter.y / _viewScale + _mapSize.y/2;
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

void MapProjector::AddMapPart(Point location, Point shift, cocos2d::CCNode *node)
{
	// умный указатель вместо delete будет вызывать release
	std::function<void(cocos2d::CCNode* node)> del = [](cocos2d::CCNode* nodeToDelete)
	{
		nodeToDelete->release();
	};

	MapPart::NodePtr nodePtr(node, del);
	
	MapPart locSprite;
	locSprite.location = location;
	locSprite.shift = shift;
	locSprite.node.swap(nodePtr);
	_mapParts.push_back(locSprite);
}

void MapProjector::UpdateNodes()
{
	for (const MapPart& node : _mapParts)
	{
		node.node->setPosition(_screenCenter + ((node.location + node.shift) + _viewLocation) * _viewScale);
		node.node->setScale(_viewScale);
	}
}

cocos2d::CCSprite* MapProjector::AddSprite(Point location, Point shift, std::string spriteName)
{
	cocos2d::CCSprite *sprite = new cocos2d::CCSprite();
	sprite->initWithFile(spriteName.c_str());
	AddMapPart(Point(0.0f, 0.0f), Point(0.0f, 0.0f), sprite);
	return sprite;
}

void MapProjector::RemoveMapPart(const cocos2d::CCNode *node)
{
	auto iterator = _mapParts.begin(), iEnd = _mapParts.end();
	while (iterator != iEnd)
	{
		const MapPart currentPart = (*iterator);	

		if (currentPart.node.get() == node)
		{
			_mapParts.erase(iterator);
			return;
		}

		iterator++;
	}
}