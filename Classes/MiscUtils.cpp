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
		_intSet[name] = value;
	}

	void VariablesSet::SetTime(const std::string &name, Utils::GameTime value)
	{
		_timeSet[name] = value;
	}

	void VariablesSet::SetString(const std::string &name, const std::string& value)
	{
		_strSet[name] = value;
	}

	bool VariablesSet::HaveInt(const std::string &name) const
	{
		return _intSet.find(name) != _intSet.end();
	}

	bool VariablesSet::HaveFloat(const std::string &name) const
	{
		return _floatSet.find(name) != _floatSet.end();
	}

	bool VariablesSet::HaveBool(const std::string &name) const
	{
		return _intSet.find(name) != _intSet.end();
	}

	bool VariablesSet::HaveTime(const std::string &name) const
	{
		return _timeSet.find(name) != _timeSet.end();
	}

	bool VariablesSet::HaveString(const std::string &name) const
	{
		return _strSet.find(name) != _strSet.end();
	}

	int VariablesSet::GetInt(const std::string &name) const
	{
		const auto it = _intSet.find(name);
		if (it != _intSet.end())
		{
			return it->second;
		}
		else
		{
			Log::Instance().writeError("Trying to access to unknown int variable \"" + name + "\" in VariablesSet");
			return -1;
		}
	}

	float VariablesSet::GetFloat(const std::string &name) const
	{
		const auto it = _floatSet.find(name);
		if (it != _floatSet.end())
		{
			return it->second;
		}
		else
		{
			Log::Instance().writeError("Trying to access to unknown float variable \"" + name + "\" in VariablesSet");
			return -1;
		}
	}

	bool VariablesSet::GetBool(const std::string &name) const
	{
		const auto it = _intSet.find(name);
		if (it != _intSet.end())
		{
			return it->second != 0;
		}
		else
		{
			Log::Instance().writeError("Trying to access to unknown bool variable \"" + name + "\" in VariablesSet");
			return false;
		}
	}

	Utils::GameTime VariablesSet::GetTime(const std::string &name) const
	{
		const auto it = _timeSet.find(name);
		if (it != _timeSet.end())
		{
			return it->second;
		}
		else
		{
			Log::Instance().writeError("Trying to access to unknown time variable \"" + name + "\" in VariablesSet");
			return 0;
		}
	}

	std::string VariablesSet::GetString(const std::string &name) const
	{
		const auto it = _strSet.find(name);
		if (it != _strSet.end())
		{
			return it->second;
		}
		else
		{
			Log::Instance().writeError("Trying to access to unknown string variable \"" + name + "\" in VariablesSet");
			return "";
		}
	}

} // namespace Utils
