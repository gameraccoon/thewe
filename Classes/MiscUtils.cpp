#include "MiscUtils.h"

#include <cocos2d.h>
#include <string>

#include "World.h"

namespace Utils
{
	std::string GetResourcesPath()
	{
	#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
		return "../../Resources/";
	#else
		return "";
	#endif
	}

	std::string GetDocumentsPath()
	{
		return cocos2d::FileUtils::getInstance()->getWritablePath();
	}

	std::string GetWritablePath()
	{
		return cocos2d::FileUtils::getInstance()->getWritablePath();
	}

	GameTime GetGameTime(void)
	{
		return World::Instance().GetTimeCurrent();
	}

	std::string TimeToString(GameTime time)
	{
		return std::to_string(time);
	}

	GameTime StringToTime(const std::string& stringTime)
	{
		return std::stof(stringTime);
	}

	bool IsPlatformDesktop()
	{
		cocos2d::Application::Platform platform = cocos2d::Application::getInstance()->getTargetPlatform();
		return platform == cocos2d::Application::Platform::OS_WINDOWS
				||
				platform == cocos2d::Application::Platform::OS_LINUX
				||
				platform == cocos2d::Application::Platform::OS_MAC;
	}
} // namespace Utils
