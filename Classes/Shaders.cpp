#include "Application.h"

#include "Log.h"

bool AppDelegate::CreateAllShaders(void)
{
	std::string log;

	cocos2d::GLProgram *blackout;
	cocos2d::GLProgram *alphaMask;

	alphaMask = cocos2d::GLProgram::createWithFilenames("create_alpha_mask.vsh", "create_alpha_mask.fsh");
	alphaMask->bindAttribLocation(cocos2d::GLProgram::ATTRIBUTE_NAME_POSITION, cocos2d::GLProgram::VERTEX_ATTRIB_POSITION);
	alphaMask->bindAttribLocation(cocos2d::GLProgram::ATTRIBUTE_NAME_TEX_COORD, cocos2d::GLProgram::VERTEX_ATTRIB_TEX_COORD);
	if (!alphaMask->link()) {
		log = alphaMask->getProgramLog();
		Log::Instance().writeError(log);
		return false;
	}
	alphaMask->updateUniforms();
	alphaMask->retain();

	blackout = cocos2d::GLProgram::createWithFilenames("tutorial_blackout.vsh", "tutorial_blackout.fsh");
	blackout->bindAttribLocation(cocos2d::GLProgram::ATTRIBUTE_NAME_POSITION, cocos2d::GLProgram::VERTEX_ATTRIB_POSITION);
	blackout->bindAttribLocation(cocos2d::GLProgram::ATTRIBUTE_NAME_TEX_COORD, cocos2d::GLProgram::VERTEX_ATTRIB_TEX_COORD);
	if (!blackout->link()) {
		log = blackout->getProgramLog();
		Log::Instance().writeError(log);
		return false;
	}
	blackout->updateUniforms();
	blackout->retain();

	cocos2d::ShaderCache::getInstance()->addGLProgram(blackout, "TutorialBlackout");
	cocos2d::ShaderCache::getInstance()->addGLProgram(alphaMask, "CreateAlphaMask");

	return true;
}