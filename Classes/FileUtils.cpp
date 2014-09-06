#include "FileUtils.h"

#include <cocos2d.h>

std::string Utils::GetResourcesPath()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	return "../../Resources/";
#else
	return "./";
#endif
}
