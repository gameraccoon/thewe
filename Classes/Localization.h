#ifndef LOCALIZATION_H
#define LOCALIZATION_H

#include <string>
#include <map>

class LocalizationManager
{
public:
	static LocalizationManager& Instance();

	/**
	*  Load the localization with locality
	*/
	bool InitWithLocale(const char * locality);

	/**
	*  Return the word of determined locality
	*/
	std::string getText(const char * key);

private:
	std::string _currentLocale;
	std::map<std::string, std::string> _texts;

	LocalizationManager();
	~LocalizationManager();
	LocalizationManager(const LocalizationManager&);
	void operator=(const LocalizationManager&);
};

#endif //LOCALIZATION_H

