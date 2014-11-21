#include "World.h"

#include "TaskManager.h"
#include "NotificationMessageManager.h"
#include "Log.h"
#include "LuaBindings.h"
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
	, _uid(0)
	, _currentTime(0)
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

CellsNetwork& World::GetCellsNetwork()
{
	return _cellsNetwork;
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

		lua::BindGameClasses(_luaScript);
		lua::BindFunctions(_luaScript);
		lua::BindGlobalData(_luaScript);

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

void World::AddTown(Town::Ptr cell)
{
	_towns.push_back(cell);
}

void World::AddInvestigator(Investigator::Ptr investigator)
{
	if (World::Instance().GetTutorialState() == "WaitForFirstInvestigator")
	{
		investigator->BeginCatchTime(GameInfo::Instance().GetTime("INVESTIGATOR_TUTORIAL_CATCH_TIME"));
		World::Instance().RunTutorialFunction("FirstInvestigationStarted");
	}

	_investigators.push_back(investigator);
	
	MessageManager::Instance().PutMessage(Message("AddInvestigatorWidget", investigator->GetUid()));
}

void World::AddInvestigatorByCell(Cell::WeakPtr investigationRoot)
{
	Investigator::Ptr investigator = Investigator::Create((Cell::WeakPtr)investigationRoot);
	investigator->BeginCatchTime(GameInfo::Instance().GetTime("INVESTIGATOR_CATCH_TIME", 1.0f));

	AddInvestigator(investigator);

	MessageManager::Instance().PutMessage(Message("SaveGame"));
}

void World::AddInvestigatorByCellUid(unsigned int celluid)
{
	AddInvestigatorByCell(GetCellsNetwork().GetCellByUid(celluid));
}

void World::RemoveCellFromInvestigation(Cell::Ptr cell)
{
	for (Investigator::Ptr investigator : _investigators) {
		investigator->CancelInvestigationTo(cell);
	}
}

bool World::RemoveInvestigator(Investigator::Ptr investigator)
{
	for (Investigators::iterator it = _investigators.begin(); it != _investigators.end(); ++it)
	{
		if ((*it) == investigator)
		{
			if (GetTutorialState() == "WaitForCatchUncatchedInvestigator")
			{
				RunTutorialFunction("FirstUncatchedInvestigatorCatched");
			}

			MessageManager::Instance().PutMessage(Message("DeleteInvestigatorWidget", investigator->GetUid()));
			it = _investigators.erase(it);
			return true;
		}
	}

	return false;
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

const World::Regions& World::GetRegions() const
{
	return _regions;
}

const World::Towns& World::GetTowns() const
{
	return _towns;
}

const World::Investigators& World::GetInvestigators(void) const
{
	return _investigators;
}

void World::Update(float deltaTime)
{
	_currentTime += deltaTime;

	CalcWorldCapturingState();

	for (Investigator::Ptr investigator : _investigators)
	{
		investigator->UpdateToTime(_currentTime);
	}

	_taskManager.UpdateToTime(_currentTime);
	_cellsNetwork.UpdateToTime(_currentTime);

	MessageManager::Instance().CallAcceptMessages();

	if (!_cellsNetwork.GetRootCell().expired()) {
		if (!_cellsNetwork.GetRootCell().lock()->IsState(Cell::State::READY)) {
			SetGameOver();
		}
	}

	for (Investigators::iterator it = _investigators.begin(); it != _investigators.end(); ++it) {
		Investigator::Ptr ptr = (*it);
		if (ptr->IsStateType(Investigator::State::FINISHED))
		{
			RemoveInvestigator(ptr);
			break;
		}
	}
}

Utils::GameTime World::GetTimeCurrent() const
{
	return _currentTime;
}

void World::InitTime(Utils::GameTime time)
{
	if (_currentTime != 0)
	{
		Log::Instance().writeWarning("Trying to init time twice");
	}

	_currentTime = time;
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
	for (Cell::Ptr cell : _cellsNetwork.GetActiveCells())
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
	for (Investigator::Ptr investigator : _investigators) {
		if (investigator->IsCellUnderInvestigation(cell)) {
			return true;
		}
	}

	return false;
}

unsigned int World::GetNewUid(void)
{
	return _uid++;
}

unsigned int World::GetLastUid() const
{
	return _uid;
}

void World::InitUid(unsigned int uid)
{
	if (_uid == 0)
	{
		_uid = uid;
	}
	else
	{
		Log::Instance().writeWarning("Trying to double initialize UId");
	}
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
