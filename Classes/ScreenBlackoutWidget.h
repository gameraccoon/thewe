#ifndef SCREEN_BLACKOUT_WIDGET_H
#define SCREEN_BLACKOUT_WIDGET_H

#include <cocos2d.h>

#include "MiscUtils.h"
#include "World.h"

class ScreenBlackoutWidget : public cocos2d::Node
{
public:
	static ScreenBlackoutWidget* create(cocos2d::Color4F color = cocos2d::Color4F(0.5f, 0.5f, 0.5f, 0.5f));

	void AddSpot(cocos2d::Sprite *spot);
	void RemoveSpot(cocos2d::Sprite *spot, bool release = true);
	void ClearSpots(void);
	void SetBackgroundColor(const cocos2d::Color4F &color);

protected:
	ScreenBlackoutWidget(void);
	virtual ~ScreenBlackoutWidget(void);
	
	bool initWithColor(cocos2d::Color4F color);
	void update(float dt) override;

private:
	typedef std::vector<cocos2d::Sprite *> Spots;

private:
	Spots _spots;

	cocos2d::RenderTexture *_mask;
	cocos2d::GLProgram *_maskProg;
	cocos2d::GLProgram *_drawProg;

	CC_DISALLOW_COPY_AND_ASSIGN(ScreenBlackoutWidget);
};

#endif