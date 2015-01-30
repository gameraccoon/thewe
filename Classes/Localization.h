#ifndef LOCALIZATION_H
#define LOCALIZATION_H

#include <string>
#include <map>

class LocalizationManager
{
public:
	static LocalizationManager& Instance();

	/**
	* Load the texts for the locale
	* Sets dafault locale if doesn't find requested locale
	*/
	void InitWithLocale(const std::string& localizationsXml, const std::string& locale);

	/**
	*  Return the word of determined locality
	*/
	std::string getText(const std::string& key);

private:
	std::string _currentLocale;
	std::map<std::string, std::string> _texts;

	LocalizationManager();
	~LocalizationManager();
	LocalizationManager(const LocalizationManager&);
	void operator=(const LocalizationManager&);
};

#endif //LOCALIZATION_H

