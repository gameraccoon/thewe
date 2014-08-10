#ifndef WORLD_LOADER_H
#define WORLD_LOADER_H

namespace WorldLoader
{
	/* загружает не зависящие от сейвов данные карты*/
	bool LoadWorld(void);
	
	/* восстанавливает состояние игры из файла */
	bool LoadGameState(void);

	/* сохраняет состояние игры в профиль текущего игрока, если такой существует */
	bool SaveGameState(void);

	/* сбрасывает состояние игры (новая игры\первый запуск) */
	void FlushGameState(void);
};

#endif