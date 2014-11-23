#ifndef SESSION_END_SCREEN_H
#define SESSION_END_SCREEN_H

#include <cocos2d.h>
#include <string>

class CustomSessionEndScreen : public cocos2d::Layer
{
public:
	struct ConstructionInfo
	{
		std::string bigMessage;
		std::string shortDesc;
		cocos2d::Color3B backgrndColor;
		cocos2d::Color3B messageColor;
		bool showStaticstics;

		cocos2d::ccMenuCallback callback;
	};

public:
	CustomSessionEndScreen(const ConstructionInfo &ci);

	virtual bool init(void) override;

private:
	void ApplyPulsationAnimation(cocos2d::Node *drawable, float amplX, float amplY);

	ConstructionInfo _info;
};

#endif