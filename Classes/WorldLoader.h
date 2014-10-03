#ifndef WORLD_LOADER_H
#define WORLD_LOADER_H

namespace WorldLoader
{
	/* Load static data */
	bool LoadGameInfo(void);
	
	/* Load save file */
	bool LoadGameState(void);

	/* Save current state of the game */
	bool SaveGameState(void);

	/* Flush game state (relaunch) */
	void FlushGameState(void);
};

#endif // WORLD_LOADER_H