#include "WorldLoader.h"

#include "pugixml/pugixml.hpp"

#include "World.h"
#include "Vector2.h"
#include "PlayersProfiles.h"
#include "Region.h"
#include "TaskManager.h"
#include "Log.h"
#include "Vector2.h"

#include <string>
#include <strstream>

static void LoadCellsRecursively(pugi::xml_node root, pugi::xml_node parent_node, Cell *parent)
{
	pugi::xml_node child_id_node = parent_node.first_child();

	while (child_id_node)
	{
		int child_id = child_id_node.attribute("id").as_int();

		//char s[16];
		//sprintf_s(s, "%d", child_id);
		std::stringstream ss;
		ss << child_id;
		std::string s = ss.str();
		pugi::xml_node child = root.find_child_by_attribute("id", s.c_str());

		Cell::Info info;
		info.parent = parent;
		info.town = World::Instance().GetTownByName(child.attribute("town").as_string());
		info.location.x = child.attribute("location_x").as_float();
		info.location.y = child.attribute("location_y").as_float();
		info.cash = child.attribute("cash").as_float();
		info.morale = child.attribute("morale").as_float();
		info.contentment = child.attribute("contentment").as_float();
		info.membersCount = child.attribute("members_num").as_int();

		Cell::Ptr cell = Cell::Create(info);
		World::Instance().AddCell(cell);
		parent->AddChild(cell);

		LoadCellsRecursively(root, child, cell.get());

		child_id_node = child_id_node.next_sibling();
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
	
	if (!tasks_xml_doc.load_file("../../Resources/worldinfo/tasks.xml"))
	{
		Log::Instance().writeError("Can't find file 'worldinfo/tasks.xml'");
		return false;
	}

	std::vector<Task::Info> infos;
	
	pugi::xml_node root = tasks_xml_doc.first_child();
	pugi::xml_node task_node = root.first_child();

	while (task_node)
	{
		Task::Info info;

		info.id = task_node.attribute("Id").as_string();
		info.severity = task_node.attribute("Severity").as_float();
		info.duration = task_node.attribute("Duration").as_float();
		info.moralLevel = task_node.attribute("MoralLevel").as_float();
		info.successFn = task_node.attribute("SuccessFn").as_string();
		info.failFn = task_node.attribute("FailFn").as_string();
		info.abortFn = task_node.attribute("AbortFn").as_string();

		infos.push_back(info);

		task_node = task_node.next_sibling();
	}
	
	TaskManager::Instance().FillTasks(infos);

	return true;
}

static bool LoadWorld(void)
{
	pugi::xml_document hulls_xml_doc;
	pugi::xml_document regions_xml_doc;
	
	/*
	Временное решение.
	Нужно добавить директорию
	поиска и реализовать поиск файлов по ней в 
	методе load_file
	*/
	
	std::string filename_hulls, filename_regions;
#ifdef CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	filename_hulls = "../../Resources/worldinfo/hulls.xml";
	filename_regions = "../../Resources/worldinfo/regions.xml";
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID 
	filename_hulls = "map/hulls.xml";
	filename_regions = "map/regions.xml";
#endif

	/*if (!hulls_xml_doc.load_file(filename_hulls.c_str()) || !regions_xml_doc.load_file(filename_regions.c_str()))
	{
		Log::Instance().writeError("Can't find file 'worldinfo/hulls.xml'");
		return false;
	}*/

	//cocos2d::FileUtils::getInstance()->full
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

bool WorldLoader::LoadGameInfo()
{
	bool result = true;
	result &= LoadWorld();
	result &= LoadTasksInfo();
	if (result == true)
	{
		Log::Instance().writeInit("World info load successfully");
	}

	return result;
}

bool WorldLoader::LoadGameState(void)
{
	PlayerProfile *profile = ProfilesManager::Instance().GetCurrentProfile();

	if (profile)
	{
		// 1. Находим корневую ячейку
		// 2. рекурсивно добавляем детей

		pugi::xml_document doc;
		/*if (!doc.load_file(profile->gameStateFilename.c_str()))
		{
			return false;
		}*/

		std::string fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(profile->gameStateFilename.c_str());
		unsigned char* pBuffer = NULL;
		ssize_t bufferSize = 0;
		pBuffer = cocos2d::FileUtils::getInstance()->getFileData(fullPath.c_str(), "r", &bufferSize);
		pugi::xml_parse_result r = doc.load_buffer(pBuffer,bufferSize);


		pugi::xml_node root = doc.first_child();
		pugi::xml_node cells_network = root.first_child();
		pugi::xml_node cell_root = cells_network.find_child_by_attribute("parent_id", "-1");

		if (cell_root)
		{ 
			World::Instance().SetFirstLaunch(false);

			Cell::Info info;
			info.parent = nullptr;
			info.town = World::Instance().GetTownByName(cell_root.attribute("town").as_string());
			info.location.x = cell_root.attribute("location_x").as_float();
			info.location.y = cell_root.attribute("location_y").as_float();
			info.cash = cell_root.attribute("cash").as_float();
			info.morale = cell_root.attribute("morale").as_float();
			info.contentment = cell_root.attribute("contentment").as_float();
			info.membersCount = cell_root.attribute("members_num").as_int();

			Cell::Ptr cell = Cell::Create(info);
			World::Instance().AddCell(cell);

			LoadCellsRecursively(cells_network, cell_root, cell.get());
		}
		else
		{
			World::Instance().SetFirstLaunch(true);
		}

		return true;
	}

	return false;
}

bool WorldLoader::SaveGameState(void)
{
	PlayerProfile *profile = ProfilesManager::Instance().GetCurrentProfile();

	if (profile)
	{
		// TODO
		// 1. узнать какой по счету профиль
		// 2. создать и сохранить xml файл с именем save-@имя профиля@-@какой по счету + 1@
		// 3. если удалось сохранить удаляем предидущий.

		// Временный код без транзакционного сохранения.

		World &map = World::Instance();
		const World::Cells &cells = map.GetCells();

		std::map<const Cell *, int> cells_indices;
		int index = 0;
		for (World::Cells::const_iterator it = cells.begin(); it != cells.end(); ++it)
		{
			index = index + 1;
			cells_indices.insert(std::pair<const Cell *, int>((*it).get(), index));
		}

		pugi::xml_document doc;
		pugi::xml_node root = doc.append_child("Save");

		pugi::xml_node cells_root = root.append_child("CellsNetwork");

		for (World::Cells::const_iterator it = cells.begin(); it != cells.end(); ++it)
		{
			const Cell *cell = (*it).get();
			const Cell::Info info = (*it)->GetInfo();
	
			int parent_id = info.parent != nullptr ? cells_indices.find(info.parent)->second : -1;

			pugi::xml_node cell_node = cells_root.append_child("Cell");
			cell_node.append_attribute("id").set_value(cells_indices.find(cell)->second);
			cell_node.append_attribute("parent_id").set_value(parent_id);
			cell_node.append_attribute("town").set_value(info.town.lock()->GetInfo().name.c_str());
			cell_node.append_attribute("location_x").set_value(info.location.x);
			cell_node.append_attribute("location_y").set_value(info.location.y);
			cell_node.append_attribute("cash").set_value(info.cash);
			cell_node.append_attribute("morale").set_value(info.morale);
			cell_node.append_attribute("contentment").set_value(info.contentment);
			cell_node.append_attribute("members_num").set_value(info.membersCount);

			for (Cell::Ptr child : cell->GetChildrens())
			{
				pugi::xml_node cell_child_node = cell_node.append_child("Child");
				cell_child_node.append_attribute("id").set_value(cells_indices.find(child.get())->second);
			}
		}

		doc.save_file(profile->gameStateFilename.c_str());
	}

	return false;
}

void WorldLoader::FlushGameState(void)
{
}
