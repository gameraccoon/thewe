#include "Localization.h"




Localization::Localization()
{
	
}

Localization::~Localization()
{
}

bool Localization::InitWithLocale(const char * Locale)
{	
	unsigned char * XMLSource;
	ssize_t  XMLSourceSize;

	pugi::xml_parse_result parsedXML;
	pugi::xml_node  
			Head_node, 
			Child_node;
	int  
		Loc_count, 
		Pos_count;		
	
	pugi::xml_document * XMLDoc = new pugi::xml_document();

	auto fileUtil = cocos2d::FileUtils::getInstance();
	XMLSource = // unload XML from ODS to buffer
		fileUtil->getFileDataFromZip("localization.ods", "content.xml", &XMLSourceSize);
	fileUtil->destroyInstance();

	parsedXML = XMLDoc->load_buffer(XMLSource, XMLSourceSize);

	// XML statistics
	Head_node = XMLDoc->child("office:document-content").child("office:body").child("office:spreadsheet").find_child_by_attribute("table:name", "Sheet1");
	Child_node = Head_node.child("table:table-row");
	Pos_count = Child_node.child("table:table-cell").attribute("office:value").as_int();
	Loc_count = Child_node.child("table:table-cell").next_sibling().attribute("office:value").as_int();

	Child_node = Child_node.next_sibling(); // load XML columns

	std::vector<std::string> LocaleSet;

	int Locale_pos = 0; // Default

	for (auto locale_node = Child_node.child("table:table-cell").next_sibling(); locale_node; locale_node = locale_node.next_sibling())
	{
		Locale_pos++;
		if (locale_node.child("text:p").text().as_string() == std::string(Locale))
			break;		
	}

	if (!Locale_pos) // if no Locale
		return false;

	Child_node = Child_node.next_sibling(); // load XML data

	for (int i = 0; i < Pos_count; i++)
	{
		auto locale_node = Child_node.child("table:table-cell");
		std::pair<std::string, std::string> LocaleWord;
		LocaleWord.first = locale_node.child("text:p").text().as_string(); // key
		for (int j = 0; j < Locale_pos; j++) // finding locale by Locale_pos
			locale_node = locale_node.next_sibling();
		LocaleWord.second = locale_node.child("text:p").text().as_string(); // word
		LocalizationTable.insert(LocaleWord);	
		Child_node = Child_node.next_sibling(); // next row
	}

	return true;
}

std::string Localization::getText(const char * key)
{
	auto it = LocalizationTable.find(std::string(key));
	if (it != LocalizationTable.end())
		return it->second;
	else
		return std::string(key);
}




