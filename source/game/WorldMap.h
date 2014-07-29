#ifndef WORLD_MAP_H
#define WORLD_MAP_H

#include <cocos2d.h>
#include <string>

#include "Region.h"

class WorldMap
{
public:
	/**
	 * Возвращает экземпляр карты
	 */
	static WorldMap& Instance();

	typedef std::vector<Region::Ptr> Regions;

	// удаляет все регионы
	void CleanupRegions(void);
	void AddRegion(Region::Ptr region);
	void CreateRegion(std::string regionName);

	const Regions& GetRegions() const;

private:
	Regions _regions;

	static WorldMap* _singleInstance;

	/*
	 * Отключаем ненужные операции
	 */
	WorldMap();
	~WorldMap();
	WorldMap(const WorldMap&);
	void operator=(const WorldMap&);
};

#endif // WORLD_MAP_H
