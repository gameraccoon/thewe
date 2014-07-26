#include "WorldMap.h"

WorldMap* WorldMap::_singleInstance = nullptr;

WorldMap::WorldMap()
{
}
WorldMap::~WorldMap()
{
}

WorldMap& WorldMap::Instance()
{
	if (WorldMap::_singleInstance == nullptr)
	{
		WorldMap::_singleInstance = new WorldMap();
	}

	return *WorldMap::_singleInstance;
}

Region::Ptr WorldMap::GetRegion(std::string regionName) const
{
	auto regionIterator = _regions.find(regionName);
	return (regionIterator != _regions.end()) ? regionIterator->second : Region::Ptr();
}

void WorldMap::AddRegion(std::string regionName, Region::Ptr region)
{
	_regions.insert(std::pair<std::string, Region::Ptr>(regionName, region));
}

const WorldMap::RegionsMap& WorldMap::GetRegions() const
{
	return _regions;
}

void WorldMap::CreateRegion(std::string regionName)
{
	_regions.insert(std::pair<std::string, Region::Ptr>(regionName, std::make_shared<Region>(Region())));
}