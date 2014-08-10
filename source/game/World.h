#ifndef WORLD_H
#define WORLD_H

#include <string>

#include "Region.h"
#include "Cell.h"
#include "Town.h"

class World
{
public:
	/**
	 * Возвращает экземпляр карты
	 */
	static World& Instance();
	
	typedef std::vector<Region::Ptr> Regions;
	typedef std::vector<Cell::Ptr> Cells;
	typedef std::vector<Town::Ptr> Towns;

public:
	// удаляет все регионы
	void CleanupMapContent(void);

	const Regions& GetRegions() const;
	const Cells& GetCells() const;
	const Towns& GetTowns() const;

	float GetWorldTime() const;

	void AddRegion(Region::Ptr region);
	void AddCell(Cell::Ptr cell);
	void AddTown(Town::Ptr town);

	const Region::Ptr GetRegionByName(const std::string &name) const;
	const Town::Ptr GetTownByName(const std::string &name) const;
	const Cell::Ptr GetRootCell(void) const;

	void Update(float deltatime);

	void SetPause(bool pause);
	void SetFirstLaunch(bool newGame);

	bool IsFirstLaunch(void) const;
	bool IsTownAvaliableToPlaceCell(Town::Ptr town) const;

private:
	Regions _regions;
	Cells _cells;
	Towns _towns;

	float _worldTime;
	bool _isGamePaused;
	bool _isFirstLaunch;

	/*
	 * Отключаем ненужные операции
	 */
	World();
	~World();
	World(const World&);
	void operator=(const World&);
};

#endif // WORLD_MAP_H
