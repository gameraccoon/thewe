#ifndef SETTINGS_MANAGER_H
#define SETTINGS_MANAGER_H

#include <pugixml.hpp>

class SettingsManager
{
public:
	struct Settings
	{
		int data;
	};

public:
	static SettingsInstance& Instance(void);

	pugi::xml_node FindXmlNodeByName(const std::string &name);

	void SetMusicVolume(float volume);
	void SetSoundVolume(float volume);
};

#endif