#ifndef LOCALIZATION_H
#define LOCALIZATION_H

#include <memory>
#include "pugixml.hpp"
#include "cocos2d.h"
#include <string>
#include <functional>


class Localization
{
private:	
	std::string current_Locale;
	std::map<std::string, std::string> LocalizationTable;
	
public:
	Localization();
	virtual ~Localization();

	/**
	*  Load the localization with locality
	*/
	bool InitWithLocale(const char * locality);

	/**
	*  Return the word of determined locality
	*/
	std::string getText(const char * key);



};

#endif //LOCALIZATION_H

