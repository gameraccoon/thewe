#ifndef HUMAN_TASK_WIDGET_H
#define HUMAN_TASK_WIDGET_H

#include "GameInterface.h"

class MemberWidget : public cocos2d::ui::Layout
{
public:
	static MemberWidget* create(void);

	void TouchListener(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType eventType);

protected:
	MemberWidget(void);
	virtual ~MemberWidget(void);

	bool init(void) override;

private:
	typedef std::vector<cocos2d::Sprite *> Stars;

private:
	cocos2d::Sprite *_specialIcon;
	Stars _stars;
};

#endif
