#include "ScreenBlackoutWidget.h"

#include "Log.h"

ScreenBlackoutWidget* ScreenBlackoutWidget::create(cocos2d::Color4F color)
{
	ScreenBlackoutWidget *ret = new ScreenBlackoutWidget();
	if (ret && ret->initWithColor(color))
	{
		ret->autorelease();
		return ret;
	}

	delete ret;
	return nullptr;
}

ScreenBlackoutWidget::ScreenBlackoutWidget(void)
{
}

ScreenBlackoutWidget::~ScreenBlackoutWidget(void)
{
	_mask->release();
}

void ScreenBlackoutWidget::AddSpot(cocos2d::Sprite *spot)
{
	for (Spots::const_iterator it = _spots.begin(); it != _spots.end(); ++it) {
		if ((*it) == spot) {
			WRITE_WARN("ScreenBlackoutWidget: Trying to add duplicated spot.");
			return;
		}
	}

	_spots.push_back(spot);
}

void ScreenBlackoutWidget::RemoveSpot(cocos2d::Sprite *spot, bool release)
{
	for (Spots::iterator it = _spots.begin(); it != _spots.end(); ++it) {
		if ((*it) == spot) {
			if (release) {(*it)->release();}
			it = _spots.erase(it);
			return;
		}
	}
}

void ScreenBlackoutWidget::ClearSpots(void)
{
	for (Spots::iterator it = _spots.begin(); it != _spots.end(); ++it) {
		(*it)->release();
	}
	_spots.clear();
}

void ScreenBlackoutWidget::SetBackgroundColor(const cocos2d::Color4F &color)
{
	_background = color;

	_mask->getSprite()->getGLProgramState()->setUniformVec4("u_backgroundColor",
		cocos2d::Vec4(color.r, color.g, color.b, color.a));
}

bool ScreenBlackoutWidget::initWithColor(cocos2d::Color4F color)
{
	_background = color;

	_maskProg = cocos2d::ShaderCache::getInstance()->getGLProgram("CreateAlphaMask");
	_drawProg = cocos2d::ShaderCache::getInstance()->getGLProgram("TutorialBlackout");

	cocos2d::Size size = cocos2d::Director::getInstance()->getWinSize();
	_mask = cocos2d::RenderTexture::create(size.width, size.height);
	_mask->getSprite()->setGLProgram(_drawProg);
	_mask->getSprite()->getGLProgramState()->setUniformVec4("u_backgroundColor",
		cocos2d::Vec4(color.r, color.g, color.b, color.a));
	_mask->retain();

	addChild(_mask);
	scheduleUpdate();

	return true;
}

void ScreenBlackoutWidget::update(float dt)
{
	if (_background.a <= 0.0f) {
		return;
	}

	_mask->beginWithClear(1.0f, 1.0f, 1.0f, 1.0f);
	for (auto spot : _spots) {
		spot->setGLProgram(_maskProg);
		spot->visit();
	}
	_mask->end();
}
