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
static const std::string CONSTRUCTIONS_TABLE = "cell_constructions";

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
	std::string dbPath = cocos2d::FileUtils::sharedFileUtils()->getWritablePath();
	dbPath.append("userdata.db");
	_impl = new GameSavesManagerImpl(dbPath);

	MessageManager::Instance().RegisterReceiver(this);

	FirstInitSave();
}

GameSavesManager::~GameSavesManager()
{
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
		_impl->database.execSql("CREATE TABLE " + USER_DATA_TABLE + " (tutorial_state VARCHAR(255));");
	}
	else
	{
		World::Instance().SetFirstLaunch(false);
	}

	if (!_impl->database.IsTableExists(CELLS_TABLE))
	{
		_impl->database.execSql("CREATE TABLE " + CELLS_TABLE + " ("
								"'id' INTEGER NOT NULL,"
								"'state' INTEGER NOT NULL,"
								"'parent_id' INTEGER DEFAULT (-1),"
								"'town' VARCHAR(255) NOT NULL,"
								"'location_x' REAL NOT NULL,"
								"'location_y' REAL NOT NULL,"
								"'cash' INTEGER NOT NULL DEFAULT (0),"
								"'morale' REAL NOT NULL,"
								"'members_count' INTEGER NOT NULL"
								");");
	}

	if (!_impl->database.IsTableExists(RUNNED_TASKS_TABLE))
	{
		_impl->database.execSql("CREATE TABLE " + RUNNED_TASKS_TABLE + " ("
								"'task_id' VARCHAR(255) NOT NULL,"
								"'cell_id' INTEGER NOT NULL,"
								"'start_time' VARCHAR(255) NOT NULL"
								");");
	}

	if (!_impl->database.IsTableExists(CONSTRUCTIONS_TABLE))
	{
		_impl->database.execSql("CREATE TABLE " + CONSTRUCTIONS_TABLE + " ("
								"'cell_id' INTEGER NOT NULL,"
								"'begin_time' VARCHAR(255) NOT NULL,"
								"'duration' VARCHAR(255) NOT NULL"
								");");
	}
}

void GameSavesManager::LoadGameState(void)
{
	World::Instance().SetFirstLaunch(true);
	return; // nothing more while unstable

	{
		std::map<int, std::pair<Cell::Ptr, int>> cellsIndicesCast;

		SqliteDataReader::Ptr cellsReader = _impl->database.execQuery("SELECT * FROM " + CELLS_TABLE);
		while (cellsReader->next())
		{
			Cell::Info info;
			int id = cellsReader->getValueByName("id")->asInt();
			int parentId = cellsReader->getValueByName("parent_id")->asInt();
			info.state = (Cell::State)cellsReader->getValueByName("state")->asInt();
			info.town = World::Instance().GetTownByName(cellsReader->getValueByName("town")->asString());
			info.location.x = cellsReader->getValueByName("location_x")->asFloat();
			info.location.y = cellsReader->getValueByName("location_y")->asFloat();
			info.cash = cellsReader->getValueByName("cash")->asInt();
			info.morale = cellsReader->getValueByName("morale")->asFloat();
			info.membersCount = cellsReader->getValueByName("members_count")->asInt();
			info.parent = nullptr;

			// TEST
			info.constructionBegin = Utils::GetGameTime();
			info.constructionDuration = 1;

			Cell::Ptr cell = std::make_shared<Cell>(info, false);
			//World::Instance().AddCell(cell);
			cellsIndicesCast.insert(std::pair<int, std::pair<Cell::Ptr, int>>(id, std::pair<Cell::Ptr, int>(cell, parentId)));
		}

		// link cells by their id
		for (auto& cell : cellsIndicesCast)
		{
			if (cell.second.second != -1)
			{
				Cell::Ptr parentCell = (*cellsIndicesCast.find(cell.second.second)).second.first;
				parentCell->AddChild(cell.second.first);
			}
		}

		SqliteDataReader::Ptr constructionsReader = _impl->database.execQuery("SELECT * FROM " + CONSTRUCTIONS_TABLE);
		while (constructionsReader->next())
		{
			int cell_id = constructionsReader->getValueByName("cell_id")->asInt();
			Cell::Ptr cell = (*cellsIndicesCast.find(cell_id)).second.first;
			Cell::Info &info = cell->GetInfo();
			info.constructionBegin =  Utils::StringToTime(constructionsReader->getValueByName("begin_time")->asString());
			info.constructionDuration = Utils::StringToTime(constructionsReader->getValueByName("duration")->asString());
		}

		SqliteDataReader::Ptr tasksReader = _impl->database.execQuery("SELECT * FROM " + RUNNED_TASKS_TABLE);
		while (tasksReader->next())
		{
			std::string currentTaskId = tasksReader->getValueByName("task_id")->asString();
			Utils::GameTime startTime = Utils::StringToTime(tasksReader->getValueByName("start_time")->asString());
			int cell_id = tasksReader->getValueByName("cell_id")->asInt();
			Cell::Ptr cell = (*cellsIndicesCast.find(cell_id)).second.first;
			World::Instance().GetTaskManager().RunTask(cell, currentTaskId, startTime);
		}
	}

	// loading the tutorial state
	{
		SqliteDataReader::Ptr reader = _impl->database.execQuery("SELECT * FROM " + USER_DATA_TABLE + " LIMIT 1");
		if (reader->next())
		{
			World::Instance().SetTutorialState(reader->getValueByName("tutorial_state")->asString());
		}
	}
}

void AppendCellToQuery(std::string* const query, Cell* const cell, const std::map<const Cell *, int>* const cellsIndicesCast)
{
	const Cell::Info info = cell->GetInfo();

	std::string parent_id = info.parent != nullptr ? std::to_string(cellsIndicesCast->find(info.parent)->second) : "-1";

	query->append("(")
		.append(std::to_string(cellsIndicesCast->find(cell)->second)).append(",")
		.append(std::to_string(info.state)).append(",")
		.append(parent_id).append(",")
		.append("'").append(info.town.lock()->GetInfo().name).append("',")
		.append(std::to_string(info.location.x)).append(",")
		.append(std::to_string(info.location.y)).append(",")
		.append(std::to_string(info.cash)).append(",")
		.append(std::to_string(info.morale)).append(",")
		.append(std::to_string(info.membersCount))
		.append(")");
}

void AppendTaskToQuery(std::string* const query, Task::Ptr task, int cellIndex)
{
	std::string currentTaskId = task->GetInfo()->id;
	std::string taskStartTime = Utils::TimeToString(task->GetStartTime());

	query->append("(")
		.append("'").append(currentTaskId).append("',")
		.append("'").append(std::to_string(cellIndex)).append("',")
		.append("'").append(taskStartTime).append("'")
		.append(")");
}

void AppendCellConstructionToQuery(std::string* const query, Cell* cell, int cellIndex)
{
	const Cell::Info info = cell->GetInfo();

	query->append("(")
		.append("'").append(std::to_string(cellIndex)).append("',")
		.append("'").append(Utils::TimeToString(info.constructionBegin)).append("',")
		.append("'").append(Utils::TimeToString(info.constructionDuration)).append("'")
		.append(")");
}

void GameSavesManager::SaveGameState(void)
{
	// saving the cells network
	std::string cellsAdditionSqlStatement =
		"INSERT INTO " + CELLS_TABLE +
		"(id, state, parent_id, town, location_x, location_y, cash, morale, members_count)"
		"VALUES";
	bool addCells = false;

	// saving the active tasks
	std::string taskAdditionSqlStatement = "DELETE FROM " + RUNNED_TASKS_TABLE + ";"
			"INSERT INTO " + RUNNED_TASKS_TABLE +
			"(task_id, cell_id, start_time)"
			"VALUES";
	bool addTasks = false;

	// saving construction state of the cells
	std::string constructionSqlStatement = "DELETE FROM " + CONSTRUCTIONS_TABLE + ";"
			"INSERT INTO " + CONSTRUCTIONS_TABLE +
			"(cell_id, begin_time, duration)"
			"VALUES";
	bool addConstructions = false;

	World &world = World::Instance();
	const CellsNetwork::CellsList &cells = world.GetCellsNetwork().GetActiveCellsList();
	int cellIndex = -1;
	std::map<const Cell *, int> cellsIndicesCast;
	for (CellsNetwork::CellsList::const_iterator it = cells.begin(); it != cells.end(); ++it)
	{
		if (addCells)
		{
			cellsAdditionSqlStatement.append(",");
		}
		else
		{
			addCells = true;
		}

		cellIndex++;
		cellsIndicesCast.insert(std::pair<const Cell *, int>((*it).get(), cellIndex));

		Cell *cell = (*it).get();
		Cell::Info &cellInfo = cell->GetInfo();
		AppendCellToQuery(&cellsAdditionSqlStatement, cell, &cellsIndicesCast);

		if (cell->IsCurrentTaskExists())
		{
			if (addTasks)
			{
				taskAdditionSqlStatement.append(",");
			}
			else
			{
				addTasks = true;
			}

			Task::Ptr task = cell->getCurrentTask().lock();
			AppendTaskToQuery(&taskAdditionSqlStatement, task, cellIndex);
		}

		if (cellInfo.state == Cell::State::CONSTRUCTION)
		{
			if (addConstructions)
			{
				constructionSqlStatement.append(",");
			}
			else
			{
				addConstructions = true;
			}

			AppendCellConstructionToQuery(&constructionSqlStatement, cell, cellIndex);
		}
	}

	cellsAdditionSqlStatement.append(";");
	taskAdditionSqlStatement.append(";");
	constructionSqlStatement.append(";");

	// save investigator
//	for (Investigator::Ptr investigator : World::Instance().GetInvestigators())
//	{
//		pugi::xml_node investigatorNode = inves_root.append_child("Investigator");
//		int investigation_root_cell = cellsIndicesCast.find(investigator->GetInvestigationRoot().get())->second;
//		investigatorNode.append_attribute("investigation_root_cell").set_value(investigation_root_cell);

//		SaveInvestigator(investigator->GetRootBranchBundle(), investigatorNode, cellsIndicesCast, true);
//	}

	// saving the tutorial state
	std::string savingTutorialSqlStatement =
			"INSERT INTO "+USER_DATA_TABLE+" (tutorial_state) VALUES ('" + world.GetTutorialState() +"');";

	// begining transaction
	_impl->database.execSql("BEGIN;");
	// clearing tables
	_impl->database.execSql("DELETE FROM " + CELLS_TABLE + ";"
							"DELETE FROM " + RUNNED_TASKS_TABLE + ";"
							"DELETE FROM " + CONSTRUCTIONS_TABLE + ";"
							"DELETE FROM " + USER_DATA_TABLE + ";");
	// adding new data
	if (addCells) _impl->database.execSql(cellsAdditionSqlStatement);
	if (addTasks) _impl->database.execSql(taskAdditionSqlStatement);
	if (addConstructions) _impl->database.execSql(constructionSqlStatement);
	_impl->database.execSql(savingTutorialSqlStatement);
	// commiting transaction
	_impl->database.execSql("COMMIT;");
}
