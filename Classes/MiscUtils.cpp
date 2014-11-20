#include "MiscUtils.h"

#include <cocos2d.h>
#include <string>

#include "World.h"

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
	return cocos2d::FileUtils::getInstance()->getWritablePath();
}

std::string Utils::GetWritablePath()
{
	return cocos2d::FileUtils::getInstance()->getWritablePath();
}

Utils::GameTime Utils::GetGameTime(void)
{
	return World::Instance().GetTimeCurrent();
}

std::string Utils::TimeToString(Utils::GameTime time)
{
	return std::to_string(time);
}

Utils::GameTime Utils::StringToTime(const std::string& stringTime)
{
	return std::stof(stringTime);
}
