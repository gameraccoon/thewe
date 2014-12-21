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
}

bool TownMapWidget::init(void)
{
	_townMapSprite = cocos2d::Sprite::create("town.png");
	_townMapSprite->setPosition(0.0f, 0.0f);
	_townMapSprite->setScale(1.0f);

	addChild(_townMapSprite, 0);

	setContentSize(_townMapSprite->getContentSize());
		
	return true;
}

void TownMapWidget::AcceptMessage(const Message&)
{
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
