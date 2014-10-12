#ifndef WORLD_LOADER_H
#define WORLD_LOADER_H

class WorldLoader
{
public:
	/**
	 * Returns the single instance of the class
	 */
	static WorldLoader& Instance();

	/* Load static data */
	bool LoadGameInfo(void);
	
	/* Load save file */
	bool LoadGameState(void);

	/* Save current state of the game */
	bool SaveGameState(void);

	/** Say to save data */
	void RequestToSave();

private:
	enum class State
	{
		Ready
		,Loading
		,Saving
	} _state;

	/*
	 * Turn off useless operations
	 */
	WorldLoader();
	~WorldLoader();
	WorldLoader(const WorldLoader&);
	void operator=(const WorldLoader&);
};

#endif // WORLD_LOADER_H
