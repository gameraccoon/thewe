#ifndef TUTORIAL_WIDGET_AFTER_FIRST_CELL_H
#define TUTORIAL_WIDGET_AFTER_FIRST_CELL_H

#include "TutorialWidget.h"

class TutorialWidgetAfterFirstCell : public TutorialWidget
{
public:
	explicit TutorialWidgetAfterFirstCell(Tutorial::WeakPtr tutorial);

	static TutorialWidgetAfterFirstCell* create(Tutorial::WeakPtr tutorial);

protected:
	virtual bool init() override;
};

#endif // TUTORIAL_WIDGET_AFTER_FIRST_CELL_H
