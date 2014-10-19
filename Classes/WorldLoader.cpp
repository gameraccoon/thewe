#include "WorldLoader.h"

#include <cocos2d.h>

#include <pugixml.hpp>

#include "World.h"
#include "Vector2.h"
#include "Region.h"
#include "TaskManager.h"
#include "Log.h"
#include "Vector2.h"
#include "MiscUtils.h"
#include "GameInfo.h"

#include <string>
#include "SqliteDataReader.h"
#include "SqliteConnection.h"

static const std::string USER_DATA_TABLE = "user_data";
static const std::string CELLS_TABLE = "cells";
static const std::string RUNNED_TASKS_TABLE = "runned_tasks";
static const std::string CONSTRUCTIONS_TABLE = "cell_constructions";

struct WorldLoaderImpl
{
public:
	WorldLoaderImpl(const std::string& filePath)
		:database(filePath)
	{
	}

public:
	SqliteConnection database;
};

WorldLoader::WorldLoader()
{
	std::string dbPath = cocos2d::FileUtils::sharedFileUtils()->getWritablePath();
	dbPath.append("userdata.db");
	_impl = new WorldLoaderImpl(dbPath);

	if (!_impl->database.IsTableExists(USER_DATA_TABLE))
	{
		_impl->database.execSql("CREATE TABLE " + USER_DATA_TABLE + " (tutorial_state VARCHAR(255));");
		World::Instance().SetFirstLaunch(true);
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
								"'contentment' REAL NOT NULL,"
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

WorldLoader::~WorldLoader()
{
	delete _impl;
}

WorldLoader& WorldLoader::Instance()
{
	static WorldLoader singleInstance;
	return singleInstance;
}

static void LoadInvestigator(Investigator::BranchBundle &bundle, pugi::xml_node node, std::map<int, Cell *> &cast)
{		
	pugi::xml_node bundleNode = node.first_child();

	while (bundleNode)
	{
		Investigator::Branch branch;

		branch.cellTo = cast.find(bundleNode.attribute("cellToId").as_int())->second;
		branch.cellFrom = cast.find(bundleNode.attribute("cellFromId").as_int())->second;
		branch.timeBegin = Utils::StringToTime(bundleNode.attribute("timeBegin").as_string());
		branch.timeEnd = Utils::StringToTime(bundleNode.attribute("timeEnd").as_string());
		branch.timeDuration = Utils::StringToTime(bundleNode.attribute("timeDuration").as_string());
		branch.progressPercentage = 0.0f;
		LoadInvestigator(branch.childBrunches, bundleNode, cast);

		bundle.push_back(branch);
		bundleNode = bundleNode.next_sibling();
	}
}

static void SaveInvestigator(const Investigator::BranchBundle &bundle, pugi::xml_node parentBranchNode,
							 std::map<const Cell *, int> &cast, bool first = false)
{
	if (!bundle.empty())
	{	
		for (const Investigator::Branch &branch : bundle)
		{
			pugi::xml_node branchNode = parentBranchNode.append_child("Branch");

			branchNode.append_attribute("cellToId").set_value(cast.find(branch.cellTo)->second);
			branchNode.append_attribute("cellFromId").set_value(cast.find(branch.cellFrom)->second);
			branchNode.append_attribute("timeBegin").set_value(Utils::TimeToString(branch.timeBegin).c_str());
			branchNode.append_attribute("timeEnd").set_value(Utils::TimeToString(branch.timeEnd).c_str());
			branchNode.append_attribute("timeDuration").set_value(Utils::TimeToString(branch.timeDuration).c_str());

			SaveInvestigator(branch.childBrunches, branchNode, cast);
		}
	}
}

static void InitHullFromXml(const char *name, const pugi::xml_node &root,  ArbitraryHull &hull)
{
	pugi::xml_node hull_node = root.find_child_by_attribute("Name", name);

	if (hull_node)
	{
		pugi::xml_node point_node = hull_node.first_child();

		while (point_node)
		{
			Vector2 point;
			point.x = point_node.attribute("x").as_float();
			point.y = point_node.attribute("y").as_float();

			hull.PushPoint(point);

			point_node = point_node.next_sibling();
		}
	}
}

static bool LoadTasksInfo()
{
	pugi::xml_document tasks_xml_doc;
	std::string fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename("tasks.xml");
	unsigned char* pBuffer = NULL;
	ssize_t bufferSize = 0;
	pBuffer = cocos2d::FileUtils::getInstance()->getFileData(fullPath.c_str(), "r", &bufferSize);
	pugi::xml_parse_result result = tasks_xml_doc.load_buffer(pBuffer, bufferSize);

	std::vector<Task::Info> infos;
	
	pugi::xml_node root = tasks_xml_doc.first_child();
	pugi::xml_node task_node = root.first_child();

	while (task_node)
	{
		Task::Info info;

		info.id = task_node.attribute("Id").as_string();
		info.title = task_node.attribute("Title").as_string();
		info.severity = task_node.attribute("Severity").as_float();
		info.duration = Utils::StringToTime(task_node.attribute("Duration").as_string());
		info.moralLevel = task_node.attribute("MoralLevel").as_float();
		info.successFn = task_node.attribute("SuccessFn").as_string();
		info.failFn = task_node.attribute("FailFn").as_string();
		info.abortFn = task_node.attribute("AbortFn").as_string();

		infos.push_back(info);

		task_node = task_node.next_sibling();
	}
	
	World::Instance().GetTaskManager().FillTasks(infos);

	return true;
}

static bool LoadWorld(void)
{
	pugi::xml_document hulls_xml_doc;
	pugi::xml_document regions_xml_doc;

	std::string fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename("hulls.xml");
	unsigned char* pBuffer = NULL;
	ssize_t bufferSize = 0;
	pBuffer = cocos2d::FileUtils::getInstance()->getFileData(fullPath.c_str(), "r", &bufferSize);
	pugi::xml_parse_result result = hulls_xml_doc.load_buffer(pBuffer,bufferSize);
	
	fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename("regions.xml");
	pBuffer = cocos2d::FileUtils::getInstance()->getFileData(fullPath.c_str(), "r", &bufferSize);
	result = regions_xml_doc.load_buffer(pBuffer,bufferSize);

	World &map = World::Instance();
	map.CleanupMapContent();

	pugi::xml_node root = regions_xml_doc.first_child();
	pugi::xml_node region_node = root.first_child();

	while (region_node)
	{
		Region::Info info;

		info.name = region_node.attribute("Name").as_string();
		info.population = region_node.attribute("Population").as_float();
		info.desc = region_node.attribute("Desc").as_string();

		Region::Ptr region = Region::Create(info);

		pugi::xml_node hulls_root =  region_node.first_child();
		pugi::xml_node towns_root = hulls_root.next_sibling();

		pugi::xml_node hull_node = hulls_root.first_child();
		while (hull_node)
		{
			const char *hull_name = hull_node.attribute("Name").as_string();
			ArbitraryHull hull;
			
			InitHullFromXml(hull_name, hulls_xml_doc.first_child(), hull);

			region->AddHull(hull);

			hull_node = hull_node.next_sibling();
		}

		pugi::xml_node town_node = towns_root.first_child();
		while (town_node)
		{
			Town::Info info;

			float x = town_node.attribute("x").as_float();
			float y = town_node.attribute("y").as_float();

			info.region = region;
			info.name = town_node.attribute("name").as_string();
			info.desc = town_node.attribute("desc").as_string();
			info.population = town_node.attribute("population").as_float();
			info.rank = town_node.attribute("rank").as_float();
			info.spriteScale = town_node.attribute("scale").as_float();
			info.location = Vector2(x, y);

			Town::Ptr town = Town::Create(info);
			map.AddTown(town);

			town_node = town_node.next_sibling();
		}

		map.AddRegion(region);

		region_node = region_node.next_sibling();
	}

	return true;
}

static bool CreateEmptyGameState(const std::string &filename)
{
	pugi::xml_document doc;

	pugi::xml_node root = doc.append_child("Save");
	pugi::xml_node node = root.append_child("CellsNetwork");

	return doc.save_file(Utils::GetDocumentsPath().append("save.sav").c_str());
}

bool WorldLoader::LoadGameInfo()
{
	_state = State::Loading;
	bool result = true;
	result &= LoadWorld();
	result &= LoadTasksInfo();
	if (result == true)
	{
		Log::Instance().writeInit("World info load successfully");
	}

	_state = State::Ready;
	return result;
}

bool WorldLoader::LoadGameState(void)
{
	_state = State::Loading;

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
			info.contentment = cellsReader->getValueByName("contentment")->asFloat();
			info.membersCount = cellsReader->getValueByName("members_count")->asInt();
			info.parent = nullptr;

			// TEST
			info.constructionBegin = Utils::GetGameTime();
			info.constructionDuration = 1;

			Cell::Ptr cell = std::make_shared<Cell>(info);
			World::Instance().AddCell(cell);
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

//	if (inves_root)
//	{
//		pugi::xml_node investigatorNode = inves_root.first_child();

//		while (investigatorNode)
//		{
//			int investigation_root_id = investigatorNode.attribute("investigation_root_cell").as_int();
//			Cell* ptr = cellsIndicesCast.find(investigation_root_id)->second;
//			Cell::WeakPtr cell = World::Instance().GetCellByInfo(ptr->GetInfo());

//			Investigator::BranchBundle bundle;
//			LoadInvestigator(bundle, investigatorNode, cellsIndicesCast);
//			World::Instance().AddInvestigator(Investigator::Create(cell, bundle));

//			investigatorNode = investigatorNode.next_sibling();
//		}
//	}

	// loading the tutorial state
	{
		SqliteDataReader::Ptr reader = _impl->database.execQuery("SELECT * FROM " + USER_DATA_TABLE + " LIMIT 1");
		if (reader->next())
		{
			World::Instance().SetTutorialState(reader->getValueByName("tutorial_state")->asString());
		}
	}

	_state = State::Ready;
	return false;
}

bool WorldLoader::SaveGameState(void)
{
	_state = State::Saving;


	// saving the cells network
	std::string cellsAdditionSqlStatement =
		"INSERT INTO " + CELLS_TABLE +
		"(id, state, parent_id, town, location_x, location_y, cash, morale, contentment, members_count)"
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
	const World::Cells &cells = world.GetCells();
	int cellIndex = -1;
	std::map<const Cell *, int> cellsIndicesCast;
	for (World::Cells::const_iterator it = cells.begin(); it != cells.end(); ++it)
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

		const Cell *cell = (*it).get();
		const Cell::Info info = (*it)->GetInfo();

		std::string parent_id = info.parent != nullptr ? std::to_string(cellsIndicesCast.find(info.parent)->second) : "-1";

		cellsAdditionSqlStatement.append("(");
		cellsAdditionSqlStatement.append(std::to_string(cellsIndicesCast.find(cell)->second)).append(",");
		cellsAdditionSqlStatement.append(std::to_string(info.state)).append(",");
		cellsAdditionSqlStatement.append(parent_id).append(",");
		cellsAdditionSqlStatement.append("'").append(info.town.lock()->GetInfo().name).append("',");
		cellsAdditionSqlStatement.append(std::to_string(info.location.x)).append(",");
		cellsAdditionSqlStatement.append(std::to_string(info.location.y)).append(",");
		cellsAdditionSqlStatement.append(std::to_string(info.cash)).append(",");
		cellsAdditionSqlStatement.append(std::to_string(info.morale)).append(",");
		cellsAdditionSqlStatement.append(std::to_string(info.contentment)).append(",");
		cellsAdditionSqlStatement.append(std::to_string(info.membersCount));
		cellsAdditionSqlStatement.append(")");

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
			std::string currentTaskId = task->GetInfo()->id;
			std::string taskStartTime = Utils::TimeToString(task->GetStartTime());

			taskAdditionSqlStatement.append("(");
			taskAdditionSqlStatement.append("'").append(currentTaskId).append("',");
			taskAdditionSqlStatement.append("'").append(std::to_string(cellIndex)).append("',");
			taskAdditionSqlStatement.append("'").append(taskStartTime).append("'");
			taskAdditionSqlStatement.append(")");
		}

		if (info.state == Cell::State::CONSTRUCTION)
		{
			if (addConstructions)
			{
				constructionSqlStatement.append(",");
			}
			else
			{
				addConstructions = true;
			}

			constructionSqlStatement.append("(");
			constructionSqlStatement.append("'").append(std::to_string(cellIndex)).append("',");
			constructionSqlStatement.append("'").append(Utils::TimeToString(info.constructionBegin)).append("',");
			constructionSqlStatement.append("'").append(Utils::TimeToString(info.constructionDuration)).append("'");
			constructionSqlStatement.append(")");
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

	_state = State::Ready;
}

void WorldLoader::RequestToSave()
{
	if (_state == State::Ready)
	{
		SaveGameState();
	}
}
