#ifndef GAME_INFO_H
#define GAME_INFO_H

#include <string>
#include <map>

#include "MiscUtils.h"

class GameInfo
{
public:
	static GameInfo& Instance(void);

public:
	bool ParseXml(const std::string &filename);

	int GetInt(const std::string &name, int def = 0) const;
	bool GetBool(std::string &name, bool def = false) const;
	float GetFloat(const std::string &name, float def = 0.0f) const;
	std::string GetString(const std::string &name, std::string def = " ") const;
	Utils::GameTime GetTime(const std::string &time, Utils::GameTime def = 0) const;

private:
	std::map<std::string, int> _intMap;
	std::map<std::string, float> _floatMap;
	std::map<std::string, std::string> _strMap;
	std::map<std::string, Utils::GameTime> _timeMap;
};

#endif