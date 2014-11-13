#include "Localization.h"




LocalizationManager::LocalizationManager()
{
	
}

LocalizationManager::~LocalizationManager()
{
}

bool LocalizationManager::InitWithLocale(const char * Locale)
{	
	unsigned char * XMLSource;
	ssize_t  XMLSourceSize;

	pugi::xml_parse_result parsedXML;

	pugi::xml_document  XMLDoc;

	pugi::xml_node Head_node;
	pugi::xml_node Child_node;

	int	Loc_count,	// localizations count (columns = DefalutLocColumn + others)
	int Pos_count;	// positions count (rows)	

	cocos2d::FileUtils * fileUtil = cocos2d::FileUtils::getInstance();

	// unload XML from ODS to buffer
	XMLSource = fileUtil->getFileDataFromZip("localization.ods", "content.xml", &XMLSourceSize);
	fileUtil->destroyInstance();

	parsedXML = XMLDoc.load_buffer(XMLSource, XMLSourceSize);

	// XML statistics
	Head_node = XMLDoc.child("office:document-content").child("office:body").child("office:spreadsheet").find_child_by_attribute("table:name", "Sheet1");
	Child_node = Head_node.child("table:table-row");
	Pos_count = Child_node.child("table:table-cell").attribute("office:value").as_int();
	Loc_count = Child_node.child("table:table-cell").next_sibling().attribute("office:value").as_int();

	Child_node = Child_node.next_sibling(); // load XML columns

	std::vector<std::string> LocaleSet;

	int Current_pos = 0; // "Default" column
	int Locale_pos = 0;

	for (auto locale_node = Child_node.child("table:table-cell").next_sibling(); locale_node; locale_node = locale_node.next_sibling())
	{
		Current_pos++;
		if (locale_node.child("text:p").text().as_string() == std::string(Locale))
		{
			Locale_pos = Current_pos;
			break;
		}				
	}
	if (!Locale_pos) return false; // if no Locale

	Child_node = Child_node.next_sibling(); // load XML records (rows)

	for (int i = 0; i < Pos_count; i++)
	{
		auto locale_node = Child_node.child("table:table-cell");
		std::pair<std::string, std::string> LocaleWord;

		LocaleWord.first = locale_node.child("text:p").text().as_string(); // key

		// finding locale by Locale_pos
		for (int j = 0; j < Locale_pos; j++) locale_node = locale_node.next_sibling();

		LocaleWord.second = locale_node.child("text:p").text().as_string(); // word

		LocalizationTable.insert(LocaleWord);	

		Child_node = Child_node.next_sibling(); // next row
	}

	return true;
}

std::string LocalizationManager::getText(const char * key)
{
	auto it = LocalizationTable.find(std::string(key));
	return (it != LocalizationTable.end() ? it->second : std::string(key));
}




