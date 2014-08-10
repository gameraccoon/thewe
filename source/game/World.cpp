#include "World.h"

World::World()
	: _worldTime(0.0f)
	, isGamePaused(false)
{
}

World::~World()
{
}

World& World::Instance()
{
	static World singleWorldMap;
	return singleWorldMap;
}

void World::CleanupMapContent(void)
{
	_regions.clear();
	_towns.clear();
}

void World::AddRegion(Region::Ptr region)
{
	_regions.push_back(region);
}

void World::AddCell(Cell::Ptr cell)
{
	_cells.push_back(cell);
}

void World::AddTown(Town::Ptr cell)
{
	_towns.push_back(cell);
}

const Region::Ptr World::GetRegionByName(const std::string name) const
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

const World::Regions& World::GetRegions() const
{
	return _regions;
}

const World::Cells& World::GetCells() const
{
	return _cells;
}

const World::Towns& World::GetTowns() const
{
	return _towns;
}

float World::GetWorldTime() const
{
	return _worldTime;
}

void World::Update(float deltatime)
{
	if (!isGamePaused)
	{
		_worldTime += deltatime;

		for (Cell::Ptr cell : _cells)
		{
			cell->Update(deltatime);
		}
	}
}

void World::SetPause(bool pause)
{
	isGamePaused = pause;
}