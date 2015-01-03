#ifndef TUTORIAL_WIDGET_H
#define TUTORIAL_WIDGET_H

#include <cocos2d.h>
#include <cocostudio/CCSGUIReader.h>
#include <ui/CocosGUI.h>

#include "Tutorial.h"
#include "ScreenBlackoutWidget.h"
#include "Localization.h"

class WorldMapLayer;
class MapProjector;

class TutorialWidget : public cocos2d::Node
{
public:
	static TutorialWidget* Make(Tutorial::WeakPtr tutorial, WorldMapLayer *worldMapLayer, MapProjector *projector);
	static cocos2d::Action* Arrow(const cocos2d::Vec2 &dir, float amplitude, float time);

	virtual bool IsReadyToClose(void) const;

protected:
	TutorialWidget(Tutorial::WeakPtr tutorial);

	void OnContinueCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType eventType);

	Tutorial::WeakPtr _tutorial;
};

#endif // TUTORIAL_WIDGET_H
