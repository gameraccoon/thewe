#include "Localization.h"

#include <memory>
#include <functional>

#include "pugixml.hpp"
#include "cocos2d.h"

#include "Log.h"
#include "MiscUtils.h"

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

bool LocalizationManager::InitWithLocale(const char * localizationFile, const char * Locale)
{	
	unsigned char * xmlSource;
	ssize_t xmlSourceSize;

	pugi::xml_parse_result parsedXml;

	pugi::xml_document xmlDocument;

	pugi::xml_node headNode;
	pugi::xml_node childNode;

	int	localeCount;	// localizations count (columns = DefalutLocColumn + others)
	int positionsCount;	// positions count (rows)

	cocos2d::FileUtils * fileUtils = cocos2d::FileUtils::getInstance();

	// unload XML from ODS to buffer
	xmlSource = fileUtils->getFileDataFromZip(fileUtils->fullPathForFilename(localizationFile), "content.xml", &xmlSourceSize);
	if (!xmlSource)
	{
		Log::Instance().writeWarning(std::string("File \"").append(localizationFile).append("\" not found"));
	}

	parsedXml = xmlDocument.load_buffer(xmlSource, xmlSourceSize);

	// XML statistics
	headNode = xmlDocument.child("office:document-content").child("office:body").child("office:spreadsheet").find_child_by_attribute("table:name", "Sheet1");
	childNode = headNode.child("table:table-row");
	positionsCount = childNode.child("table:table-cell").attribute("office:value").as_int();
	localeCount = childNode.child("table:table-cell").next_sibling().attribute("office:value").as_int();

	childNode = childNode.next_sibling(); // load XML columns

	int currentPos = 0; // "Default" column
	int localePos = 0;

	for (auto locale_node = childNode.child("table:table-cell").next_sibling(); locale_node; locale_node = locale_node.next_sibling())
	{
		currentPos++;
		if (locale_node.child("text:p").text().as_string() == std::string(Locale))
		{
			localePos = currentPos;
			break;
		}				
	}
	if (!localePos) return false; // if no Locale

	childNode = childNode.next_sibling(); // load XML records (rows)

	for (int i = 0; i < positionsCount; i++)
	{
		auto locale_node = childNode.child("table:table-cell");
		std::pair<std::string, std::string> localeWord;

		localeWord.first = locale_node.child("text:p").text().as_string(); // key

		// finding locale by Locale_pos
		for (int j = 0; j < localePos; j++) locale_node = locale_node.next_sibling();

		localeWord.second = locale_node.child("text:p").text().as_string(); // word

		_texts.insert(localeWord);

		childNode = childNode.next_sibling(); // next row
	}

	return true;
}

std::string LocalizationManager::getText(const char * key)
{
	auto it = _texts.find(std::string(key));
	return (it != _texts.end() ? it->second : std::string(key));
}
