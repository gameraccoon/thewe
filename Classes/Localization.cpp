#include "Localization.h"

#include <memory>
#include <functional>

#include "pugixml.hpp"
#include "cocos2d.h"

#include "Log.h"
#include "MiscUtils.h"

#include "GameInfo.h"

LocalizationManager::LocalizationManager()
{
	
}

LocalizationManager::~LocalizationManager()
{
}

LocalizationManager& LocalizationManager::Instance()
{
	static LocalizationManager singleInstance;
	return singleInstance;
}

void LocalizationManager::InitWithLocale(const std::string& localizationFile, const std::string& locale)
{
	pugi::xml_parse_result parsedXml;

	pugi::xml_document xmlDocument;

	pugi::xml_node headNode;
	pugi::xml_node childNode;

	cocos2d::FileUtils * fileUtils = cocos2d::FileUtils::getInstance();
	std::string fullName = fileUtils->fullPathForFilename(localizationFile);
	std::string fileBuff = fileUtils->getStringFromFile(fullName);

	parsedXml = xmlDocument.load_buffer(fileBuff.c_str(), fileBuff.size());

	// XML statistics
	headNode = xmlDocument.child("office:document-content").child("office:body").child("office:spreadsheet").find_child_by_attribute("table:name", "Sheet1");
	childNode = headNode.child("table:table-row");

	int currentPos = 0; // "Default" column
	int localePos = 0;

	for (auto locale_node = childNode.child("table:table-cell"); locale_node; locale_node = locale_node.next_sibling())
	{
		if (locale_node.child("text:p").text().as_string() == locale)
		{
			localePos = currentPos;
			break;
		}
		currentPos++;
	}

	// if no locale found
	if (localePos == 0)
	{
		std::string defaultLocale = GameInfo::Instance().GetString("DEFAULT_LOCALE");
		if (locale != defaultLocale)
		{
			Log::Instance().writeLog(std::string("Locale \"").append(locale).append("\" not found. Trying to use default locale"));
			// Warning: recursive call
			InitWithLocale(localizationFile, defaultLocale);
			return;
		}
		else
		{
			Log::Instance().writeError(std::string("Default locale \"").append(locale).append("\" not found."));
			return;
		}
	}

	childNode = childNode.next_sibling(); // load XML records (rows)

	while (childNode)
	{
		auto locale_node = childNode.child("table:table-cell");
		std::pair<std::string, std::string> localeWord;

		localeWord.first = locale_node.child("text:p").text().as_string(); // key

		// finding locale by Locale_pos
		for (int j = 0; j < localePos; j++)
		{
			locale_node = locale_node.next_sibling();
		}

		localeWord.second = locale_node.child("text:p").text().as_string(); // word

		_texts.insert(localeWord);

		childNode = childNode.next_sibling(); // next row
	}
}

std::string LocalizationManager::getText(const std::string& key)
{
	auto it = _texts.find(key);
	return (it != _texts.end() ? it->second : key);
}
