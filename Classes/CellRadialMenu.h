#ifndef CELL_UI_RADIAL_MENU_H
#define CELL_UI_RADIAL_MENU_H

#include "GameInterface.h"
#include "MessageManager.h"
#include "Cell.h"

class CellRadialMenu : public cocos2d::Node, public MessageReceiver
{
public:
	static CellRadialMenu* create(MapProjector *projector);

	void SetTargetCell(Cell::WeakPtr cell);
	void Show(bool animated);
	void Hide(bool animated);

	void AcceptMessage(const Message &message) override;

	bool IsOpened(void) const;
	bool IsBelongTo(Cell::Ptr cell) const;

protected:
	CellRadialMenu(MapProjector *projector);
	virtual ~CellRadialMenu(void);

	bool init(void) override;;
	void update(float dt) override;

	void ButtonTouchHandler(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType eventType);

private:
	enum ButtonsType
	{
		OPEN_SPINOFF_TEAM_MENU,
		OPEN_RECRUTMENT_MENU,
		OPEN_TASK_MENU,
		BUTTONS_AMOUNT
	};

private:
	MapProjector *_projector;
	Cell::WeakPtr _cell;

	std::vector<cocos2d::Node *> _buttons;
};

#endif
