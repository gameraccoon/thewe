#include "GameSavesManager.h"

#include <mutex>
#include <string>

#include "World.h"
#include "Log.h"
#include "Vector2.h"
#include "MiscUtils.h"

#include "SqliteDataReader.h"
#include "SqliteConnection.h"

static const std::string USER_DATA_TABLE = "user_data";
static const std::string CELLS_TABLE = "cells";
static const std::string RUNNED_TASKS_TABLE = "runned_tasks";
static const std::string PROCESSES_TABLE = "cell_processes";
static const std::string INVESIGATIONS_TABLE = "investigation_branches";
static const std::string INVESIGATORS_TABLE = "investigators";
static const std::string TUTORIAL_STATES_TABLE = "tutorial_states";
static const std::string CELLS_RESOURCES_TABLE = "cells_resources";

static std::recursive_mutex InstanceMutex;
static std::recursive_mutex SavesManagerMutex;


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
	:_isWorking(false)
{
	std::string dbPath = Utils::GetWritablePath();
	dbPath.append("userdata.db");
	_impl = new GameSavesManagerImpl(dbPath);

	FirstInitSave();

	MessageManager::Instance().RegisterReceiver(this, "SaveGame");
	MessageManager::Instance().RegisterReceiver(this, "SaveTime");
}

GameSavesManager::~GameSavesManager()
{
	delete _impl;
}

GameSavesManager& GameSavesManager::Instance()
{
	std::lock_guard<std::recursive_mutex> lock(::InstanceMutex);

	static GameSavesManager singleInstance;
	return singleInstance;
}

void GameSavesManager::AcceptMessage(const Message &msg)
{
	if (msg.is("SaveGame"))
	{
		SaveGameState();
	}
	else if (msg.is("SaveTime"))
	{
		SaveGameTime();
	}
}

void GameSavesManager::FirstInitSave()
{
	if (!_impl->database.IsTableExists(USER_DATA_TABLE))
	{
		_impl->database.execSql("CREATE TABLE " + USER_DATA_TABLE + " ("
								"last_uid INTEGER NOT NULL"
								",root_cell INTEGER NOT NULL"
								",current_time VARCHAR(20) NOT NULL"
								");");
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

	if (!_impl->database.IsTableExists(INVESIGATIONS_TABLE))
	{
		_impl->database.execSql("CREATE TABLE " + INVESIGATIONS_TABLE + " ("
								"'investigator_uid' INTEGER NOT NULL"
								",'cellFrom_uid' INTEGER NOT NULL"
								",'cellTo_uid' INTEGER NOT NULL"
								",'begin_time' VARCHAR(20) NOT NULL"
								",'duration' VARCHAR(20) NOT NULL"
								");");
	}

	if (!_impl->database.IsTableExists(INVESIGATORS_TABLE))
	{
		_impl->database.execSql("CREATE TABLE " + INVESIGATORS_TABLE + " ("
								"'investigator_uid' INTEGER NOT NULL"
								",'root_cell' INTEGER"
								",'catch_time_begin' VARCHAR(20) NOT NULL"
								",'catch_time_end' VARCHAR(20) NOT NULL"
								",'state' VARCHAR(100) NOT NULL"
								");");
	}

	if (!_impl->database.IsTableExists(TUTORIAL_STATES_TABLE))
	{
		_impl->database.execSql("CREATE TABLE " + TUTORIAL_STATES_TABLE + " ("
								"'state' VARCHAR(100) NOT NULL"
								");");
	}

	if (!_impl->database.IsTableExists(CELLS_RESOURCES_TABLE))
	{
		_impl->database.execSql("CREATE TABLE " + CELLS_RESOURCES_TABLE + " ("
								"'cell_uid' INTEGER NOT NULL"
								",'resource_id' VARCHAR(100) NOT NULL"
								",'count' INTEGER NOT NULL"
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

	string to_string(Investigator::State __val)
	{
		switch (__val) {
		case Investigator::State::ABORTED:
			return "ABORTED";
		case Investigator::State::INVESTIGATION:
			return "INVESTIGATION";
		case Investigator::State::START_CATCH_DELAY:
			return "START_CATCH_DELAY";
		case Investigator::State::STAYING:
			return "STAYING";
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

Investigator::State CastInvestigatorStateFromString(const std::string& state)
{
	if (state == "ABORTED")
	{
		return Investigator::State::ABORTED;
	}
	else if (state == "INVESTIGATION")
	{
		return Investigator::State::INVESTIGATION;
	}
	else if (state == "START_CATCH_DELAY")
	{
		return Investigator::State::START_CATCH_DELAY;
	}
	else if (state == "STAYING")
	{
		return Investigator::State::STAYING;
	}
	else
	{
		return Investigator::State::START_CATCH_DELAY;
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
		int uid = cellsReader->getValueByName("uid")->asInt();
		int parentId = cellsReader->getValueByName("parent_uid")->asInt();

		Cell::Ptr cell = std::make_shared<Cell>(Cell(World::Instance().GetTownByName(cellsReader->getValueByName("town")->asString())
									  , uid));
		cellsNetwork.AddCell(cell);

		cell->SetCash(cellsReader->getValueByName("cash")->asInt());
		cell->SetExperience(cellsReader->getValueByName("experience")->asInt());

		Vector2 location;
		location.x = cellsReader->getValueByName("location_x")->asFloat();
		location.y = cellsReader->getValueByName("location_y")->asFloat();
		cell->SetLocation(location);

		cell->SetState(CastCellStateFromString(cellsReader->getValueByName("state")->asString()),
					   Utils::GetGameTime(), 1);

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

void GameSavesManager::LoadProcesses()
{
	CellsNetwork &cellsNetwork = World::Instance().GetCellsNetwork();
	SqliteDataReader::Ptr processesReader = _impl->database.execQuery("SELECT * FROM " + PROCESSES_TABLE);
	while (processesReader->next())
	{
		unsigned int cell_uid = processesReader->getValueByName("cell_uid")->asInt();
		Cell::Ptr cell = cellsNetwork.GetCellByUid(cell_uid);

		Cell::State state = CastCellStateFromString(processesReader->getValueByName("type")->asString());
		if (cell->GetState() == state)
		{
			Utils::GameTime stateBegin = Utils::StringToTime(processesReader->getValueByName("begin_time")->asString());
			Utils::GameTime stateDuration = Utils::StringToTime(processesReader->getValueByName("duration")->asString());
			cell->SetState(state, stateBegin, stateDuration);
		}
		else
		{
			WRITE_WARN("Loaded old unusual process for a cell. Process type: "
				+ processesReader->getValueByName("type")->asString());
		}
	}
}

void GameSavesManager::LoadInvestigations()
{
	World &world = World::Instance();
	CellsNetwork &cellsNetwork = world.GetCellsNetwork();

	{
		SqliteDataReader::Ptr investigatorsReader = _impl->database.execQuery("SELECT * FROM "
																			   + INVESIGATORS_TABLE);
		while (investigatorsReader->next())
		{
			int investigatorUid = investigatorsReader->getValueByName("investigator_uid")->asInt();
			Investigator::Ptr investigator = std::make_shared<Investigator>(Investigator(investigatorUid));

			investigator->_catchTimeBegin = Utils::StringToTime(investigatorsReader->getValueByName("catch_time_begin")->asString());
			investigator->_catchTimeEnd = Utils::StringToTime(investigatorsReader->getValueByName("catch_time_end")->asString());
			if (!investigatorsReader->getValueByName("root_cell")->isNull())
			{
				investigator->_investigationRoot = cellsNetwork.GetCellByUid(investigatorsReader->getValueByName("root_cell")->asInt());
			}
			else
			{
				investigator->_investigationRoot = Cell::WeakPtr();
			}
			investigator->_state = CastInvestigatorStateFromString(investigatorsReader->getValueByName("state")->asString());
			world.AddInvestigator(investigator);
		}
	}

	{
		SqliteDataReader::Ptr investigationsReader = _impl->database.execQuery("SELECT * FROM "
																			   + INVESIGATIONS_TABLE
																			   + " ORDER BY investigator_uid ASC");

		while (investigationsReader->next())
		{
			int investigatorUid = investigationsReader->getValueByName("investigator_uid")->asInt();

			Investigator::Branch investigationBranch;
			investigationBranch.cellFrom = cellsNetwork.GetCellByUid(investigationsReader->getValueByName("cellFrom_uid")->asInt());
			investigationBranch.cellTo = cellsNetwork.GetCellByUid(investigationsReader->getValueByName("cellTo_uid")->asInt());
			investigationBranch.timeBegin = Utils::StringToTime(investigationsReader->getValueByName("begin_time")->asString());
			investigationBranch.timeDuration = Utils::StringToTime(investigationsReader->getValueByName("duration")->asString());

			Investigator::Ptr investigator = world.GetInvestigatorByUid(investigatorUid);
			investigator->_activeBranches.push_back(investigationBranch);
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

		World::Instance().InitTime(Utils::StringToTime(reader->getValueByName("current_time")->asString()));
	}
}

void GameSavesManager::LoadTutorialStates()
{
	SqliteDataReader::Ptr reader = _impl->database.execQuery("SELECT * FROM " + TUTORIAL_STATES_TABLE);
	while (reader->next())
	{
		World::Instance().GetTutorialManager().AddTutorialState(reader->getValueByName("state")->asString());
	}
}

void GameSavesManager::LoadCellsResources()
{
	World &world = World::Instance();
	CellsNetwork &cellsNetwork = world.GetCellsNetwork();

	SqliteDataReader::Ptr reader = _impl->database.execQuery("SELECT * FROM " + CELLS_RESOURCES_TABLE);
	while (reader->next())
	{
		unsigned int cell_uid = reader->getValueByName("cell_uid")->asInt();
		Cell::Ptr cell = cellsNetwork.GetCellByUid(cell_uid);

		Resource resource(reader->getValueByName("resource_id")->asString(), reader->getValueByName("count")->asInt());
		cell->AddResource(resource);
	}
}

void GameSavesManager::LoadGameState(void)
{
	std::lock_guard<std::recursive_mutex> lock(::SavesManagerMutex);

	if (_isWorking)
		return;

	_isWorking = true;
	LoadCellsState();
	LoadRunnedTasks();
	LoadProcesses();
	LoadCellsResources();
	LoadInvestigations();
	LoadUserInfo();
	LoadTutorialStates();
	_isWorking = false;
}

static std::string InitCellsAdditionStatement()
{
	return "INSERT INTO " + CELLS_TABLE +
			"(uid, state"
			",parent_uid"
			",town"
			",location_x"
			",location_y"
			",cash"
			",experience"
			")"
			"VALUES";
}

static std::string InitRunnedTasksAdditionStatement()
{
	return "INSERT INTO " + RUNNED_TASKS_TABLE +
			"(task_id"
			",cell_uid"
			",start_time"
			")"
			"VALUES";
}

static std::string InitConstrucrionAdditionStatement()
{
	return "INSERT INTO " + PROCESSES_TABLE +
			"(cell_uid"
			",type"
			",begin_time"
			",duration"
			")"
			"VALUES";
}

static std::string InitCellsResourcesAdditionStatement()
{
	return "INSERT INTO " + CELLS_RESOURCES_TABLE +
			"(cell_uid"
			",resource_id"
			",count"
			")"
			"VALUES";
}

static std::string InitInvestigationAdditionStatement()
{
	return "INSERT INTO " + INVESIGATIONS_TABLE +
			"(investigator_uid"
			",cellFrom_uid"
			",cellTo_uid"
			",begin_time"
			",duration"
			")"
			"VALUES";
}

static std::string InitInvestigatorAdditionStatement()
{
	return "INSERT INTO " + INVESIGATORS_TABLE +
			"(investigator_uid"
			",catch_time_begin"
			",catch_time_end"
			",root_cell"
			",state"
			")"
			"VALUES";
}

static std::string InitTutoraialStatesAdditionStatement()
{
	return "INSERT INTO " + TUTORIAL_STATES_TABLE +
			"(state"
			")"
			"VALUES";
}

static void AppendCellToQuery(std::string* const query, Cell* const cell)
{
	std::string parent_id = cell->GetParent().expired() ? "-1" : std::to_string(cell->GetParent().lock()->GetUid());

	query->append("(")
		.append(std::to_string(cell->GetUid())).append(",")
		.append("'").append(std::to_string(cell->GetState())).append("',")
		.append(parent_id).append(",")
		.append("'").append(cell->GetTown().lock()->GetInfo().name).append("',")
		.append(std::to_string(cell->GetLocation().x)).append(",")
		.append(std::to_string(cell->GetLocation().y)).append(",")
		.append(std::to_string(cell->GetCash())).append(",")
		.append(std::to_string(cell->GetExperience()))
		.append(")");
}

static void AppendTaskToQuery(std::string* const query, Task::Ptr task, int cellIndex)
{
	std::string currentTaskId = task->GetInfo().id;
	std::string taskStartTime = Utils::TimeToString(task->GetStartTime());

	query->append("(")
		.append("'").append(currentTaskId).append("',")
		.append("'").append(std::to_string(cellIndex)).append("',")
		.append("'").append(taskStartTime).append("'")
		.append(")");
}

static void AppendCellProcessToQuery(std::string* const query, Cell* cell)
{
	query->append("(")
		.append("'").append(std::to_string(cell->GetUid())).append("',")
		.append("'").append(std::to_string(cell->GetState())).append("',")
		.append("'").append(Utils::TimeToString(cell->GetStateBegin())).append("',")
		.append("'").append(Utils::TimeToString(cell->GetStateDuration())).append("'")
		.append(")");
}

static void AppendCellResourceToQuery(std::string* const query, unsigned int CellUid, const Resource& resource)
{
	query->append("(")
		.append("'").append(std::to_string(CellUid)).append("',")
		.append("'").append(resource.name).append("',")
		.append(Utils::TimeToString(resource.count))
		.append(")");
}

static void AppendInvestigationToQuery(std::string* const query,
									   const Investigator::Branch& branch,
									   unsigned int investigatorUid)
{
	query->append("(")
		.append("'").append(std::to_string(investigatorUid)).append("',")
		.append("'").append(std::to_string(branch.cellFrom.lock()->GetUid())).append("',")
		.append("'").append(std::to_string(branch.cellTo.lock()->GetUid())).append("',")
		.append("'").append(Utils::TimeToString(branch.timeBegin)).append("',")
		.append("'").append(Utils::TimeToString(branch.timeDuration)).append("'")
		.append(")");
}

static void AppendInvestigatorsToQuery(std::string* const query,
									   const Investigator::Ptr investigator)
{
	query->append("(")
		.append("'").append(std::to_string(investigator->GetUid())).append("',")
		.append("'").append(Utils::TimeToString(investigator->GetCatchBeginTime())).append("',")
		.append("'").append(Utils::TimeToString(investigator->GetCatchEndTime())).append("',");

	if (Cell::Ptr investigationRoot = investigator->GetInvestigationRoot().lock())
	{
		query->append("'").append(std::to_string(investigationRoot->GetUid())).append("',");
	}
	else
	{
		query->append("NULL,");
	}

	query->append("'").append(std::to_string(investigator->GetState())).append("'")
		.append(")");
}

static void AppendSeparator(std::string* const sqlStatement, bool notFirst)
{
	if (notFirst)
	{
		sqlStatement->append(",");
	}
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

static bool FillResourcesAdditionStatement(std::string* const resourcesSqlStatement)
{
	const CellsNetwork::Cells &cells = World::Instance().GetCellsNetwork().GetActiveCells();
	bool addResources = false;
	for (auto& cell : cells)
	{
		for (const auto& resource : cell->GetResources())
		{
			AppendSeparator(resourcesSqlStatement, addResources);
			addResources = true;

			AppendCellResourceToQuery(resourcesSqlStatement, cell->GetUid(), resource.second);
		}
	}
	resourcesSqlStatement->append(";");

	return addResources;
}

static bool FillInvestigationAdditionStatement(std::string* const processesSqlStatement)
{
	const World::Investigators& investigators = World::Instance().GetInvestigators();
	bool addInvestigations = false;
	for (Investigator::Ptr investigator : investigators)
	{
		for (const Investigator::Branch& investigatonBranch : investigator->GetBranches())
		{
			AppendSeparator(processesSqlStatement, addInvestigations);
			addInvestigations = true;

			AppendInvestigationToQuery(processesSqlStatement, investigatonBranch, investigator->GetUid());
		}
	}
	processesSqlStatement->append(";");

	return addInvestigations;
}

static bool FillInvestigatorsAdditionStatement(std::string* const processesSqlStatement)
{
	const World::Investigators& investigators = World::Instance().GetInvestigators();
	bool addInvestigations = false;
	for (Investigator::Ptr investigator : investigators)
	{
		AppendSeparator(processesSqlStatement, addInvestigations);
		addInvestigations = true;

		AppendInvestigatorsToQuery(processesSqlStatement, investigator);
	}
	processesSqlStatement->append(";");

	return addInvestigations;
}

static std::string GetUserInfoAdditionStatement()
{
	World &world = World::Instance();
	return std::string("INSERT INTO ").append(USER_DATA_TABLE).append(" (last_uid, current_time,"
																	  "root_cell) VALUES (")
			.append(std::to_string(world.GetLastUid())).append(",")
			.append(Utils::TimeToString(Utils::GetGameTime())).append(",")
			.append(std::to_string(world.GetCellsNetwork().GetRootCell().lock()->GetUid())).append(");");
}

static bool FillTutorialStatesAdditionStatement(std::string* const tutorialStatesSqlStatement)
{
	const TutorialManager& tutorialManager = World::Instance().GetTutorialManager();
	bool addTutorialStates = false;
	for (auto& tutorialState : tutorialManager.GetTutorialStatements())
	{
		AppendSeparator(tutorialStatesSqlStatement, addTutorialStates);
		addTutorialStates = true;

		tutorialStatesSqlStatement->append("('").append(tutorialState).append("')");
	}
	tutorialStatesSqlStatement->append(";");

	return addTutorialStates;
}

void GameSavesManager::SaveGameState(void)
{
	std::lock_guard<std::recursive_mutex> lock(::SavesManagerMutex);

	if (_isWorking)
		return;

	_isWorking = true;

	std::string cellsAdditionSqlStatement = InitCellsAdditionStatement();
	bool addCells = FillCellsAdditionStatement(&cellsAdditionSqlStatement);

	std::string taskAdditionSqlStatement = InitRunnedTasksAdditionStatement();
	bool addTasks = FillRunnedTasksAdditionStatement(&taskAdditionSqlStatement);

	std::string processesSqlStatement = InitConstrucrionAdditionStatement();
	bool addProcesses = FillProcessesAdditionStatement(&processesSqlStatement);

	std::string cellResourcesSqlStatement = InitCellsResourcesAdditionStatement();
	bool addCellResources = FillResourcesAdditionStatement(&cellResourcesSqlStatement);

	std::string investigationsSqlStatement = InitInvestigationAdditionStatement();
	bool addInvestigations = FillInvestigationAdditionStatement(&investigationsSqlStatement);

	std::string investigatorsSqlStatement = InitInvestigatorAdditionStatement();
	bool addInvestigators = FillInvestigatorsAdditionStatement(&investigatorsSqlStatement);

	std::string userInfoSqlStatement = GetUserInfoAdditionStatement();

	std::string tutorialStatesSqlStatement = InitTutoraialStatesAdditionStatement();
	bool addTutorialStates = FillTutorialStatesAdditionStatement(&tutorialStatesSqlStatement);

	// begining transaction
	_impl->database.execSql("BEGIN;");
	// clearing tables
	_impl->database.execSql("DELETE FROM " + CELLS_TABLE + ";"
							"DELETE FROM " + RUNNED_TASKS_TABLE + ";"
							"DELETE FROM " + PROCESSES_TABLE + ";"
							"DELETE FROM " + INVESIGATIONS_TABLE + ";"
							"DELETE FROM " + INVESIGATORS_TABLE + ";"
							"DELETE FROM " + TUTORIAL_STATES_TABLE + ";"
							"DELETE FROM " + USER_DATA_TABLE + ";"
							"DELETE FROM " + CELLS_RESOURCES_TABLE + ";"
							);
	// adding new data
	if (addCells) _impl->database.execSql(cellsAdditionSqlStatement);
	if (addTasks) _impl->database.execSql(taskAdditionSqlStatement);
	if (addProcesses) _impl->database.execSql(processesSqlStatement);
	if (addInvestigations) _impl->database.execSql(investigationsSqlStatement);
	if (addInvestigators) _impl->database.execSql(investigatorsSqlStatement);
	if (addCellResources) _impl->database.execSql(cellResourcesSqlStatement);
	_impl->database.execSql(userInfoSqlStatement);
	if (addTutorialStates) _impl->database.execSql(tutorialStatesSqlStatement);
	// commiting transaction
	_impl->database.execSql("COMMIT;");

	_isWorking = false;
}

void GameSavesManager::SaveGameTime()
{
	std::lock_guard<std::recursive_mutex> lock(::SavesManagerMutex);

	if (_isWorking)
		return;

	_isWorking = true;

	_impl->database.execSql("UPDATE user_data SET current_time = '" + Utils::TimeToString(Utils::GetGameTime()) + "';");

	_isWorking = false;
}
