#include "FileUtils.h"

#include <cocos2d.h>
#include "Log.h"

std::string Utils::GetResourcesPath()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	return "../../Resources/";
#else
	return "";
#endif
}

std::string Utils::GetDocumentsPath()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	return "../../Resources/saves/";
#else
	return cocos2d::FileUtils::getInstance()->getWritablePath();
#endif
}

std::string Utils::GetWritablePath()
{
	return cocos2d::FileUtils::getInstance()->getWritablePath();
}
