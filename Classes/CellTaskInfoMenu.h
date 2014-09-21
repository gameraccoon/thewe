#ifndef CELL_TASK_INFO_MENU
#define CELL_TASK_INFO_MENU

#include "CellGameInterface.h"

class CellTaskInfoMenu : public cocos2d::Layer
{
public:
	CellTaskInfoMenu(Cell::WeakPtr cell, CellMenuSelector *selector);

	virtual bool init(void) override;
	virtual void update(float dt) override;

private:
	void _OnCloseCallback(cocos2d::Ref *sender);

	cocos2d::Label *_taskProgressLabel;

	ProgressBar *_taskProgressBar;

	Cell::WeakPtr _cell;
	Task::Ptr _cellCurrentTask;
	CellMenuSelector *_cellMenuSelector;
};

#endif // CELL_TASK_INFO_MENU