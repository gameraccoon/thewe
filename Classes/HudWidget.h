#ifndef HUD_WIDGET_H
#define HUD_WIDGET_H

#include "GameInterface.h"

class HudWidget : public cocos2d::Node, public MessageReceiver
{
public:
	enum class Element
	{
		PC_ZOOM_IN,
		PC_ZOOM_OUT,
		PC_EDITOR
	};

public:
	static HudWidget* create(void);

	void AcceptMessage(const Message &message) override;

	void HidePanels(void);
	void ShowPanels(void);

protected:
	HudWidget(void);
	~HudWidget(void);

	bool init(void) override;

	void DesctopButtonsHandler(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType eventType);

private:
	cocos2d::ui::Widget *_widget;
};

#endif