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

	if (!_impl->database.IsTableExists("UserData"))
	{
		_impl->database.execSql("create table UserData (tutorial_state varchar(255));");
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

static void LoadCellsRecursively(pugi::xml_node root, pugi::xml_node parent_node, Cell *parent, std::map<int, Cell *> &cast)
{
	pugi::xml_node childrens = parent_node.child("Childrens");
	pugi::xml_node child_id_node;
	if (childrens)
	{
		child_id_node = childrens.first_child();
	}

	while (child_id_node)
	{
		int child_id = child_id_node.attribute("id").as_int();

		pugi::xml_node child = root.find_child_by_attribute("id", cocos2d::StringUtils::format("%d", child_id).c_str());

		Cell::Info info;
		info.parent = parent;
		info.state = (Cell::State)child.attribute("state").as_int();
		info.town = World::Instance().GetTownByName(child.attribute("town").as_string());
		info.location.x = child.attribute("location_x").as_float();
		info.location.y = child.attribute("location_y").as_float();
		info.cash = child.attribute("cash").as_float();
		info.morale = child.attribute("morale").as_float();
		info.contentment = child.attribute("contentment").as_float();
		info.membersCount = child.attribute("members_num").as_int();
		info.constructionBegin = Utils::GetGameTime();
		info.constructionDuration = GameInfo::Instance().GetFloat("CELL_CONSTRUCTION_TIME");

		pugi::xml_node construct = child.child("Construction");
		if (construct)
		{
			info.constructionBegin =  Utils::StringToTime(construct.attribute("construction_begin").as_string());
			info.constructionDuration = Utils::StringToTime(construct.attribute("construction_duration").as_string());
		}

		Cell::Ptr cell = Cell::Create(info);
		World::Instance().AddCell(cell);
		parent->AddChild(cell);
		cast.insert(std::pair<int, Cell *>(child.attribute("id").as_int(), cell.get()));

		pugi::xml_node tasks = child.child("Tasks");
		if (tasks)
		{
			std::string currentTaskId = tasks.attribute("current_task_id").as_string();
			Utils::GameTime startTime = Utils::StringToTime(tasks.attribute("task_start_time").as_string());
			World::Instance().GetTaskManager().RunTask((Cell::WeakPtr)cell, currentTaskId, startTime);
		}

		LoadCellsRecursively(root, child, cell.get(), cast);

		child_id_node = child_id_node.next_sibling();
	}
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

	cocos2d::FileUtils *fu = cocos2d::FileUtils::getInstance();
	std::string fullPath;
	unsigned char* pBuffer = NULL;
	ssize_t bufferSize = 0;
	std::string filename = "save.sav";

	pugi::xml_document doc;
	pugi::xml_parse_result result;

	fullPath = fu->fullPathForFilename(Utils::GetDocumentsPath().append(filename).c_str());
	pBuffer = fu->getFileData(fullPath.c_str(), "r", &bufferSize);
	result = doc.load_buffer(pBuffer, bufferSize);

	if (!result)
	{
		if (CreateEmptyGameState(filename))
		{
			fullPath = fu->fullPathForFilename(Utils::GetDocumentsPath().append(filename).c_str());
			pBuffer = fu->getFileData(fullPath.c_str(), "r", &bufferSize);
			result = doc.load_buffer(pBuffer, bufferSize);
		}
		else
		{
			Log::Instance().writeError("Failed to create empty world state file.");
		}
	}

	if (result)
	{
		// 1. find the root cell
		// 2. recursively add childs

		pugi::xml_node root = doc.first_child();
		pugi::xml_node player_info = root.child("PlayerInfo");
		pugi::xml_node cells_network = root.child("CellsNetwork");
		pugi::xml_node inves_root = root.child("Investigators");
		pugi::xml_node cell_root = cells_network.find_child_by_attribute("parent_id", "-1");

		std::map<int, Cell *> cellsIndicesCast;

		if (cell_root)
		{ 
			World::Instance().SetFirstLaunch(false);

			Cell::Info info;
			info.parent = nullptr;
			info.state = (Cell::State)cell_root.attribute("state").as_int();
			info.town = World::Instance().GetTownByName(cell_root.attribute("town").as_string());
			info.location.x = cell_root.attribute("location_x").as_float();
			info.location.y = cell_root.attribute("location_y").as_float();
			info.cash = cell_root.attribute("cash").as_float();
			info.morale = cell_root.attribute("morale").as_float();
			info.contentment = cell_root.attribute("contentment").as_float();
			info.membersCount = cell_root.attribute("members_num").as_int();
			info.constructionBegin = Utils::GetGameTime();
			info.constructionDuration = GameInfo::Instance().GetFloat("CELL_CONSTRUCTION_TIME");

			pugi::xml_node construct = cell_root.child("Construction");
			if (construct)
			{
				info.constructionBegin =  Utils::StringToTime(construct.attribute("construction_begin").as_string());
				info.constructionDuration = Utils::StringToTime(construct.attribute("construction_duration").as_string());
			}

			Cell::Ptr cell = Cell::Create(info);
			World::Instance().AddCell(cell);
			cellsIndicesCast.insert(std::pair<int, Cell *>(cell_root.attribute("id").as_int(), cell.get()));

			pugi::xml_node tasks = cell_root.child("Tasks");
			if (tasks)
			{
				std::string currentTaskId = tasks.attribute("current_task_id").as_string();
				Utils::GameTime startTime = Utils::StringToTime(tasks.attribute("task_start_time").as_string());
				World::Instance().GetTaskManager().RunTask((Cell::WeakPtr)cell, currentTaskId, startTime);
			}

			LoadCellsRecursively(cells_network, cell_root, cell.get(), cellsIndicesCast);
		}
		else
		{
			World::Instance().SetFirstLaunch(true);
		}

		if (inves_root)
		{
			pugi::xml_node investigatorNode = inves_root.first_child();

			while (investigatorNode)
			{
				int investigation_root_id = investigatorNode.attribute("investigation_root_cell").as_int();
				Cell* ptr = cellsIndicesCast.find(investigation_root_id)->second;
				Cell::WeakPtr cell = World::Instance().GetCellByInfo(ptr->GetInfo());

				Investigator::BranchBundle bundle;
				LoadInvestigator(bundle, investigatorNode, cellsIndicesCast);
				World::Instance().AddInvestigator(Investigator::Create(cell, bundle));

				investigatorNode = investigatorNode.next_sibling();
			}
		}



		// loading the tutorial state
		{
			SqliteDataReader::Ptr reader = _impl->database.execQuery("select * from UserData limit 1");
			if (reader->next())
			{
				World::Instance().SetTutorialState(reader->getValueByName("tutorial_state")->asString());
			}
		}

		_state = State::Ready;
		return true;
	}
	else
	{
		Log::Instance().writeError("Failed to read world .");
	}

	_state = State::Ready;
	return false;
}

bool WorldLoader::SaveGameState(void)
{
	_state = State::Saving;
	// TODO
	// 1. get number of current profile
	// 2. create and save xml-file whith name of save-@profile name@-@index + 1@
	// 3. if saving complete delete the last

	// temporary code without transactional save

	World &map = World::Instance();
	const World::Cells &cells = map.GetCells();

	std::map<const Cell *, int> cellsIndicesCast;
	int index = 0;
	for (World::Cells::const_iterator it = cells.begin(); it != cells.end(); ++it)
	{
		index = index + 1;
		cellsIndicesCast.insert(std::pair<const Cell *, int>((*it).get(), index));
	}

	pugi::xml_document doc;
	pugi::xml_node root = doc.append_child("Save");

	pugi::xml_node player_info = root.append_child("PlayerInfo");
	pugi::xml_node cells_root = root.append_child("CellsNetwork");
	pugi::xml_node inves_root = root.append_child("Investigators");

	// save cells network
	for (World::Cells::const_iterator it = cells.begin(); it != cells.end(); ++it)
	{
		const Cell *cell = (*it).get();
		const Cell::Info info = (*it)->GetInfo();

		int parent_id = info.parent != nullptr ? cellsIndicesCast.find(info.parent)->second : -1;

		pugi::xml_node cell_node = cells_root.append_child("Cell");
		cell_node.append_attribute("id").set_value(cellsIndicesCast.find(cell)->second);
		cell_node.append_attribute("state").set_value((int)info.state);
		cell_node.append_attribute("parent_id").set_value(parent_id);
		cell_node.append_attribute("town").set_value(info.town.lock()->GetInfo().name.c_str());
		cell_node.append_attribute("location_x").set_value(info.location.x);
		cell_node.append_attribute("location_y").set_value(info.location.y);
		cell_node.append_attribute("cash").set_value(info.cash);
		cell_node.append_attribute("morale").set_value(info.morale);
		cell_node.append_attribute("contentment").set_value(info.contentment);
		cell_node.append_attribute("members_num").set_value(info.membersCount);

		if (!cell->GetChildren().empty())
		{
			pugi::xml_node childrens = cell_node.append_child("Childrens");
			for (Cell::Ptr child : cell->GetChildren())
			{
				pugi::xml_node cell_child_node = childrens.append_child("Child");
				cell_child_node.append_attribute("id").set_value(cellsIndicesCast.find(child.get())->second);
			}
		}

		if (cell->IsCurrentTaskExists())
		{
			Task::Ptr task = cell->getCurrentTask().lock();
			std::string currentTaskId = task->GetInfo()->id;
			std::string taskStartTime = Utils::TimeToString(task->GetStartTime());

			pugi::xml_node tasks = cell_node.append_child("Tasks");
			tasks.append_attribute("current_task_id").set_value(currentTaskId.c_str());
			tasks.append_attribute("task_start_time").set_value(taskStartTime.c_str());
		}

		if (info.state == Cell::State::CONSTRUCTION)
		{
			pugi::xml_node construct = cell_node.append_child("Construction");
			construct.append_attribute("construction_begin").set_value(Utils::TimeToString(info.constructionBegin).c_str());
			construct.append_attribute("construction_duration").set_value(Utils::TimeToString(info.constructionDuration).c_str());
		}
	}

	// save investigator
	for (Investigator::Ptr investigator : World::Instance().GetInvestigators())
	{
		pugi::xml_node investigatorNode = inves_root.append_child("Investigator");
		int investigation_root_cell = cellsIndicesCast.find(investigator->GetInvestigationRoot().get())->second;
		investigatorNode.append_attribute("investigation_root_cell").set_value(investigation_root_cell);

		SaveInvestigator(investigator->GetRootBranchBundle(), investigatorNode, cellsIndicesCast, true);
	}

	// saving the tutorial state
	_impl->database.execSql("begin;"
		"delete from UserData;"
		"insert into UserData (tutorial_state) values ('" + World::Instance().GetTutorialState() +"');"
		"commit;");

	_state = State::Ready;
	return doc.save_file(Utils::GetDocumentsPath().append("save.sav").c_str());
}

void WorldLoader::RequestToSave()
{
	if (_state == State::Ready)
	{
		SaveGameState();
	}
}
