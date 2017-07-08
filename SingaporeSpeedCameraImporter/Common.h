#ifndef COMMON_API_H
#define COMMON_API_H
#pragma warning(disable:4251)
#ifdef _M_AMD64
#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF", "EndOfFile")
//#import <MSJRO.DLL> no_namespace rename("ReplicaTypeEnum", "_ReplicaTypeEnum")
//#pragma message("this is win64")
#else
#import "C:\Program Files (x86)\Common Files\System\ado\msado15.dll" no_namespace rename("EOF", "EndOfFile")
//#pragma message("this is win32")
//#pragma message("this is win32")
#endif
//using namespace ADODB;

#include "stdafx.h"
#include <assert.h>
#include <string>
#include <vector>
#ifdef _DEBUG
#define DEBUG_CLIENTBLOCK   new( _CLIENT_BLOCK, __FILE__, __LINE__)
#else
#define DEBUG_CLIENTBLOCK
#endif
#define MAX_NUM 1024
//#define _CRTDBG_MAP_ALLOC
//#include <crtdbg.h>
//#ifdef _DEBUG
//#define new DEBUG_CLIENTBLOCK
//#endif

#ifdef DATABASE_EXPORTS
#define DATABASE_API  __declspec(dllexport)
#else
#define DATABASE_API __declspec(dllimport)
#endif

DATABASE_API std::string string_format(const std::string fmt_str, ...);
DATABASE_API std::vector<std::string> split(const std::string& strSrc, const std::string& strSpliter);
DATABASE_API std::string replaceAll(const std::string& strSrc, std::string strOrg, std::string strRep);
DATABASE_API BSTR ConvertMBSToBSTR(const std::string& str);
DATABASE_API int strFindNoCase(std::string strSrc, std::string strFind, int iPos = 0);
#endif
