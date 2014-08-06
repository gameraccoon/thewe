#include "WorldMap.h"

WorldMap::WorldMap()
{
}

WorldMap::~WorldMap()
{
}

WorldMap& WorldMap::Instance()
{
	static WorldMap singleWorldMap;
	return singleWorldMap;
}

void WorldMap::CleanupRegions(void)
{
	_regions.clear();
}

void WorldMap::AddRegion(Region::Ptr region)
{
	_regions.push_back(region);
}

void WorldMap::AddCell(Cell::Ptr cell)
{
	_cells.push_back(cell);
}

const Region::Ptr WorldMap::GetRegionByName(const std::string name) const
{
	for (Region::Ptr region : _regions)
	{
		if (region->GetInfo().name == name)
		{
			return region;
		}
	}

	return nullptr;
}

const WorldMap::Regions& WorldMap::GetRegions() const
{
	return _regions;
}

const WorldMap::Cells& WorldMap::GetCells() const
{
	return _cells;
}
