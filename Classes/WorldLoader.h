#ifndef WORLD_LOADER_H
#define WORLD_LOADER_H

#include <memory>
#include "ResourceCache.h"

namespace WorldLoader
{
	typedef ResourceCacheMap<std::string>::Ptr CachedGamedata;
	typedef ResourceCacheQueue<std::string>::Ptr CachedLuaScripts;

	/** Load static data from files */
	CachedGamedata LoadGameInfo();
	/** Parse and prepare world with static data */
	bool ParseGameInfo(const CachedGamedata gamedata);

	const CachedLuaScripts LoadLuaScripts();
}

#endif // WORLD_LOADER_H
