#ifndef WORLD_LOADER_H
#define WORLD_LOADER_H

#include <string>
#include <pugixml.hpp>

#include "Region.h"

class WorldLoader
{
public:
	enum class SaveSlotID
	{
		SAVE_SLOT_1,
		SAVE_SLOT_2,
		SAVE_SLOT_3,
		SAVE_SLOT_4,
		SAVES_SLOT_NUM
	};

public:
	WorldLoader(void);

	bool LoadWorld(void);

	/*
	void UploadWorldRegionsHull(const char *xmlFilename);
	void UploadWorldRegionsInfo(const char *xmlFilename);
	*/

	void SaveCurrentWorldState(SaveSlotID slotID);
	void RestoreLastWorldState(SaveSlotID slotID);

private:
	void _InitHullFromXml(const char *name, pugi::xml_node &root, ArbitraryHull &hull) const;

	std::string _slotPaths[SaveSlotID::SAVES_SLOT_NUM];
};

#endif