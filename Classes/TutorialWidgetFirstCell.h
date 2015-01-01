#ifndef TUTORIAL_WIDGET_FIRST_CELL_H
#define TUTORIAL_WIDGET_FIRST_CELL_H

#include "TutorialWidget.h"

class TutorialWidgetFirstCell : public TutorialWidget
{
public:
	explicit TutorialWidgetFirstCell(Tutorial::WeakPtr tutorial);

	static TutorialWidgetFirstCell* create(Tutorial::WeakPtr tutorial);

protected:
	virtual bool init() override;
};

#endif // TUTORIAL_WIDGET_H
