#ifndef TUTORIAL_WIDGET_WAIT_FIRST_TASK_H
#define TUTORIAL_WIDGET_WAIT_FIRST_TASK_H

#include "TutorialWidget.h"

class TutorialWidgetWaitFirstTask : public TutorialWidget
{
public:
	explicit TutorialWidgetWaitFirstTask(Tutorial::WeakPtr tutorial);

	static TutorialWidgetWaitFirstTask* create(Tutorial::WeakPtr tutorial);

protected:
	virtual bool init() override;
};

#endif // TUTORIAL_WIDGET_BEFORE_FIRST_TASK_H
