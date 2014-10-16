#ifndef CELLS_NET_LAYER_H
#define CELLS_NET_LAYER_H

#include <map>
#include <cocos2d.h>
#include "CellNetWidget.h"

class CellsNetLayer : public cocos2d::Layer
{
public:
	typedef std::multimap<int, CellNetWidget*> CellsTree;

public:
	CellsNetLayer();

	virtual bool init(void) override;

private:
	virtual void update(float dt) override;
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event *event) override;
	void ShowLevel(int level);
	void ShowCell(CellNetWidget* cell);
	void HideAllLevels();
	CellNetWidget* RecursivelyCreateCellsNetwork(Cell::Ptr rootCell, cocos2d::Vector<cocos2d::MenuItem*>* menuItems, int deepness);
	void _CellTouchInputListener(cocos2d::Ref *sender);

private:
	int _currentLevel;
	CellsTree _cells;
	cocos2d::DrawNode *_networkVisualiser;
	bool _isInMooving;
	float levelHeight;
};

#endif // CELLS_NET_LAYER_H
