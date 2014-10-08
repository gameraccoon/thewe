#ifndef MISC_UTILS_H
#define MISC_UTILS_H

#include <string>

namespace Utils
{
	typedef float GameTime;

	std::string GetResourcesPath();
	std::string GetDocumentsPath();
	std::string GetWritablePath();

	GameTime GetGameTime(void);
}

#endif // FILE_UTILS_H
