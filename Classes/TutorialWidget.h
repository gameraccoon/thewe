#ifndef TUTORIAL_WIDGET_H
#define TUTORIAL_WIDGET_H

#include <cocos2d.h>
#include "Tutorial.h"

class TutorialWidget : public cocos2d::Node
{
public:
	explicit TutorialWidget(Tutorial tutorial);

	static TutorialWidget* create(Tutorial tutorial);

	bool IsReadyToClose() const;

private:
	virtual bool init() override;
	void _OnCloseCallback(cocos2d::Ref *sender);

private:
	Tutorial _tutorial;
	bool _isReadyToClose;
};

#endif // TUTORIAL_WIDGET_H
