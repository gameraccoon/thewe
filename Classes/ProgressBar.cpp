#include "ProgressBar.h"

#include "Log.h"

SquareProgressBar::SquareProgressBar(float maxSizeX, float maxSizeY, cocos2d::Color4F color)
	: _progress(0.0f)
	, _maxSizeX(maxSizeX)
	, _maxSizeY(maxSizeY)
	, _barColor(color)
{
	init();
}

void SquareProgressBar::UpdateBarSize(float width, float height)
{
	cocos2d::Vec2 vertices[4];
	vertices[0] = cocos2d::Vec2(0.0f, 0.0f);
	vertices[1] = cocos2d::Vec2(width, 0.0f);
	vertices[2] = cocos2d::Vec2(width, height);
	vertices[3] = cocos2d::Vec2(0.0f, height);

	_progressDraw->clear();
	_progressDraw->drawPolygon(vertices, 4, _barColor, 0.0f, cocos2d::Color4F(0,0,0,0));
}

bool SquareProgressBar::init(void)
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

void SquareProgressBar::SetProgressPercentage(float progress)
{	
	_progress = progress;

	if (_progress < 0.0f)
		_progress = 0.0f;
	if (_progress > 100.0f)
		_progress = 100.0f;

	UpdateBarSize(_maxSizeX * _progress / 100.0f, _maxSizeY);
}

float SquareProgressBar::GetMaxWidth(void) const
{
	return _maxSizeX;
}

float SquareProgressBar::GetMaxHeight(void) const
{
	return _maxSizeY;
}

float SquareProgressBar::GetCurrentProgress(void) const
{
	return _progress;
}

bool SquareProgressBar::IsFinished(void)
{
	return _progress >= 100.0f;
}

// RoundProgressBar implementation

RoundProgressBar::RoundProgressBar(const std::string &roundTexture, float scale, float alpha)
	: _roundTexturName(roundTexture)
	, _scale(scale)
	, _alpha(alpha)
{
	init();
}

bool RoundProgressBar::init(void)
{
	_roundTexture = cocos2d::Sprite::create(_roundTexturName);
	_progressTimer = cocos2d::ProgressTimer::create(_roundTexture);
	_progressTimer->setPosition(0.0f, 0.0f);
	_progressTimer->setScale(1.0f);
	_progressTimer->setOpacity((int)(255.0f * _alpha));
	_progressTimer->setType(cocos2d::ProgressTimer::Type::RADIAL);
	_progressTimer->setPercentage(0.0f);

	addChild(_progressTimer, 0);

	return true;
}

void RoundProgressBar::SetProgressAnimated(float progress, float duration)
{
	cocos2d::ProgressFromTo *p = cocos2d::ProgressFromTo::create(duration, _progressTimer->getPercentage(), progress);
	_progressTimer->runAction(p);
}

void RoundProgressBar::SetProgressImmediately(float progress)
{
	_progressTimer->setPercentage(progress);
}

float RoundProgressBar::GetCurrentProgress(void)
{
	return _progressTimer->getPercentage();
}

bool RoundProgressBar::IsFinished(void) const
{
	return _progressTimer->getPercentage() >= 100.0f;
}