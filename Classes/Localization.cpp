#include "Localization.h"



/*
	Initiliazing of new localization object and load the localization file
*/
Localization::Localization()
{
	
}

Localization::~Localization()
{
}

bool Localization::InitWithLocale(const char * Locale)
{	
	ssize_t * XMLSourceSize = new ssize_t();

	pugi::xml_parse_result * parsedXML;
	pugi::xml_node  Head_node, Child_node;
	int  Loc_count, WordsPos_count;

	parsedXML = new pugi::xml_parse_result();

	unsigned char * XMLSource = nullptr;
	
	pugi::xml_document * XMLDoc = new pugi::xml_document();

	auto fileUtil = cocos2d::FileUtils::getInstance();
	XMLSource = // unload XML from ODS to buffer
		fileUtil->getFileDataFromZip("localization.ods", "content.xml", XMLSourceSize);
	fileUtil->destroyInstance();

	*parsedXML = XMLDoc->load_buffer(XMLSource, *XMLSourceSize);

	Head_node = XMLDoc->child("office:document-content").child("office:body").child("office:spreadsheet").find_child_by_attribute("table:name", "Sheet1");
	Child_node = Head_node.child("table:table-row");
	WordsPos_count = Child_node.child("table:table-cell").attribute("office:value").as_int();
	Child_node = Child_node.next_sibling();
	Loc_count = Child_node.child("table:table-cell").attribute("office:value").as_int();

	Child_node = Child_node.next_sibling();

	//auto y = Child_node.child("table:table-cell").value();

	//for (pugi::xml_node child = Head_node.child("table:table-row"); child; child = child.next_sibling("table:table-row"))
		//break;

		//LocalizationTable.insert(std::pair<std::string, std::string>(child.name, child.text));
		//pugi::xml_node Localization_info = Head_node.find_child_by_attribute("table:style-name", "ro2");	

	delete XMLSourceSize;
	return true;
}




