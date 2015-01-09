#ifndef TUTORIAL_WIDGET_INVESTIGATION_STARTED_H
#define TUTORIAL_WIDGET_INVESTIGATION_STARTED_H

#include "TutorialWidget.h"

class TutorialWidgetInvestigationStarted : public TutorialWidget
{
public:
    explicit TutorialWidgetInvestigationStarted(Tutorial::WeakPtr tutorial);

    static TutorialWidgetInvestigationStarted* create(Tutorial::WeakPtr tutorial);

protected:
    virtual bool init() override;
};

#endif // TUTORIAL_WIDGET_SPINOFF_H
