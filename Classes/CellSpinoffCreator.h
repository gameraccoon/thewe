#ifndef CELL_SPINOFF_CREATOR_H
#define CELL_SPINOFF_CREATOR_H

#include "CellGameInterface.h"

class WorldMapLayer;

class CellSpinoffCreator : public cocos2d::Node
{
public:
	CellSpinoffCreator(const Cell::Info &info, float time, WorldMapLayer *map);

	virtual bool init(void) override;
	virtual void update(float dt) override;

private:
	Cell::Info _cellInfo;

	WorldMapLayer *_worldMapLayer;
	RoundProgressBar *_progressBar;

	float _timeTotal;
};

#endif