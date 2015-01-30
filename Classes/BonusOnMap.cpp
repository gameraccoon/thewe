#include "BonusOnMap.h"

#include "GameInfo.h"
#include "World.h"

#include <stdlib.h>

BonusOnMap::BonusOnMap(void)
	: _waitForBonusDestroy(false)
{
	MessageManager::Instance().RegisterReceiver(this, "DeleteBonusWidget");
}

BonusOnMap::~BonusOnMap(void)
{
}

void BonusOnMap::OnStartLogic(void)
{
	Utils::GameTime delay = GameInfo::Instance().GetTime("BONUS_BORN_TIME_DELAY");
	Utils::GameTime offset = GameInfo::Instance().GetTime("BONUS_BORN_TIME_OFFSET");
	_nextBonusTime = CalcNextWaitTime(delay, offset);
}

void BonusOnMap::UpdateToTime(Utils::GameTime time)
{
	if (!_waitForBonusDestroy && time > _nextBonusTime)
	{
		const CellsNetwork::Cells& cells = World::Instance().GetCellsNetwork().GetActiveCells();
		int town_id = rand() % cells.size();
		SendMessageAboutBonus(cells[town_id]->GetUid());
		_waitForBonusDestroy = true;
	}
}

void BonusOnMap::SendMessageAboutBonus(int cellUid)
{
	Message message("ShowBonus");
	message.variables.SetInt("CELL_UID", cellUid);
	MessageManager::Instance().PutMessage(message);
}

void BonusOnMap::AcceptMessage(const Message &message)
{
	if (message.is("DeleteBonusWidget") && _waitForBonusDestroy)
	{
		Utils::GameTime delay = GameInfo::Instance().GetTime("BONUS_BORN_TIME_DELAY");
		Utils::GameTime offset = GameInfo::Instance().GetTime("BONUS_BORN_TIME_OFFSET");
		_nextBonusTime = CalcNextWaitTime(delay, offset);
		
		_waitForBonusDestroy = false;
	}
}

Utils::GameTime BonusOnMap::CalcNextWaitTime(Utils::GameTime delay, Utils::GameTime offset) const
{
	size_t cellsCount = World::Instance().GetCellsNetwork().GetActiveCells().size();
	size_t townsCount = World::Instance().GetTowns().size();
	double r = ((double)rand()/(double)RAND_MAX);
	return Utils::GetGameTime() + ((float)townsCount / cellsCount) * (delay + offset * r - offset/2.0f);
}
