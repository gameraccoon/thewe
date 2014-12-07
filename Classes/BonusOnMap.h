#ifndef BONUS_ON_MAP_H
#define BONUS_ON_MAP_H

#include "MessageManager.h"
#include "MiscUtils.h"
#include "Town.h"

class BonusOnMap : public MessageReceiver
{
public:
	BonusOnMap(void);
	~BonusOnMap(void);

	void OnStartLogic(void);
	void UpdateToTime(Utils::GameTime time, const std::vector<Town::Ptr> &towns);
	void SendMessageAboutBornBonus(int townUid);

	void AcceptMessage(const Message &message) override;

private:
	Utils::GameTime CalcNextWaitTime(Utils::GameTime delay, Utils::GameTime offset) const;

	Utils::GameTime _startTime;
	Utils::GameTime _waitTime;

	bool _waitForBonusDestroy;
};

#endif