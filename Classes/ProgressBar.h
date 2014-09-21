#ifndef PROGRESS_BAR_H
#define PROGRESS_BAR_H

#include <cocos2d.h>

class ProgressBar : public cocos2d::Node
{
public:
	ProgressBar(float maxSizeX, float maxSizeY, cocos2d::Color4F color = cocos2d::Color4F(0.3f, 0.3f, 0.3f, 1.0f));

	virtual bool init(void) override;

	void SetProgress(float progress);

	float GetMaxWidth(void) const;
	float GetMaxHeight(void) const;
	float GetCurrentProgress(void) const;

	bool IsFinished(void);

protected:
	virtual void UpdateBarSize(float width, float height);

	cocos2d::Color4F _barColor;
	cocos2d::DrawNode *_progressDraw;
	cocos2d::DrawNode *_progressBorder;

	float _progress;
	float _maxSizeX, _maxSizeY;
};


#endif