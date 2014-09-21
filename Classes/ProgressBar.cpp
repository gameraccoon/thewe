#include "ProgressBar.h"

#include "Log.h"

ProgressBar::ProgressBar(float maxSizeX, float maxSizeY, cocos2d::Color4F color)
	: _progress(0.0f)
	, _maxSizeX(maxSizeX)
	, _maxSizeY(maxSizeY)
	, _barColor(color)
{
	init();
}

void ProgressBar::UpdateBarSize(float width, float height)
{
	cocos2d::Vec2 vertices[4];
	vertices[0] = cocos2d::Vec2(0.0f, 0.0f);
	vertices[1] = cocos2d::Vec2(width, 0.0f);
	vertices[2] = cocos2d::Vec2(width, height);
	vertices[3] = cocos2d::Vec2(0.0f, height);

	_progressDraw->clear();
	_progressDraw->drawPolygon(vertices, 4, _barColor, 0.0f, cocos2d::Color4F(0,0,0,0));
}

bool ProgressBar::init(void)
{
	cocos2d::Color4F c(1.0f, 0.0f, 0.0f, 1.0f);

	_progressDraw = cocos2d::DrawNode::create();
	_progressDraw->setPosition(0.0f, 0.0f);
	_progressBorder = cocos2d::DrawNode::create();
	_progressBorder->setPosition(0.0f, 0.0f);
	_progressBorder->drawSegment(cocos2d::Vec2(0.0f, _maxSizeY), cocos2d::Vec2(_maxSizeX, _maxSizeY), 1.0f, c);
	_progressBorder->drawSegment(cocos2d::Vec2(_maxSizeX, _maxSizeY), cocos2d::Vec2(_maxSizeX, 0.0f), 1.0f, c);
	_progressBorder->drawSegment(cocos2d::Vec2(_maxSizeX, 0.0f), cocos2d::Vec2(0.0f, 0.0f), 1.0f, c);
	_progressBorder->drawSegment(cocos2d::Vec2(0.0f, 0.0f), cocos2d::Vec2(0.0f, _maxSizeY), 1.0f, c);

	UpdateBarSize(_maxSizeX, _maxSizeY);

	addChild(_progressDraw, 0);
	addChild(_progressBorder, 1);

	return true;
}

void ProgressBar::SetProgress(float progress)
{	
	_progress = progress;

	if (_progress < 0.0f)
		_progress = 0.0f;
	if (_progress > 100.0f)
		_progress = 100.0f;

	UpdateBarSize(_maxSizeX * _progress / 100.0f, _maxSizeY);
}

float ProgressBar::GetMaxWidth(void) const
{
	return _maxSizeX;
}

float ProgressBar::GetMaxHeight(void) const
{
	return _maxSizeY;
}

float ProgressBar::GetCurrentProgress(void) const
{
	return _progress;
}

bool ProgressBar::IsFinished(void)
{
	return _progress >= 100.0f;
}