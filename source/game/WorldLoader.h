#ifndef WORLD_LOADER_H
#define WORLD_LOADER_H

#include <string>
#include <pugixml.hpp>

#include "Region.h"

namespace WorldLoader
{
	enum class SaveSlotID
	{
		SAVE_SLOT_1,
		SAVE_SLOT_2,
		SAVE_SLOT_3,
		SAVE_SLOT_4,
		SAVES_SLOT_NUM
	};

	bool LoadWorld(void);
	
	void SaveCurrentWorldState(SaveSlotID slotID);
	void RestoreLastWorldState(SaveSlotID slotID);
};

#endif