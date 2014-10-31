#ifndef GAME_SAVES_MANAGER_H
#define GAME_SAVES_MANAGER_H

#include "MessageManager.h"

// pimpl idiom
struct GameSavesManagerImpl;

class GameSavesManager: public MessageReceiver
{
public:
	/**
	 * Returns the single instance of the class
	 */
	static GameSavesManager& Instance();

	void LoadGameState();

	virtual void AcceptMessage(const Message &msg) override;

private:
	void FirstInitSave();
	void LoadCellsState();
	void LoadRunnedTasks();
	void LoadProcesses();
	void LoadInvestigations();
	void LoadUserInfo();
	void SaveGameState();

private:
	GameSavesManagerImpl* _impl;

	/*
	 * Turn off useless operations
	 */
	GameSavesManager();
	~GameSavesManager();
	GameSavesManager(const GameSavesManager&);
	void operator=(const GameSavesManager&);
};

#endif // GAME_SAVES_MANAGER_H
