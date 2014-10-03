#ifndef CELLS_NET_LAYER_H
#define CELLS_NET_LAYER_H

#include <cocos2d.h>

class CellsNetLayer : public cocos2d::Layer
{
public:
	CellsNetLayer();

	virtual bool init(void) override;

private:
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event *event) override;

private:
};

#endif // CELLS_NET_LAYER_H
