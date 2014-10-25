#include "World.h"

#include "TaskManager.h"
#include "NotificationMessageManager.h"
#include "Log.h"
#include "LuaInstance.h"
#include "GameInfo.h"
#include "World.h"
#include "WorldLoader.h"
#include "MessageManager.h"

#include <cocos2d.h>
#include <luabind/luabind.hpp>

World::World()
	: _isGamePaused(false)
	, _isFirstLaunch(true)
	, _isLuaInited(false)
	, _isGameOver(false)
	, _tutorialState("NotInitialized")
{
	_luaScript = new LuaInstance();
}

World::~World()
{
	delete _luaScript;
}

World& World::Instance()
{
	static World singleInstance;
	return singleInstance;
}

TaskManager& World::GetTaskManager()
{
	return _taskManager;
}

NotificationMessageManager& World::GetMessageManager()
{
	return _messageManager;
}

void ExecScript(LuaInstance* instance, std::string filename)
{
	std::string fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(filename);
	std::string script = cocos2d::FileUtils::getInstance()->getStringFromFile(fullPath);
	instance->ExecScript(script.c_str());
}

void World::InitLuaContext()
{
	if (!_isLuaInited)
	{
		_isLuaInited = true;

		_luaScript->BindClass<Log>();
		_luaScript->BindClass<NotificationMessageManager>();
		_luaScript->BindClass<GameInfo>();
		_luaScript->BindClass<World>();
		_luaScript->BindClass<Cell::Info>();
		_luaScript->BindClass<Cell>();
		_luaScript->BindClass<const Task::Info>();
		_luaScript->BindClass<Vector2>();
		_luaScript->BindClass<Tutorial>();

		_luaScript->RegisterVariable("Log", &(Log::Instance()));
		_luaScript->RegisterVariable("MessageManager", &(_messageManager));
		_luaScript->RegisterVariable("GameInfo", &(GameInfo::Instance()));
		_luaScript->RegisterVariable("World", &(World::Instance()));

		ExecScript(_luaScript, "mainLogic.lua");
		ExecScript(_luaScript, "tasks.lua");
		ExecScript(_luaScript, "tutorials.lua");
	}
	else
	{
		Log::Instance().writeWarning("Trying to init Lua context twice");
	}
}

void World::StartLogic()
{
	luabind::call_function<void>(World::Instance().GetLuaInst()->GetLuaState()
		, "StartGame"
		, 0);
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
	CalcWorldCapturingState();
	WorldLoader::Instance().RequestToSave();
}

void World::AddTown(Town::Ptr cell)
{
	_towns.push_back(cell);
}

void World::AddInvestigator(Investigator::Ptr investigator)
{
	if (World::Instance().GetTutorialState() == "WaitForFirstInvestigator")
	{
		investigator->BeginCatchTime(GameInfo::Instance().GetFloat("INVESTIGATOR_TUTORIAL_CATCH_TIME"));
		World::Instance().RunTutorialFunction("FirstInvestigationStarted");
	}

	_investigators.push_back(investigator);
	
	MessageManager::Instance().PutMessage(Message("AddInvestigatorWidget", investigator->GetUid()));
}

void World::AddInvestigatorByCell(Cell::Ptr investigationRoot)
{
	Investigator::Ptr investigator = Investigator::Create((Cell::WeakPtr)investigationRoot);
	investigator->BeginCatchTime(GameInfo::Instance().GetFloat("INVESTIGATOR_CATCH_TIME", 1.0f));

	AddInvestigator(investigator);

	WorldLoader::Instance().RequestToSave();
}

void World::AddInvestigatorByInfo(const Cell::Info &cellInfo)
{
	Cell::WeakPtr cell = GetCellByInfo(cellInfo);
	if (!cell.expired())
	{
		AddInvestigatorByCell(cell.lock());
	}
}

bool World::RemoveCell(Cell::Ptr cell)
{
	for (Cells::iterator it = _cells.begin(); it != _cells.end(); ++it)
	{
		if ((*it) == cell)
		{
			cell->GetInfo().town.lock()->SetCellPresented(false);

			Cell *parent = cell->GetInfo().parent;
			if (parent) {
				parent->RemoveChild(cell);
			}
			for (Cell::Ptr child : cell->GetChildren()) {
				child->SetParent(nullptr);
			}

			it = _cells.erase(it);
			return true;
		}
	}

	return false;
}

void World::RemoveCellFromInvestigation(Cell::Ptr cell)
{
	for (Investigator::Ptr investigator : _investigators) {
		investigator->CancleInvestigationTo(cell, investigator->GetRootBranchBundle());
	}
}

bool World::RemoveInvestigator(Investigator::Ptr investigator)
{
	for (Investigators::iterator it = _investigators.begin(); it != _investigators.end(); ++it)
	{
		if ((*it) == investigator)
		{
			MessageManager::Instance().PutMessage(Message("DeleteInvestigatorWidget", investigator->GetUid()));
			it = _investigators.erase(it);
			return true;
		}
	}

	return false;
}

Cell::Ptr World::GetCellByUid(int uid) const
{
	for (Cell::Ptr cell : _cells) {
		if (cell->GetUid() == uid) {
			return cell;
		}
	}
	return Cell::Ptr();
}

Investigator::Ptr World::GetInvestigatorByUid(int uid)
{
	for (Investigator::Ptr investigator : _investigators) {
		if (investigator->GetUid() == uid) {
			return investigator;
		}
	}
	return Investigator::Ptr();
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

int World::GetCellsCount() const
{
	return _cells.size();
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

void World::Update()
{
	Utils::GameTime time = Utils::GetGameTime();

	for (Investigator::Ptr investigator : _investigators)
	{
		investigator->UpdateToTime(time);
	}

	_taskManager.UpdateToTime(time);

	MessageManager::Instance().CallAcceptMessages();
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

float World::GetWorldCapturingState()
{
	return _worldCapturingState;
}

void World::CalcWorldCapturingState()
{
	int capturedTownsCount = 0;
	for (auto town : _towns)
	{
		if (town->IsCellPresented())
		{
			capturedTownsCount++;
		}
	}
	_worldCapturingState = (float)capturedTownsCount / _towns.size();
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

bool World::IsCellUnderInvestigation(Cell::Ptr cell) const
{
	for (Investigator::Ptr investigator : _investigators)
	{
		if (investigator->IsCellUnderInvestigation(cell, investigator->GetRootBranchBundle())){
			return true;
		}
	}

	return false;
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

void World::AddTutorial(Tutorial tutrorial)
{
	_tutorials.push(std::make_shared<Tutorial>(tutrorial));
}

bool World::IsHaveTutorial()
{
	return _tutorials.size() > 0;
}

Tutorial::WeakPtr World::GetCurrentTutorial()
{
	return _tutorials.front();
}

void World::RemoveCurrentTutorial()
{
	if (_tutorials.size() > 0)
	{
		if (!_tutorials.front()->luaCallback.empty())
		{
			luabind::call_function<void>(World::Instance().GetLuaInst()->GetLuaState()
				, _tutorials.front()->luaCallback.c_str()
				, 0);
		}

		_tutorials.pop();
	}
}

std::string World::GetTutorialState()
{
	return _tutorialState;
}

void World::SetTutorialState(const std::string& state)
{
	_tutorialState = state;
}

void World::RunTutorialFunction(const std::string& function)
{
	luabind::call_function<void>(_luaScript->GetLuaState()
		, std::string("RunTutorial_" + function).c_str()
		, 0);
}

int World::GetExperienceForLevel(int level) const
{
	return luabind::call_function<int>(_luaScript->GetLuaState()
		, std::string("ExperienceForLevel").c_str()
		, level
		, 0);
}

int World::GetLevelFromExperience(int experience) const
{
	return luabind::call_function<int>(_luaScript->GetLuaState()
		, std::string("LevelFromExperience").c_str()
		, experience
		, 0);
}

float World::GetCellPursuedLevel(Cell* cell) const
{
	return luabind::call_function<float>(_luaScript->GetLuaState()
		, std::string("CalcCellPursuedLevel").c_str()
		, cell
		, 0);
}
