#ifndef GAME_LOADER_H
#define GAME_LOADER_H

namespace GameLoader
{
	/* загружает не зависящие от сейвов данные игры */
	bool LoadGameInfo(void);
	
	/* восстанавливает состояние игры из файла */
	bool LoadGameState(void);

	/* сохраняет состояние игры в профиль текущего игрока, если такой существует */
	bool SaveGameState(void);

	/* сбрасывает состояние игры (новая игры\первый запуск) */
	void FlushGameState(void);
};

#endif // GAME_LOADER_H