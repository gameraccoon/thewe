#ifndef CELL_GAME_INTERFACE_H
#define CELL_GAME_INTERFACE_H

#include <cocos2d.h>

#include "Cell.h"
#include "WorldMapLayer.h"
#include "TaskManager.h"

class CellMenuSelector : public cocos2d::Layer
{
public:
	CellMenuSelector(Cell::WeakPtr cell, const Vector2 &position);
		
	virtual bool init() override;

private:
	Cell::WeakPtr _cell;

	cocos2d::Sprite *_test;

	Vector2 _position;
};

#endif