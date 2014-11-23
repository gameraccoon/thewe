#ifndef TUTORIAL_WIDGET_H
#define TUTORIAL_WIDGET_H

#include <cocos2d.h>
#include "Tutorial.h"

class TutorialWidget : public cocos2d::Node
{
public:
	explicit TutorialWidget(Tutorial::WeakPtr tutorial);

	static TutorialWidget* create(Tutorial::WeakPtr tutorial);

	bool IsReadyToClose() const;

private:
	virtual bool init() override;
	void _OnCloseCallback(cocos2d::Ref *sender);

private:
	Tutorial::WeakPtr _tutorial;
};

#endif // TUTORIAL_WIDGET_H
