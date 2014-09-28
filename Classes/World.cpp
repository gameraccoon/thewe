#include "World.h"

#include "TaskManager.h"

World::World()
	: _worldTime(0.0f)
	, _isGamePaused(false)
	, _isFirstLaunch(true)
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

const Region::WeakPtr World::GetRegionByName(const std::string &name) const
{
	for (Region::Ptr region : _regions)
	{
		if (region->GetInfo().name == name)
		{
			return region;
		}
	}

	return Region::WeakPtr();
}

const Town::WeakPtr World::GetTownByName(const std::string &name) const
{
	for (Town::Ptr town : _towns)
	{
		if (town->GetInfo().name == name)
		{
			return town;
		}
	}

	return Town::WeakPtr();
}

const Cell::WeakPtr World::GetRootCell(void) const
{
	for (Cell::Ptr cell : _cells)
	{
		if (cell->GetInfo().parent == nullptr)
		{
			return cell;
		}
	}

	return Cell::WeakPtr();
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
	if (!_isGamePaused)
	{
		_worldTime += deltatime;

		for (Cell::Ptr cell : _cells)
		{
			cell->Update(deltatime);
		}

		TaskManager::Instance().UpdateToTime(_worldTime);
	}
}

void World::SetPause(bool pause)
{
	_isGamePaused = pause;
}

void World::SetFirstLaunch(bool newGame)
{
	_isFirstLaunch = newGame;
}

bool World::IsFirstLaunch(void) const
{
	return _isFirstLaunch;
}

bool World::IsTownAvaliableToPlaceCell(Town::WeakPtr town) const
{
	for (Cell::Ptr cell : _cells)
	{
		if (cell->GetInfo().town.lock()->GetName() == town.lock()->GetName())
		{
			return false;
		}
	}

	return true;
}

unsigned int World::GetNewUid(void) const
{
	static unsigned int uid = 0;
	return uid++;
}