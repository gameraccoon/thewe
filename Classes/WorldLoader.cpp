#include "WorldLoader.h"

#include <pugixml.hpp>

#include <cocos2d.h>

#include "World.h"
#include "Vector2.h"
#include "Region.h"
#include "TaskManager.h"
#include "Log.h"
#include "MiscUtils.h"
#include "GameInfo.h"

#include <string>

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
	std::string buffer = cocos2d::FileUtils::getInstance()->getStringFromFile(fullPath);
	pugi::xml_parse_result result = tasks_xml_doc.load_buffer(buffer.c_str(), buffer.size());

	std::vector<Task::Info> infos;
	
	pugi::xml_node root = tasks_xml_doc.first_child();
	pugi::xml_node task_node = root.first_child();

	while (task_node)
	{
		Task::Info info;

		info.id = task_node.attribute("id").as_string();
		info.fameImpact = task_node.attribute("fameImpact").as_float();
		info.duration = Utils::StringToTime(task_node.attribute("duration").as_string());
		info.moralLevel = task_node.attribute("moralLevel").as_float();
		info.heartPoundingLevel = task_node.attribute("heartPoundingLevel").as_float();
		info.level = task_node.attribute("level").as_int();
		info.needCash = task_node.attribute("needCash").as_int();
		info.needMembers = task_node.attribute("needMembers").as_int();
		info.needTech = task_node.attribute("needTech").as_int();
		info.chanceToLooseMembers = task_node.attribute("chanceToLooseMembers").as_float();
		info.politicalImpact = task_node.attribute("politicalImpact").as_float();
		info.successFn = task_node.attribute("successFn").as_string();
		info.failFn = task_node.attribute("failFn").as_string();
		info.abortFn = task_node.attribute("abortFn").as_string();

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

	std::string hullsBuffer, regionsBuffer;
	
	std::string fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename("hulls.xml");
	hullsBuffer = cocos2d::FileUtils::getInstance()->getStringFromFile(fullPath);
	if (!hulls_xml_doc.load_buffer(hullsBuffer.c_str(), hullsBuffer.size())) {
		Log::Instance().writeError("Failed to open hulls data file.");
		return false;
	}
	
	fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename("regions.xml");
	regionsBuffer = cocos2d::FileUtils::getInstance()->getStringFromFile(fullPath);
	if (!regions_xml_doc.load_buffer(regionsBuffer.c_str(), regionsBuffer.size())) {
		Log::Instance().writeError("Failed to open regions data file.");
		return false;
	}

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
	bool result = true;
	result &= LoadWorld();
	result &= LoadTasksInfo();
	if (result == true)
	{
		Log::Instance().writeInit("World info load successfully");
	}

	return result;
}
