#ifndef TUTORIAL_WIDGET_H
#define TUTORIAL_WIDGET_H

#include <cocos2d.h>

class TutorialWidget : public cocos2d::Node
{
public:
	explicit TutorialWidget(std::string text);

	static TutorialWidget* create(std::string text);

private:
	virtual bool init() override;

private:
	std::string _text;
};

#endif // TUTORIAL_WIDGET_H
