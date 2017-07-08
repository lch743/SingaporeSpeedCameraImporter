#pragma once 
/*------------------------------------------------------------------------------
                           GENERAL INCLUDES
------------------------------------------------------------------------------*/
#include <string>
#include <map>
#include <vector>
/*---------------------------------------------------------------------
                                TYPES
---------------------------------------------------------------------*/
#ifdef STRCOMPARER_EXPORTS
#define STRCOMPARER_API  __declspec(dllexport)
#else
#define STRCOMPARER_API __declspec(dllimport)
#endif
/*---------------------------------------------------------------------
                                CLASSES
---------------------------------------------------------------------*/
class STRCOMPARER_API StrComparer 
{
//---------------------------------------------------------------------
public: // FUNCTIONS
//-------------------------------------------------------------------
	StrComparer():
		__bReadAbbrConfig(false),
		__bReadReplaceConfig(false),
		__strLeft(""),
		__strRight(""),
		__strAbbrConfigFile("Abbr.cfg"),
		__strReplaceConfigFile("Replace.cfg"){};
	StrComparer(const std::string& strLeft, const std::string& strRight):
		__bReadAbbrConfig(false),
		__bReadReplaceConfig(false),
		__strLeft(strLeft),
		__strRight(strRight),
		__strAbbrConfigFile("Abbr.cfg"),
		__strReplaceConfigFile("Replace.cfg"){};
	void setLeft(const std::string& strLeft);
	void setRight(const std::string& strRight);
	bool compare();
//---------------------------------------------------------------------
private: // FUNCTION 
//--------------------------------------------------------------------
	bool __readAbbrConfig();
	bool __readReplaceConfig();
	bool __show();
//---------------------------------------------------------------------
private: // Data
//--------------------------------------------------------------------
	bool __bReadAbbrConfig;
	bool __bReadReplaceConfig;
	std::string __strLeft;	
	std::string __strRight;
	std::string __strAbbrConfigFile;
	std::string __strReplaceConfigFile;
	std::map<std::string, std::vector<std::string>> __mpWholeAbbrs;
	std::map<std::string, std::string> __mpReplaces;
};