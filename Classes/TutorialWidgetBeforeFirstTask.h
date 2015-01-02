#ifndef TUTORIAL_WIDGET_BEFORE_FIRST_TASK_H
#define TUTORIAL_WIDGET_BEFORE_FIRST_TASK_H

#include "TutorialWidget.h"

class TutorialWidgetBeforeFirstTask : public TutorialWidget
{
public:
	explicit TutorialWidgetBeforeFirstTask(Tutorial::WeakPtr tutorial);

	static TutorialWidgetBeforeFirstTask* create(Tutorial::WeakPtr tutorial);

protected:
	virtual bool init() override;
};

#endif // TUTORIAL_WIDGET_BEFORE_FIRST_TASK_H
