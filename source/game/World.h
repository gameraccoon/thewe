#ifndef WORLD_H
#define WORLD_H

#include <string>

#include "Region.h"
#include "Cell.h"

class World
{
public:
	/**
	 * Возвращает экземпляр карты
	 */
	static World& Instance();
	
	typedef std::vector<Region::Ptr> Regions;
	typedef std::vector<Cell::Ptr> Cells;

public:
	// удаляет все регионы
	void CleanupRegions(void);

	const Regions& GetRegions() const;
	const Cells& GetCells() const;

	float GetWorldTime() const;

	void AddRegion(Region::Ptr region);
	void AddCell(Cell::Ptr cell);

	const Region::Ptr GetRegionByName(const std::string name) const;

	void Update(float deltatime);

	void SetPause(bool pause);
private:
	Regions _regions;
	Cells _cells;

	float _worldTime;
	bool isGamePaused;

	/*
	 * Отключаем ненужные операции
	 */
	World();
	~World();
	World(const World&);
	void operator=(const World&);
};

#endif // WORLD_MAP_H
