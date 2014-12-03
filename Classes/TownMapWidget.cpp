#include "TownMapWidget.h"

#include "GameInfo.h"

TownMapWidget::TownMapWidget(Town::Ptr town)
	: _town(town)
	, _hitAreaBeginX(0.0f)
	, _hitAreaBeginY(0.0f)
	, _hitAreaEndX(1.0f)
	, _hitAreaEndY(1.0f)
	, _projectorUid(-1)
{
	init();
}

TownMapWidget::~TownMapWidget(void)
{
	MessageManager::Instance().UnregisterReceiver(dynamic_cast<MessageReceiver *>(this));
}

bool TownMapWidget::init(void)
{
	MessageManager::Instance().RegisterReceiver(dynamic_cast<MessageReceiver *>(this));

	_townMapSprite = cocos2d::Sprite::create("town.png");
	_townMapSprite->setPosition(0.0f, 0.0f);
	_townMapSprite->setScale(1.0f);

	addChild(_townMapSprite, 0);
		
	return true;
}

void TownMapWidget::AcceptMessage(const Message &message)
{
	if (message.is("BornBonus") && message.variables.GetInt("TOWN_UID") == _town->GetUid())
	{
		BonusMapWidget *bonus = new BonusMapWidget(_town, 
												  Vector2(0.0f, 50.0f),
												  GameInfo::Instance().GetTime("BONUS_LIVE_TIME"));
		bonus->autorelease();

		_bonuses.push_back(bonus);
		addChild(bonus, 1);
	}
	else if (message.is("DeleteBonusWidget") && message.variables.GetInt("TOWN_UID") == _town->GetUid())
	{
		for (Bonuses::iterator it = _bonuses.begin(); it != _bonuses.end();)
		{
			removeChild(*it);
			it = _bonuses.erase(it);
		}
	}
}

void TownMapWidget::SetHitArea(float beginX, float beginY, float endX, float endY)
{
	_hitAreaBeginX = beginX;
	_hitAreaEndX = endX;
	_hitAreaBeginY = beginY;
	_hitAreaEndY = endY;
}

void TownMapWidget::GetHitArea(float &beginX, float &endX, float &beginY, float &endY) const
{
	beginX = _hitAreaBeginX;
	endX = _hitAreaEndX;
	beginY = _hitAreaBeginY;
	endY = _hitAreaEndY;
}

void TownMapWidget::SetTownImageVisible(bool visible)
{
	_townMapSprite->setVisible(visible);
}

void TownMapWidget::SetProjectorUid(int uid)
{
	_projectorUid = uid;
}

int TownMapWidget::GetProjectorUid(void) const
{
	return _projectorUid;
}

const cocos2d::Rect& TownMapWidget::GetTownRect(void) const
{
	return _townMapSprite->getTextureRect();
}

Town::Ptr TownMapWidget::GetTown(void) const
{
	return _town;
}