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

#include <luabind/luabind.hpp>

World::World()
	: _isGamePaused(false)
	, _isFirstLaunch(true)
	, _isLuaInited(false)
	, _isGameOver(false)
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

TutorialManager& World::GetTutorialManager()
{
	return _tutorialManager;
}

void World::InitLuaContext(ResourceCacheQueue<std::string>::Ptr cachedScripts)
{
	if (!_isLuaInited)
	{
		_isLuaInited = true;

		lua::BindGameClasses(_luaScript);
		lua::BindFunctions(_luaScript);
		lua::BindGlobalData(_luaScript);

		while (!cachedScripts->IsEmpty())
		{
			_luaScript->ExecScript(cachedScripts->PopFrontResource().c_str());
		}
	}
	else
	{
		WRITE_WARN("Trying to init Lua context twice");
	}
}

void World::StartLogic()
{
	luabind::call_function<void>(World::Instance().GetLuaInst()->GetLuaState()
		, "StartGame"
		, 0);

	_bonusOnMap.OnStartLogic();
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
	_investigators.push_back(investigator);

	Message message("AddInvestigatorWidget");
	message.variables.SetInt("UID", investigator->GetUid());
	MessageManager::Instance().PutMessage(message);
}

void World::AddInvestigatorByCell(Cell::WeakPtr investigationRoot)
{
	Investigator::Ptr investigator = Investigator::Create((Cell::WeakPtr)investigationRoot);
	investigator->BeginCatchTime();

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
			if (GetTutorialManager().IsTutorialStateAvailable("WaitForUncatchedInvestigator"))
			{
				World::Instance().GetTutorialManager().RemoveCurrentTutorial();
			}
			else if (GetTutorialManager().IsTutorialStateAvailable("WaitForCatchUncatchedInvestigator"))
			{
				World::Instance().GetTutorialManager().RemoveCurrentTutorial();
				GetTutorialManager().RunTutorialFunction("FirstUncatchedInvestigatorCatched");
			}

			Message message("DeleteInvestigatorWidget");
			message.variables.SetInt("UID", investigator->GetUid());
			MessageManager::Instance().PutMessage(message);
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
	_bonusOnMap.UpdateToTime(_currentTime);

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

Utils::GameTime World::GetGameTime() const
{
	return _currentTime;
}

void World::InitTime(Utils::GameTime time)
{
	if (_currentTime != 0)
	{
		WRITE_WARN("Trying to init time twice");
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
		if (cell->GetTown().lock()->GetName() == town.lock()->GetName())
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
		WRITE_WARN("Trying to double initialize UId");
	}
}

LuaInstance* World::GetLuaInst(void) const
{
	return _luaScript;
}

int World::GetExperienceForLevel(int level) const
{
	return luabind::call_function<int>(_luaScript->GetLuaState()
		, "ExperienceForLevel"
		, level
		, 0);
}

int World::GetLevelFromExperience(int experience) const
{
	return luabind::call_function<int>(_luaScript->GetLuaState()
		, "LevelFromExperience"
		, experience
		, 0);
}

float World::GetCellPursuedLevel(Cell* cell) const
{
	return luabind::call_function<float>(_luaScript->GetLuaState()
		, "CalcCellPursuedLevel"
		, cell
		, 0);
}

std::function<void()> World::GetBonusCallback(Cell::WeakPtr cell) const
{
	return std::function<void()>([cell, this](){
		luabind::call_function<void>(_luaScript->GetLuaState()
			, "BonusBehavior"
			, cell.lock().get());
	});
}
