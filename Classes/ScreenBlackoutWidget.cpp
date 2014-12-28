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
	_maskProg->release();
	_drawProg->release();
}

void ScreenBlackoutWidget::AddSpot(cocos2d::Sprite *spot)
{
	for (Spots::const_iterator it = _spots.begin(); it != _spots.end(); ++it) {
		if ((*it) == spot) {
			Log::Instance().writeWarning("ScreenBlackoutWidget: Trying to add duplicated spot.");
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

bool ScreenBlackoutWidget::initWithColor(cocos2d::Color4F color)
{
	_maskProg = cocos2d::GLProgram::createWithFilenames("create_alpha_mask.vsh", "create_alpha_mask.fsh");
	_maskProg->bindAttribLocation(cocos2d::GLProgram::ATTRIBUTE_NAME_POSITION, cocos2d::GLProgram::VERTEX_ATTRIB_POSITION);
	_maskProg->bindAttribLocation(cocos2d::GLProgram::ATTRIBUTE_NAME_TEX_COORD, cocos2d::GLProgram::VERTEX_ATTRIB_TEX_COORD);
	_maskProg->bindAttribLocation(cocos2d::GLProgram::ATTRIBUTE_NAME_COLOR, cocos2d::GLProgram::VERTEX_ATTRIB_COLOR);
	_maskProg->link();
	_maskProg->updateUniforms();
	_maskProg->retain();

	_drawProg = cocos2d::GLProgram::createWithFilenames("tutorial_blackout.vsh", "tutorial_blackout.fsh");
	_drawProg->bindAttribLocation(cocos2d::GLProgram::ATTRIBUTE_NAME_POSITION, cocos2d::GLProgram::VERTEX_ATTRIB_POSITION);
	_drawProg->bindAttribLocation(cocos2d::GLProgram::ATTRIBUTE_NAME_TEX_COORD, cocos2d::GLProgram::VERTEX_ATTRIB_TEX_COORD);
	_drawProg->bindAttribLocation(cocos2d::GLProgram::ATTRIBUTE_NAME_COLOR, cocos2d::GLProgram::VERTEX_ATTRIB_COLOR);
	_drawProg->link();
	_drawProg->updateUniforms();
	_drawProg->retain();

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
	_mask->beginWithClear(1.0f, 1.0f, 1.0f, 1.0f);
	for (auto spot : _spots) {
		spot->setGLProgram(_maskProg);
		spot->visit();
	}
	_mask->end();
}