#ifndef TUTORIAL_WIDGET_H
#define TUTORIAL_WIDGET_H

#include <cocos2d.h>
#include <cocostudio/CCSGUIReader.h>
#include <ui/CocosGUI.h>

#include "Tutorial.h"
#include "ScreenBlackoutWidget.h"
#include "Localization.h"

class TutorialWidget : public cocos2d::Node
{
public:
	static TutorialWidget* Make(Tutorial::WeakPtr tutorial);

	TutorialWidget(Tutorial::WeakPtr tutorial);

	virtual bool IsReadyToClose(void) const;

protected:
	void OnContinueCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType eventType);

	Tutorial::WeakPtr _tutorial;
};

#endif // TUTORIAL_WIDGET_H
