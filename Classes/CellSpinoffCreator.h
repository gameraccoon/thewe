#ifndef CELL_SPINOFF_CREATOR_H
#define CELL_SPINOFF_CREATOR_H

#include "CellGameInterface.h"

class WorldMapLayer;

class CellSpinoffCreator : public cocos2d::Node
{
public:
	CellSpinoffCreator(const Cell::Info &info, float time, WorldMapLayer *map, MapProjector *proj);

	virtual bool init(void) override;
	virtual void update(float dt) override;

	void SetProjectorUid(unsigned int uid);

private:
	Cell::Info _cellInfo;

	WorldMapLayer *_worldMapLayer;
	MapProjector *_projector;
	RoundProgressBar *_progressBar;

	unsigned int _projUid;
	float _timeTotal;
};

#endif