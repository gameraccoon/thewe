#include "BonusOnMap.h"

#include "MessageManager.h"
#include "GameInfo.h"

BonusOnMap::BonusOnMap(void)
	: _startTime(0)
	, _waitForBonusDestroy(false)
{
	MessageManager::Instance().RegisterReceiver(dynamic_cast<BonusOnMap *>(this));

	_startTime = Utils::GetGameTime();
	Utils::GameTime delay = GameInfo::Instance().GetTime("BONUS_BORN_TIME_DELAY");
	Utils::GameTime offset = GameInfo::Instance().GetTime("BONUS_BORN_TIME_OFFSET");
	_waitTime = CalcNextWaitTime(delay, offset);
}

BonusOnMap::~BonusOnMap(void)
{
	MessageManager::Instance().UnregisterReceiver(dynamic_cast<BonusOnMap *>(this));
}

void BonusOnMap::UpdateToTime(Utils::GameTime time, const std::vector<Town::Ptr> &towns)
{
	if (time > (_startTime + _waitTime) && !_waitForBonusDestroy)
	{
		int town_id = rand() % towns.size();
		SendMessageAboutBornBonus(towns[town_id]->GetUid());
		_waitForBonusDestroy = true;
	}
}

void BonusOnMap::SendMessageAboutBornBonus(int townUid)
{
	Message message("BornBonus");
	message.variables.SetInt("TOWN_UID", townUid);
	MessageManager::Instance().PutMessage(message);
}

void BonusOnMap::AcceptMessage(const Message &message)
{
	if (message.is("DeleteBonusWidget") && _waitForBonusDestroy)
	{
		Utils::GameTime delay = GameInfo::Instance().GetTime("BONUS_BORN_TIME_DELAY");
		Utils::GameTime offset = GameInfo::Instance().GetTime("BONUS_BORN_TIME_OFFSET");
		_waitTime = CalcNextWaitTime(delay, offset);
		_startTime = Utils::GetGameTime();
		
		_waitForBonusDestroy = false;
	}
}

Utils::GameTime BonusOnMap::CalcNextWaitTime(Utils::GameTime delay, Utils::GameTime offset) const
{
	double r = ((double)rand()/(double)RAND_MAX);
	return delay + offset * r - offset/2.0f;
}