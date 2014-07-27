#include "WorldLoader.h"

#include "WorldMap.h"

static void InitHullFromXml(const char *name, const pugi::xml_node &root,  ArbitraryHull &hull)
{
	pugi::xml_node hull_node = root.find_child_by_attribute("Name", name);

	if (hull_node)
	{
		pugi::xml_node point_node = hull_node.first_child();

		while (point_node)
		{
			cocos2d::CCPoint point;
			point.x = point_node.attribute("x").as_float();
			point.y = point_node.attribute("y").as_float();

			hull.PushPoint(point);

			point_node = point_node.next_sibling();
		}
	}
}

bool WorldLoader::LoadWorld(void)
{
	pugi::xml_document hulls_xml_doc;
	pugi::xml_document regions_xml_doc;
	
	if (!hulls_xml_doc.load_file("../_gamedata/hulls.xml") || !regions_xml_doc.load_file("../_gamedata/regions.xml"))
	{
		return false;
	}

	WorldMap &map = WorldMap::Instance();
	map.CleanupRegions();

	pugi::xml_node root = regions_xml_doc.first_child();
	pugi::xml_node region_node = root.first_child();

	while (region_node)
	{
		const char *name = region_node.attribute("Name").as_string();

		Region::Ptr region = Region::Ptr(new Region());

		pugi::xml_node hull_node = region_node.first_child();
		while (hull_node)
		{
			const char *hull_name = hull_node.attribute("Name").as_string();
			ArbitraryHull hull;
			
			InitHullFromXml(hull_name, hulls_xml_doc.first_child(), hull);

			region->AddHull(hull);

			hull_node = hull_node.next_sibling();
		}

		map.AddRegion(name, region);

		region_node = region_node.next_sibling();
	}

	return true;
}

void WorldLoader::SaveCurrentWorldState(SaveSlotID slotID)
{
}

void WorldLoader::RestoreLastWorldState(SaveSlotID slotID)
{
}