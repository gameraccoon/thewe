#ifndef TUTORIAL_WIDGET_INVESTIGATOR_STARTED_H
#define TUTORIAL_WIDGET_INVESTIGATOR_STARTED_H

#include "TutorialWidget.h"

class TutorialWidgetInvestigatorUncatched : public TutorialWidget
{
public:
    explicit TutorialWidgetInvestigatorUncatched(Tutorial::WeakPtr tutorial);

    static TutorialWidgetInvestigatorUncatched* create(Tutorial::WeakPtr tutorial);

protected:
    virtual bool init() override;
};

#endif // TUTORIAL_WIDGET_INVESTIGATOR_STARTED_H
