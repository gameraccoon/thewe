#include "CellSpinoffCreator.h"

#include "WorldMapLayer.h"

CellSpinoffCreator::CellSpinoffCreator(const Cell::Info &info, float time, WorldMapLayer *map, MapProjector *proj)
	: _cellInfo(info)
	, _timeTotal(time)
	, _worldMapLayer(map)
	, _projector(proj)
	, _projUid(0)
{
	init();
}

bool CellSpinoffCreator::init(void)
{
	_progressBar = new RoundProgressBar("cell.png", 1.0f);
	_progressBar->setPosition(0.0f, 0.0f);
	_progressBar->SetProgressAnimated(100.0f, _timeTotal);
	_progressBar->autorelease();

	scheduleUpdate();
	addChild(_progressBar, 0);

	return true;
}

void CellSpinoffCreator::update(float dt)
{
	if (_progressBar->IsFinished())
	{
		_worldMapLayer->removeChild(this);
		_projector->RemoveMapPart(_projUid);

		_worldMapLayer->CreateNewCell(_cellInfo);
	}
}

void CellSpinoffCreator::SetProjectorUid(unsigned int uid)
{
	_projUid = uid;
}