#include "GameSavesManager.h"

#include <cocos2d.h>

#include "World.h"
#include "Log.h"
#include "Vector2.h"
#include "MiscUtils.h"

#include <string>
#include "SqliteDataReader.h"
#include "SqliteConnection.h"

static const std::string USER_DATA_TABLE = "user_data";
static const std::string CELLS_TABLE = "cells";
static const std::string RUNNED_TASKS_TABLE = "runned_tasks";
static const std::string PROCESSES_TABLE = "cell_processes";
static const std::string INVESIGATIONS_TABLE = "investigations";

struct GameSavesManagerImpl
{
public:
	GameSavesManagerImpl(const std::string& filePath)
		:database(filePath)
	{
	}

public:
	SqliteConnection database;
};

GameSavesManager::GameSavesManager()
{
	std::string dbPath = cocos2d::FileUtils::getInstance()->getWritablePath();
	dbPath.append("userdata.db");
	_impl = new GameSavesManagerImpl(dbPath);

	MessageManager::Instance().RegisterReceiver(this);

	FirstInitSave();
}

GameSavesManager::~GameSavesManager()
{
	MessageManager::Instance().UnregisterReceiver(this);
	delete _impl;
}

GameSavesManager& GameSavesManager::Instance()
{
	static GameSavesManager singleInstance;
	return singleInstance;
}

void GameSavesManager::AcceptMessage(const Message &msg)
{
	if (msg.name == "SaveGame")
	{
		SaveGameState();
	}
}

void GameSavesManager::FirstInitSave()
{
	if (!_impl->database.IsTableExists(USER_DATA_TABLE))
	{
		_impl->database.execSql("CREATE TABLE " + USER_DATA_TABLE + " ("
								"last_uid INTEGER NOT NULL"
								",root_cell INTEGER NOT NULL"
								",tutorial_state VARCHAR(255) NOT NULL"
								");");
	}
	else
	{
		World::Instance().SetFirstLaunch(false);
	}

	if (!_impl->database.IsTableExists(CELLS_TABLE))
	{
		_impl->database.execSql("CREATE TABLE " + CELLS_TABLE + " ("
								"'uid' INTEGER NOT NULL"
								",'state' VARCHAR(100) NOT NULL"
								",'parent_uid' INTEGER DEFAULT (-1)"
								",'town' VARCHAR(255) NOT NULL"
								",'location_x' REAL NOT NULL"
								",'location_y' REAL NOT NULL"
								",'cash' INTEGER NOT NULL DEFAULT (0)"
								",'morale' REAL NOT NULL"
								",'members_count' INTEGER NOT NULL"
								",'devotion' REAL NOT NULL"
								",'fame' REAL NOT NULL"
								",'rats_count' INTEGER NOT NULL"
								",'tech_units_count' INTEGER NOT NULL"
								",'town_heart_pounding' REAL NOT NULL"
								",'town_influence' REAL NOT NULL"
								",'towns_welfare' REAL NOT NULL"
								",'experience' REAL NOT NULL"
								");");
	}

	if (!_impl->database.IsTableExists(RUNNED_TASKS_TABLE))
	{
		_impl->database.execSql("CREATE TABLE " + RUNNED_TASKS_TABLE + " ("
								"'task_id' VARCHAR(255) NOT NULL"
								",'cell_uid' INTEGER NOT NULL"
								",'start_time' VARCHAR(20) NOT NULL"
								");");
	}

	if (!_impl->database.IsTableExists(PROCESSES_TABLE))
	{
		_impl->database.execSql("CREATE TABLE " + PROCESSES_TABLE + " ("
								"'cell_uid' INTEGER NOT NULL"
								",'type' VARCHAR(100) NOT NULL"
								",'begin_time' VARCHAR(20) NOT NULL"
								",'duration' VARCHAR(20) NOT NULL"
								");");
	}
}

namespace std
{
	string to_string(Cell::State __val)
	{
		switch (__val) {
		case Cell::State::READY:
			return "READY";
		case Cell::State::CONSTRUCTION:
			return "CONSTRUCTION";
		case Cell::State::DESTRUCTION:
			return "DESTRUCTION";
		case Cell::State::ARRESTED:
			return "ARRESTED";
		case Cell::State::AUTONOMY:
			return "AUTONOMY";
		default:
			return "";
		}
	}
}

Cell::State CastCellStateFromString(const std::string& state)
{
	if (state == "READY")
	{
		return Cell::State::READY;
	}
	else if (state == "CONSTRUCTION")
	{
		return Cell::State::CONSTRUCTION;
	}
	else if (state == "DESTRUCTION")
	{
		return Cell::State::DESTRUCTION;
	}
	else if (state == "ARRESTED")
	{
		return Cell::State::ARRESTED;
	}
	else if (state == "AUTONOMY")
	{
		return Cell::State::AUTONOMY;
	}
	else
	{
		return Cell::State::READY;
	}
}

void GameSavesManager::LoadProcesses()
{
	CellsNetwork &cellsNetwork = World::Instance().GetCellsNetwork();
	SqliteDataReader::Ptr processesReader = _impl->database.execQuery("SELECT * FROM " + PROCESSES_TABLE);
	while (processesReader->next())
	{
		unsigned int cell_uid = processesReader->getValueByName("cell_uid")->asInt();
		Cell::Ptr cell = cellsNetwork.GetCellByUid(cell_uid);

		Cell::Info &info = cell->GetInfo();

		if (std::to_string(info.state) == processesReader->getValueByName("type")->asString())
		{
			info.stateBegin = Utils::StringToTime(processesReader->getValueByName("begin_time")->asString());
			info.stateDuration = Utils::StringToTime(processesReader->getValueByName("duration")->asString());
		}
		else
		{
			Log::Instance().writeWarning("Loaded old unusual process for a cell. Process type: "
				+ processesReader->getValueByName("type")->asString());
		}
	}
}

void GameSavesManager::LoadRunnedTasks()
{
	CellsNetwork &cellsNetwork = World::Instance().GetCellsNetwork();
	SqliteDataReader::Ptr tasksReader = _impl->database.execQuery("SELECT * FROM " + RUNNED_TASKS_TABLE);
	while (tasksReader->next())
	{
		std::string currentTaskId = tasksReader->getValueByName("task_id")->asString();
		unsigned int cell_uid = tasksReader->getValueByName("cell_uid")->asInt();
		Utils::GameTime startTime = Utils::StringToTime(tasksReader->getValueByName("start_time")->asString());

		Cell::Ptr cell = cellsNetwork.GetCellByUid(cell_uid);
		World::Instance().GetTaskManager().RunTask(cell, currentTaskId, startTime);
	}
}

void GameSavesManager::LoadCellsState()
{
	std::map<int, std::pair<Cell::Ptr, int>> cellsIndicesCast;
	CellsNetwork &cellsNetwork = World::Instance().GetCellsNetwork();

	SqliteDataReader::Ptr cellsReader = _impl->database.execQuery("SELECT * FROM " + CELLS_TABLE);
	bool isFirstLaunch = true;
	while (cellsReader->next())
	{
		isFirstLaunch = false;
		Cell::Info info;
		int uid = cellsReader->getValueByName("uid")->asInt();
		int parentId = cellsReader->getValueByName("parent_uid")->asInt();
		info.cash = cellsReader->getValueByName("cash")->asInt();
		info.devotion = cellsReader->getValueByName("devotion")->asFloat();
		info.fame = cellsReader->getValueByName("fame")->asFloat();
		info.experience = cellsReader->getValueByName("experience")->asInt();
		info.location.x = cellsReader->getValueByName("location_x")->asFloat();
		info.location.y = cellsReader->getValueByName("location_y")->asFloat();
		info.morale = cellsReader->getValueByName("morale")->asFloat();
		info.membersCount = cellsReader->getValueByName("members_count")->asInt();
		info.parent = nullptr;
		info.ratsCount = cellsReader->getValueByName("rats_count")->asInt();
		info.specialization = Cell::Specialization::NORMAL;
		info.state = CastCellStateFromString(cellsReader->getValueByName("state")->asString());
		info.techUnitsCount = cellsReader->getValueByName("tech_units_count")->asInt();
		info.town = World::Instance().GetTownByName(cellsReader->getValueByName("town")->asString());
		info.townHeartPounding = cellsReader->getValueByName("town_heart_pounding")->asFloat();
		info.townInfluence = cellsReader->getValueByName("town_influence")->asFloat();
		info.townWelfare = cellsReader->getValueByName("towns_welfare")->asFloat();

		info.stateBegin = Utils::GetGameTime();
		info.stateDuration = 1;

		Cell::Ptr cell = std::make_shared<Cell>(Cell(info, uid));
		cellsNetwork.AppendCell(cell);
		cellsIndicesCast.insert(std::pair<int, std::pair<Cell::Ptr, int>>(uid, std::pair<Cell::Ptr, int>(cell, parentId)));
	}

	World::Instance().SetFirstLaunch(isFirstLaunch);

	// link cells by their id
	for (auto& cell : cellsIndicesCast)
	{
		if (cell.second.second != -1)
		{
			Cell::Ptr parentCell = (*cellsIndicesCast.find(cell.second.second)).second.first;
			parentCell->AddChild(cell.second.first);
		}
	}
}

void GameSavesManager::LoadUserInfo()
{
	SqliteDataReader::Ptr reader = _impl->database.execQuery("SELECT * FROM " + USER_DATA_TABLE + " LIMIT 1");
	if (reader->next())
	{
		World::Instance().InitUid(reader->getValueByName("last_uid")->asInt());

		Cell::Ptr rootCell = World::Instance().GetCellsNetwork().GetCellByUid(reader->getValueByName("root_cell")->asInt());
		World::Instance().GetCellsNetwork().SetRootCell(rootCell);

		World::Instance().SetTutorialState(reader->getValueByName("tutorial_state")->asString());
	}
}

void GameSavesManager::LoadGameState(void)
{
	LoadCellsState();
	LoadProcesses();
	LoadRunnedTasks();
	LoadUserInfo();
}

static std::string InitCellsAdditionStatement()
{
	return "INSERT INTO " + CELLS_TABLE +
			"(uid, state"
			",parent_uid"
			",town"
			",location_x"
			",location_y"
			",cash, morale"
			",members_count"
			",devotion"
			",fame"
			",rats_count"
			",tech_units_count"
			",town_heart_pounding"
			",town_influence"
			",towns_welfare"
			",experience)"
			"VALUES";
}

static std::string InitRunnedTasksAdditionStatement()
{
	return "DELETE FROM " + RUNNED_TASKS_TABLE + ";"
			"INSERT INTO " + RUNNED_TASKS_TABLE +
			"(task_id"
			",cell_uid"
			",start_time)"
			"VALUES";
}

static std::string InitConstrucrionAdditionStatement()
{
	return "DELETE FROM " + PROCESSES_TABLE + ";"
			"INSERT INTO " + PROCESSES_TABLE +
			"(cell_uid"
			",type, begin_time"
			",duration)"
			"VALUES";
}

static void AppendCellToQuery(std::string* const query, Cell* const cell)
{
	const Cell::Info info = cell->GetInfo();

	std::string parent_id = info.parent != nullptr ? std::to_string(info.parent->GetUid()) : "-1";

	query->append("(")
		.append(std::to_string(cell->GetUid())).append(",")
		.append("'").append(std::to_string(info.state)).append("',")
		.append(parent_id).append(",")
		.append("'").append(info.town.lock()->GetInfo().name).append("',")
		.append(std::to_string(info.location.x)).append(",")
		.append(std::to_string(info.location.y)).append(",")
		.append(std::to_string(info.cash)).append(",")
		.append(std::to_string(info.morale)).append(",")
		.append(std::to_string(info.membersCount)).append(",")
		.append(std::to_string(info.devotion)).append(",")
		.append(std::to_string(info.fame)).append(",")
		.append(std::to_string(info.ratsCount)).append(",")
		.append(std::to_string(info.techUnitsCount)).append(",")
		.append(std::to_string(info.townHeartPounding)).append(",")
		.append(std::to_string(info.townInfluence)).append(",")
		.append(std::to_string(info.townWelfare)).append(",")
		.append(std::to_string(info.experience))
		.append(")");
}

static void AppendTaskToQuery(std::string* const query, Task::Ptr task, int cellIndex)
{
	std::string currentTaskId = task->GetInfo()->id;
	std::string taskStartTime = Utils::TimeToString(task->GetStartTime());

	query->append("(")
		.append("'").append(currentTaskId).append("',")
		.append("'").append(std::to_string(cellIndex)).append("',")
		.append("'").append(taskStartTime).append("'")
		.append(")");
}

static void AppendCellProcessToQuery(std::string* const query, Cell* cell)
{
	const Cell::Info info = cell->GetInfo();

	query->append("(")
		.append("'").append(std::to_string(cell->GetUid())).append("',")
		.append("'").append(std::to_string(info.state)).append("',")
		.append("'").append(Utils::TimeToString(info.stateBegin)).append("',")
		.append("'").append(Utils::TimeToString(info.stateDuration)).append("'")
		.append(")");
}

static void AppendSeparator(std::string* const taskAdditionSqlStatement, bool addTasks)
{
	if (addTasks)
	{
		taskAdditionSqlStatement->append(",");
	}
	else
	{
		addTasks = true;
	}
}

static bool FillProcessesAdditionStatement(std::string* const processesSqlStatement)
{
	const CellsNetwork::Cells &cells = World::Instance().GetCellsNetwork().GetActiveCells();
	bool addProcesses = false;
	for (auto& cell : cells)
	{
		if (cell->IsInTemporaryState())
		{
			AppendSeparator(processesSqlStatement, addProcesses);
			addProcesses = true;

			AppendCellProcessToQuery(processesSqlStatement, cell.get());
		}
	}
	processesSqlStatement->append(";");

	return addProcesses;
}

static bool FillRunnedTasksAdditionStatement(std::string* const taskAdditionSqlStatement)
{
	const CellsNetwork::Cells &cells = World::Instance().GetCellsNetwork().GetActiveCells();
	bool addTasks = false;
	for (auto& cell : cells)
	{
		if (cell->IsCurrentTaskExists())
		{
			AppendSeparator(taskAdditionSqlStatement, addTasks);
			addTasks = true;

			Task::Ptr task = cell->getCurrentTask().lock();
			AppendTaskToQuery(taskAdditionSqlStatement, task, cell->GetUid());
		}
	}
	taskAdditionSqlStatement->append(";");

	return addTasks;
}

static bool FillCellsAdditionStatement(std::string* const cellsAdditionSqlStatement)
{
	const CellsNetwork::Cells &cells = World::Instance().GetCellsNetwork().GetActiveCells();
	bool addCells = false;
	for (auto& cell : cells)
	{
		AppendSeparator(cellsAdditionSqlStatement, addCells);
		addCells = true;

		AppendCellToQuery(cellsAdditionSqlStatement, cell.get());
	}
	cellsAdditionSqlStatement->append(";");

	return addCells;
}

static std::string GetUserInfoAdditionStatement()
{
	World &world = World::Instance();
	return std::string("INSERT INTO ").append(USER_DATA_TABLE).append(" (last_uid, root_cell, tutorial_state) VALUES (")
			.append(std::to_string(world.GetLastUid())).append(",")
			.append(std::to_string(world.GetCellsNetwork().GetRootCell().lock()->GetUid())).append(",")
			.append("'").append(world.GetTutorialState()).append("');");
}

void GameSavesManager::SaveGameState(void)
{
	std::string cellsAdditionSqlStatement = InitCellsAdditionStatement();
	bool addCells = FillCellsAdditionStatement(&cellsAdditionSqlStatement);

	std::string taskAdditionSqlStatement = InitRunnedTasksAdditionStatement();
	bool addTasks = FillRunnedTasksAdditionStatement(&taskAdditionSqlStatement);

	std::string processesSqlStatement = InitConstrucrionAdditionStatement();
	bool addProcesses = FillProcessesAdditionStatement(&processesSqlStatement);

	std::string userInfoSqlStatement = GetUserInfoAdditionStatement();

	// begining transaction
	_impl->database.execSql("BEGIN;");
	// clearing tables
	_impl->database.execSql("DELETE FROM " + CELLS_TABLE + ";"
							"DELETE FROM " + RUNNED_TASKS_TABLE + ";"
							"DELETE FROM " + PROCESSES_TABLE + ";"
							"DELETE FROM " + USER_DATA_TABLE + ";");
	// adding new data
	if (addCells) _impl->database.execSql(cellsAdditionSqlStatement);
	if (addTasks) _impl->database.execSql(taskAdditionSqlStatement);
	if (addProcesses) _impl->database.execSql(processesSqlStatement);
	_impl->database.execSql(userInfoSqlStatement);
	// commiting transaction
	_impl->database.execSql("COMMIT;");
}
