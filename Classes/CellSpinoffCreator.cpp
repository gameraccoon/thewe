#include "CellSpinoffCreator.h"

#include "WorldMapLayer.h"

CellSpinoffCreator::CellSpinoffCreator(const Cell::Info &info, float time, WorldMapLayer *map)
	: _cellInfo(info)
	, _timeTotal(time)
	, _worldMapLayer(map)
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
		// also you must delete this from projector
		_worldMapLayer->removeChild(this);

		_worldMapLayer->CreateNewCell(_cellInfo);
	}
}