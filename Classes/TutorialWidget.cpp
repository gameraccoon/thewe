#include "TutorialWidget.h"

#include "WorldMapLayer.h"
#include "MapProjector.h"

#include "TutorialWidgetWelcome.h"
#include "TutorialWidgetFirstCell.h"
#include "TutorialWidgetAfterFirstCell.h"
#include "TutorialWidgetStartFirstTask.h"
#include "TutorialWidgetWaitFirstTask.h"
#include "TutorialWidgetAfterFirstTask.h"

TutorialWidget* TutorialWidget::Make(Tutorial::WeakPtr tutorial, WorldMapLayer *worldMapLayer, MapProjector *projector)
{
	TutorialWidget *widget;
	std::string name = tutorial.lock()->widgetName;

	if (name == "Welcome") {
		widget = TutorialWidgetWelcome::create(tutorial);
	} else if (name == "FirstCell") {
		widget = TutorialWidgetFirstCell::create(tutorial, worldMapLayer, projector);
	} else if (name == "AfterFirstCell") {
		widget = TutorialWidgetAfterFirstCell::create(tutorial, worldMapLayer, projector);
	} else if (name == "StartFirstTask") { 
		widget = TutorialWidgetStartFirstTask::create(tutorial);
	} else if (name == "WaitFirstTask") {
		widget = TutorialWidgetWaitFirstTask::create(tutorial);
	} else if (name == "tutorial") {
		widget = TutorialWidgetAfterFirstTask::create(tutorial);
	} else if (name == "tutorial_a") {
		widget = TutorialWidgetAfterFirstTask::create(tutorial);
	} else {
		Log::Instance().writeError("Unknown tutorial widget: " + name);
		widget = TutorialWidgetAfterFirstTask::create(tutorial);
	}

	return widget;
}

cocos2d::Action* TutorialWidget::Arrow(const cocos2d::Vec2 &dir, float amplitude, float time)
{
	const cocos2d::Vec2 p1 = dir * -amplitude;
	const cocos2d::Vec2 p2 = dir * amplitude;
	const float t1 = time / 2.4f;
	const float t2 = time - t1;

	cocos2d::MoveBy *move_down = cocos2d::MoveBy::create(t2, p1);
	cocos2d::MoveBy *move_up = cocos2d::MoveBy::create(t1, p2);
	cocos2d::EaseSineIn *ease_down = cocos2d::EaseSineIn::create(move_down);
	cocos2d::EaseSineOut *ease_up = cocos2d::EaseSineOut::create(move_up);
	cocos2d::Sequence *motion = cocos2d::Sequence::create(ease_down,ease_up, nullptr);
	cocos2d::RepeatForever *action = cocos2d::RepeatForever::create(motion);

	return action;
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
