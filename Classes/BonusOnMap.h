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
	void UpdateToTime(Utils::GameTime time);
	void SendMessageAboutBonus(int cellUid);

	void AcceptMessage(const Message &message) override;

private:
	Utils::GameTime CalcNextWaitTime(Utils::GameTime delay, Utils::GameTime offset) const;

	Utils::GameTime _nextBonusTime;

	bool _waitForBonusDestroy;
};

#endif
