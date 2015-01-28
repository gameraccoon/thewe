#include "TownMapWidget.h"

#include "GameInfo.h"

TownMapWidget::TownMapWidget(Town::Ptr town)
	: _town(town)
	, _hitArea(0.0f, 0.0f, 1.0f, 1.0f)
	, _projectorUid(-1)
{
	init();
}

TownMapWidget::~TownMapWidget(void)
{
}

bool TownMapWidget::init(void)
{
	_townMapSprite = cocos2d::Sprite::create("gamefield/town.png");
	_townMapSprite->setPosition(0.0f, 0.0f);
	_townMapSprite->setScale(1.0f);

	addChild(_townMapSprite, 0);

	setContentSize(_townMapSprite->getContentSize());
		
	return true;
}

void TownMapWidget::SetHitArea(const cocos2d::Rect& hitArea)
{
	_hitArea = hitArea;
}

cocos2d::Rect TownMapWidget::GetHitArea() const
{
	return _hitArea;
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
