#include "World.h"

#include "TaskManager.h"
#include "MessageManager.h"
#include "Log.h"
#include "LuaInstance.h"
#include "GameInfo.h"
#include "World.h"

#include <cocos2d.h>
#include <luabind/luabind.hpp>

World::World()
	: _worldTime(0.0f)
	, _isGamePaused(false)
	, _isFirstLaunch(true)
	, _isLuaInited(false)
	, _isGameOver(false)
{
	_luaScript = new LuaInstance();
}

World::~World()
{
	delete _luaScript;
}

World& World::Instance()
{
	static World singleWorldMap;
	return singleWorldMap;
}

void World::InitLuaContext()
{
	if (!_isLuaInited)
	{
		_isLuaInited = true;
		std::string fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename("tasks.lua");

		_luaScript->BindClass<Log>();
		_luaScript->BindClass<MessageManager>();
		_luaScript->BindClass<GameInfo>();
		_luaScript->BindClass<World>();
		_luaScript->BindClass<Cell::Info>();
		_luaScript->BindClass<const Task::Info>();
		_luaScript->BindClass<Vector2>();

		_luaScript->RegisterVariable("Log", &(Log::Instance()));
		_luaScript->RegisterVariable("MessageManager", &(MessageManager::Instance()));
		_luaScript->RegisterVariable("GameInfo", &(GameInfo::Instance()));
		_luaScript->RegisterVariable("World", &(World::Instance()));

		std::string script = cocos2d::FileUtils::getInstance()->getStringFromFile(fullPath);
		_luaScript->ExecScript(script.c_str());
	}
	else
	{
		Log::Instance().writeWarning("Trying to init Lua context twice");
	}
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

void World::AddInvestigatorByCell(Cell::Ptr investigationRoot)
{
	Investigator::Ptr investigator = Investigator::Create((Cell::WeakPtr)investigationRoot);
	investigator->BeginInvestigation();

	_investigators.push_back(investigator);
}

void World::AddInvestigatorByInfo(const Cell::Info &cellInfo)
{
	Cell::WeakPtr cell = GetCellByInfo(cellInfo);
	if (!cell.expired())
	{
		AddInvestigatorByCell(cell.lock());
	}
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

const Cell::WeakPtr World::GetCellByInfo(const Cell::Info &info) const
{
	Cell *parent = info.parent;
	Town::Ptr town = info.town.lock();

	for (Cell::Ptr cell : _cells)
	{
		if (cell->GetInfo().parent == parent && cell->GetInfo().town.lock() == town)
		{
			return cell;
		}
	}

	return Cell::WeakPtr();
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

const World::Investigators& World::GetInvestigators(void) const
{
	return _investigators;
}

Utils::GameTime World::GetWorldTime() const
{
	return _worldTime;
}

void World::Update()
{
	Utils::GameTime time = Utils::GetGameTime();

	for (Investigator::Ptr investigator : _investigators)
	{
		investigator->UpdateToTime(time);
	}

	TaskManager::Instance().UpdateToTime(time);
}

void World::SetPause(bool pause)
{
	_isGamePaused = pause;
}

void World::SetFirstLaunch(bool newGame)
{
	_isFirstLaunch = newGame;
}

void World::SetGameOver(bool over)
{
	_isGameOver = over;
}

void World::InitWorldTime(Utils::GameTime worldTime)
{
	// we must initialize it only once
	if (_worldTime <= 0.0f)
	{
		_worldTime = worldTime;
	}
}

bool World::IsFirstLaunch(void) const
{
	return _isFirstLaunch;
}

bool World::IsGameOver(void) const
{
	return _isGameOver;
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

LuaInstance* World::GetLuaInst(void) const
{
	return _luaScript;
}
