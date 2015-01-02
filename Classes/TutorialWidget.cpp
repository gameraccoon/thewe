#include "TutorialWidget.h"

#include "WorldMapLayer.h"
#include "MapProjector.h"

#include "TutorialWidgetWelcome.h"
#include "TutorialWidgetFirstCell.h"
#include "TutorialWidgetAfterFirstCell.h"

TutorialWidget* TutorialWidget::Make(Tutorial::WeakPtr tutorial, WorldMapLayer *worldMapLayer, MapProjector *projector)
{
	TutorialWidget *widget;
	std::string name = tutorial.lock()->widgetName;

	if (name == "Welcome") {
		widget = TutorialWidgetWelcome::create(tutorial);
	} else if (name == "FirstCell") {
		widget = TutorialWidgetFirstCell::create(tutorial, worldMapLayer, projector);
	} else {
		widget = TutorialWidgetAfterFirstCell::create(tutorial);
	}

	return widget;
}

TutorialWidget::TutorialWidget(Tutorial::WeakPtr tutorial)
	: _tutorial(tutorial)
{
}

bool TutorialWidget::IsReadyToClose(void) const
{
	return _tutorial.expired();
}

void TutorialWidget::OnContinueCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType eventType)
{
	if (eventType == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		World::Instance().GetTutorialManager().RemoveCurrentTutorial();
	}
}