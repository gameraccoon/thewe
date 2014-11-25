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

	VariablesSet::VariablesSet(void)
	{
	}

	void VariablesSet::ClearAll(void)
	{
		_intSet.clear();
		_floatSet.clear();
		_timeSet.clear();
		_strSet.clear();
	}

	void VariablesSet::SetInt(const std::string &name, int value)
	{
		_intSet[name] = value;
	}

	void VariablesSet::SetFloat(const std::string &name, float value)
	{
		_floatSet[name] = value;
	}

	void VariablesSet::SetBool(const std::string &name, bool value)
	{
		_timeSet[name] = value;
	}

	void VariablesSet::SetTime(const std::string &name, Utils::GameTime value)
	{
		_timeSet[name] = value;
	}

	void VariablesSet::SetString(const std::string &name, const std::string &value)
	{
		_strSet[name] = value;
	}

	int VariablesSet::GetInt(const std::string &name, int def) const
	{
		std::map<std::string, int>::const_iterator it;
		it = _intSet.find(name);
		return it != _intSet.end() ? (*it).second : def;
	}

	float VariablesSet::GetFloat(const std::string &name, float def) const
	{
		std::map<std::string, float>::const_iterator it;
		it = _floatSet.find(name);
		return it != _floatSet.end() ? (*it).second : def;
	}

	bool VariablesSet::GetBool(const std::string &name, bool def) const
	{
		std::map<std::string, int>::const_iterator it;
		it = _intSet.find(name);
		return it != _intSet.end() ? (*it).second != 0 : def;
	}

	Utils::GameTime VariablesSet::GetTime(const std::string &name, Utils::GameTime def) const
	{
		std::map<std::string, Utils::GameTime>::const_iterator it;
		it = _timeSet.find(name);
		return it != _timeSet.end() ? (*it).second : def;
	}

	std::string VariablesSet::GetString(const std::string &name, std::string def) const
	{
		std::map<std::string, std::string>::const_iterator it;
		it = _strSet.find(name);
		return it != _strSet.end() ? (*it).second : def;
	}

} // namespace Utils
