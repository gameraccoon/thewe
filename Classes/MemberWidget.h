#ifndef MEMBER_WIDGET_H
#define MEMBER_WIDGET_H

#include "GameInterface.h"

class MemberWidget : public cocos2d::ui::Layout
{
public:
	static MemberWidget* createWithMember(void);
	static MemberWidget* createEmpty(bool drawSpecial);

	bool IsEmptyMemberWidget(void) const;

	void TouchListener(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType eventType);

protected:
	MemberWidget(void);
	virtual ~MemberWidget(void);

	bool initWithMember(void);
	bool initEmpty(bool drawSpecial);

private:
	typedef std::vector<cocos2d::Sprite *> Stars;

private:
	cocos2d::Sprite *_background;
	cocos2d::Sprite *_special;
	cocos2d::Sprite *_face;
	Stars _stars;
	bool _isEmptyMemberWidget;
};

#endif
