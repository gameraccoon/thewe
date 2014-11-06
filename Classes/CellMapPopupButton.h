#ifndef CELL_MAP_POPUP_BUTTON_H
#define CELL_MAP_POPUP_BUTTON_H

#include "CellGameInterface.h"

class CellMapPopupButton : public cocos2d::Node
{
public:
	struct Settings
	{
		std::string normalStateImage;
		std::string pressedStateImage;
		cocos2d::ccMenuCallback callbackOnPress;
	};

public:
	CellMapPopupButton(const Settings &settings);
	static CellMapPopupButton* create(const Settings &settings);

	virtual bool init(void) override;

	void SetOnPressCallback(cocos2d::ccMenuCallback &callback);

	void Appear(float jumpHeight, float angle = 0.0f);
	void Disappear(bool useWarningAnim = false);

private:
	void AddJumpInAnimation(cocos2d::Node *target);
	void AddJumpOutAnimation(cocos2d::Node *target);
	void AddWarningOutAnimation(cocos2d::Node *target);
	void AddStayAnimation(cocos2d::Node *target);

	void FinishAppearCAllback(void);
	void FinishDisappearCallback(void);

	cocos2d::MenuItemImage *_button;
	cocos2d::Vec2 _startScale;

	Settings _settings;
	float _jumpHeight;
};


#endif
