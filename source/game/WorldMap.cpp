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

void WorldMap::CleanupRegions(void)
{
	_regions.clear();
}

void WorldMap::AddRegion(Region::Ptr region)
{
	_regions.push_back(region);
}

void WorldMap::CreateRegion(std::string regionName)
{
	_regions.push_back(std::make_shared<Region>(Region()));
}

void WorldMap::AddCell(Cell::Ptr cell)
{
	_cells.push_back(cell);
}

const WorldMap::Regions& WorldMap::GetRegions() const
{
	return _regions;
}

const WorldMap::Cells& WorldMap::GetCells() const
{
	return _cells;
}
