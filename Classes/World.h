#ifndef WORLD_H
#define WORLD_H

#include <string>
#include <queue>
#include <set>
#include <functional>

#include "Region.h"
#include "Cell.h"
#include "Town.h"
#include "Investigator.h"
#include "CellsNetwork.h"
#include "TaskManager.h"
#include "BonusOnMap.h"
#include "NotificationMessageManager.h"
#include "TutorialManager.h"
#include "LuaInstance.h"
#include "MiscUtils.h"

class World
{
public:
	
	typedef std::vector<Region::Ptr> Regions;
	typedef std::vector<Town::Ptr> Towns;
	typedef std::vector<Investigator::Ptr> Investigators;

public:
	/**
	 * Returns the single instance of the class
	 */
	static World& Instance();

	TaskManager& GetTaskManager();
	CellsNetwork& GetCellsNetwork();
	NotificationMessageManager& GetMessageManager();
	TutorialManager& GetTutorialManager();

	/** Removes all the regions */
	void CleanupMapContent(void);
	void InitLuaContext();
	void StartLogic();

	const Regions& GetRegions() const;
	const Towns& GetTowns() const;
	const Investigators& GetInvestigators(void) const;

	void AddRegion(Region::Ptr region);
	void AddTown(Town::Ptr town);
	void AddInvestigator(Investigator::Ptr investigator);
	void AddInvestigatorByCell(Cell::WeakPtr investigationRoot);
	void AddInvestigatorByCellUid(unsigned int celluid);

	void RemoveCellFromInvestigation(Cell::Ptr cell);
	bool RemoveInvestigator(Investigator::Ptr investigator);
	
	Investigator::Ptr GetInvestigatorByUid(int uid);

	const Region::WeakPtr GetRegionByName(const std::string &name) const;
	const Town::WeakPtr GetTownByName(const std::string &name) const;

	void Update(float deltaTime);
	Utils::GameTime GetTimeCurrent() const;
	void InitTime(Utils::GameTime time);

	void SetPause(bool pause);
	void SetFirstLaunch(bool newGame);
	void SetGameOver(bool over = true);

	float GetWorldCapturingState();

	bool IsFirstLaunch(void) const;
	bool IsGameOver(void) const;
	bool IsTownAvaliableToPlaceCell(Town::WeakPtr town) const;
	bool IsCellUnderInvestigation(Cell::Ptr cell) const;

	unsigned int GetNewUid(void);
	unsigned int GetLastUid(void) const;
	void InitUid(unsigned int uid);

	LuaInstance* GetLuaInst(void) const;

	// calculated parameters of cells
	int GetExperienceForLevel(int level) const;
	int GetLevelFromExperience(int experience) const;
	float GetCellPursuedLevel(Cell* cell) const;

	std::function<void()> GetBonusCallback(Cell::WeakPtr cell) const;

private:
	void CalcWorldCapturingState();

private:
	CellsNetwork _cellsNetwork;
	TaskManager _taskManager;
	NotificationMessageManager _messageManager;
	TutorialManager _tutorialManager;
	BonusOnMap _bonusOnMap;

	Regions _regions;
	Towns _towns;
	Investigators _investigators;

	/** Context for running lua-scripts */
	LuaInstance* _luaScript;

	bool _isGamePaused;
	bool _isFirstLaunch;
	bool _isLuaInited;
	bool _isGameOver;
	float _worldCapturingState;

	Utils::GameTime _currentTime;

	unsigned int _uid;

	/*
	 * Turn off useless operations
	 */
	World();
	~World();
	World(const World&);
	void operator=(const World&);
};

#endif // WORLD_MAP_H
