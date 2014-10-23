#ifndef WORLD_H
#define WORLD_H

#include <string>
#include <queue>

#include "Region.h"
#include "Cell.h"
#include "Town.h"
#include "Investigator.h"
#include "MiscUtils.h"
#include "LuaInstance.h"
#include "TaskManager.h"
#include "MessageManager.h"
#include "Tutorial.h"

class World
{
public:
	
	typedef std::vector<Region::Ptr> Regions;
	typedef std::vector<Cell::Ptr> Cells;
	typedef std::vector<Town::Ptr> Towns;
	typedef std::vector<Investigator::Ptr> Investigators;

public:
	/**
	 * Returns the single instance of the class
	 */
	static World& Instance();

	TaskManager& GetTaskManager();
	MessageManager& GetMessageManager();

	/** Removes all the regions */
	void CleanupMapContent(void);
	void InitLuaContext();
	void StartLogic();

	const Regions& GetRegions() const;
	const Cells& GetCells() const;
	const Towns& GetTowns() const;
	const Investigators& GetInvestigators(void) const;

	void AddRegion(Region::Ptr region);
	void AddCell(Cell::Ptr cell);
	void AddTown(Town::Ptr town);
	void AddInvestigator(Investigator::Ptr investigator);
	void AddInvestigatorByCell(Cell::Ptr investigationRoot);
	void AddInvestigatorByInfo(const Cell::Info &cellInfo);

	bool RemoveInvestigator(Investigator::Ptr investigator);

	const Region::WeakPtr GetRegionByName(const std::string &name) const;
	const Town::WeakPtr GetTownByName(const std::string &name) const;
	const Cell::WeakPtr GetCellByInfo(const Cell::Info &info) const;
	const Cell::WeakPtr GetRootCell(void) const;
	int GetCellsCount() const;

	void Update();

	void SetPause(bool pause);
	void SetFirstLaunch(bool newGame);
	void SetGameOver(bool over = true);

	float GetWorldCapturingState();

	bool IsFirstLaunch(void) const;
	bool IsGameOver(void) const;
	bool IsTownAvaliableToPlaceCell(Town::WeakPtr town) const;

	unsigned int GetNewUid(void) const;

	LuaInstance* GetLuaInst(void) const;

	void AddTutorial(Tutorial tutrorial);
	bool IsHaveTutorial();
	Tutorial::WeakPtr GetCurrentTutorial();
	void RemoveCurrentTutorial();
	std::string GetTutorialState();
	void SetTutorialState(const std::string& state);
	void RunTutorialFunction(const std::string& function);

	// calculated parameters of cells
	int GetExperienceForLevel(int level) const;
	int GetLevelFromExperience(int experience) const;
	float GetCellPursuedLevel(const Cell* cell) const;
private:
	void CalcWorldCapturingState();

private:
	TaskManager _taskManager;
	MessageManager _messageManager;

	Regions _regions;
	Cells _cells;
	Towns _towns;
	Investigators _investigators;

	/** Context for running lua-scripts */
	LuaInstance* _luaScript;

	bool _isGamePaused;
	bool _isFirstLaunch;
	bool _isLuaInited;
	bool _isGameOver;
	float _worldCapturingState;

	std::queue<Tutorial::Ptr> _tutorials;
	std::string _tutorialState;

	/*
	 * Turn off useless operations
	 */
	World();
	~World();
	World(const World&);
	void operator=(const World&);
};

#endif // WORLD_MAP_H
