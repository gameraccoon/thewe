#ifndef MISC_UTILS_H
#define MISC_UTILS_H

#include <string>
#include <ctime>

namespace Utils
{
	typedef float GameTime;

	std::string GetResourcesPath();
	std::string GetDocumentsPath();
	std::string GetWritablePath();

	GameTime GetGameTime(void);
	std::string TimeToString(GameTime time);
	GameTime StringToTime(const std::string& stringTime);

	bool IsPlatformDesktop();
}

#endif // MISC_UTILS_H
