#ifndef TUTORIAL_WIDGET_AFTER_FIRST_TASK_H
#define TUTORIAL_WIDGET_AFTER_FIRST_TASK_H

#include "TutorialWidget.h"

class TutorialWidgetAfterFirstTask : public TutorialWidget
{
public:
	explicit TutorialWidgetAfterFirstTask(Tutorial::WeakPtr tutorial);

	static TutorialWidgetAfterFirstTask* create(Tutorial::WeakPtr tutorial);

protected:
	virtual bool init() override;
};

#endif // TUTORIAL_WIDGET_BEFORE_FIRST_TASK_H
