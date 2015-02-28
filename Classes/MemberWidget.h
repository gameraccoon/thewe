#ifndef MEMBER_WIDGET_H
#define MEMBER_WIDGET_H

#include "GameInterface.h"
#include "Member.h"

class MemberWidget : public cocos2d::ui::Layout
{
public:
	static MemberWidget* createWithMember(Member::Ptr member, bool withRemoveButton = false, bool moveable = true);
	static MemberWidget* createEmpty(const std::string &specialType);

	Member::Ptr GetMemberPtr(void) const;

	bool IsEmpty(void) const;
	bool IsMoveable(void) const;

protected:
	MemberWidget(void);
	virtual ~MemberWidget(void);

	bool initWithMember(Member::Ptr member, bool withRemoveButton, bool moveable);
	bool initEmpty(const std::string &specialType);
	
	void TouchListener(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType eventType);
	void OnRemovePressed(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType eventType);

	static std::string GetIconForSpecial(const std::string &special);

private:
	typedef std::vector<cocos2d::Sprite *> Stars;

private:
	cocos2d::Sprite *_background;
	cocos2d::Sprite *_special;
	cocos2d::Sprite *_face;
	cocos2d::ui::Button *_removeButton;
	Member::Ptr _member;
	Stars _stars;
	bool _isEmpty;
	bool _isMoveable;
};

#endif
