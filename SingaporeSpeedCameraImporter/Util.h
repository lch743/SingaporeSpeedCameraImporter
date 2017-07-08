#pragma once
#include <string>
#include <vector>

#ifdef UTILITY_EXPORTS
#define UTILITY_API  __declspec(dllexport)
#else
#define UTILITY_API __declspec(dllimport)
#endif

#include "stdafx.h"
#include <assert.h>
#ifdef _DEBUG
#define DEBUG_CLIENTBLOCK   new( _CLIENT_BLOCK, __FILE__, __LINE__)
#else
#define DEBUG_CLIENTBLOCK
#endif
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifdef _DEBUG
#define new DEBUG_CLIENTBLOCK
#endif


class UTILITY_API ConfigTypeValue
{
public:
	std::string _strType;
	std::string _strValue;
public:
	ConfigTypeValue():
		_strType("Type"),
		_strValue("Value"){};
	ConfigTypeValue(const ConfigTypeValue& ctv);
	ConfigTypeValue(const std::string strType,
		            const std::string strValue):
		_strType(strType),
		_strValue(strValue){};
	ConfigTypeValue& operator=(const ConfigTypeValue& ctv);
};
UTILITY_API ConfigTypeValue getTypeValue(const std::string& str, const std::string& strTypeLeft, const std::string& strTypeRight);
UTILITY_API int StrCompareNoCase( std::string strSrc, std::string strDst );
UTILITY_API int StrFindNoCase(std::string strSrc, std::string strFind, int iPos = 0);
UTILITY_API std::string ReplaceStr(const std::string& strSrc, const std::string& strFind, const std::string& strRep);
UTILITY_API void SplitPathName( std::string strFileName, std::string& strPath, std::string& strName );
UTILITY_API std::string JoinPath(const std::string& strDir, const std::string& strFile);
UTILITY_API std::string GetCurDir();
UTILITY_API std::string WarningMessage( const std::string& strMeg );
UTILITY_API std::string ErrorMessage( const std::string& strMeg );
UTILITY_API void LogLog( const char* fmt, const char* strLogName = "Log.txt" ... );
UTILITY_API void LogLog( const std::string& fmt, const std::string& strLogName = "Log.txt" ... );
UTILITY_API void LogwoTime(const char* fmt, const char* strLogName = "Log.txt" ...);
UTILITY_API void LogwoTime(const std::string& fmt, const std::string& strLogName = "Log.txt" ...);
UTILITY_API int ParseLog( const std::string& strLogName );
UTILITY_API void PrnPgStatus( int iError, int iWarn, double dDone, double dTotal, const std::string& strTitle = "" );
UTILITY_API std::string CombineStr( const std::string& strSpliter, int iNum, ... );
UTILITY_API std::string CombineVectorStr(const std::vector<std::string>& vctStr, const std::string& strSpliter);
UTILITY_API std::string& ltrim( std::string &ss, std::string drop );
UTILITY_API std::string& rtrim( std::string &ss,  std::string drop);
UTILITY_API std::string& lrtrim( std::string &ss, std::string drop );
UTILITY_API std::vector< std::string > ParseStr( const std::string& strOrg, const std::string& strSpliter, const std::string& stEnd = "" );
UTILITY_API std::string GetSpliter(const std::string& str);
UTILITY_API int Str2Int( const std::string& str );
UTILITY_API double Str2Double( const std::string& str);
UTILITY_API int Chars2Num( const char* str );
UTILITY_API int psz2int(char* psz, int size);
UTILITY_API std::string Num2Str( int iNum );
UTILITY_API std::string Double2Str( double d, int i );
UTILITY_API std::string Chars2Str( char* chars );
UTILITY_API char* Str2Chars( const std::string& str );
UTILITY_API std::wstring String2Wstring(const std::string& str);
UTILITY_API std::string WString2String(const std::wstring& wstr);
UTILITY_API bool BCPIn( const std::string& strServer, const std::string& strDataBase, const std::string& strTable, const std::string& strFilePath, const std::string& strSpliter );
UTILITY_API bool BCPOut( const std::string& strServer, const std::string& strDataBase, const std::string& strTable, const std::string& strFilePath, const std::string& strSpliter );
UTILITY_API bool FileIsEmpty( const std::string& strFileName );
UTILITY_API bool ChkFileExist( const std::string& strFileName );
UTILITY_API bool ChkFileCanOpen( const std::string& strFileName );
UTILITY_API bool ChkFolderExist( const std::string& strFolderName );
UTILITY_API bool CopyFileFolder( const std::string& strFileNameFrm, const std::string& strFileNameTag );
UTILITY_API bool CopyFileM( const std::string& strFileNameFrm, const std::string& strFileNameTag );
UTILITY_API bool DeleteFileM( const std::string& strFileName );
UTILITY_API bool CreateFolder( const std::string& strFolderName );
UTILITY_API bool DeleteFolder( const std::string& strFolderName );
UTILITY_API bool CreateFileM( const std::string& strFileName );
UTILITY_API bool ClearFolder( const std::string& strFodlerName );
UTILITY_API bool GetFolderFiles( const std::string& strFolderName, std::vector< std::string >& vctFiles );
UTILITY_API std::vector< std::string > GetFolderFilesContain( const std::string& strFolderName, std::vector< std::string >&  vctFiles, std::string strName );
UTILITY_API int GetFolderFilesCount( const std::string& strFolderName );
UTILITY_API std::string GetFileFolder( const std::string& strFileName );
UTILITY_API std::string FolderFormatSlash( const std::string& strFolderName );
UTILITY_API std::string FolderFormatNoSlash( const std::string& strFolderName );
UTILITY_API std::string GetFileName( const std::string& strFileName );
UTILITY_API std::string GetFileNameWoSuffix(const std::string& strFileName);
UTILITY_API std::string GetFileNameSuffix(const std::string& strFileName);
UTILITY_API bool FindStrInFile( std::string strFileName, std::string strKey );
UTILITY_API std::string Lower( std::string& str );
UTILITY_API std::string Upper( std::string& str );
UTILITY_API std::string Dec2HexLittle( int dec );
UTILITY_API std::string Dec2HexHigh( int dec );
UTILITY_API int Hex2Dec( std::string hex );
UTILITY_API std::string GenTYPName( int iFamilyId );
UTILITY_API std::string GetSQLCheckerFromCodePage(int iCodePage);
UTILITY_API bool CheckItemInVector(int item, const std::vector<int> vct);
UTILITY_API bool CheckItemInVector(double item, const std::vector<double> vct);
UTILITY_API bool CheckItemInVector(const std::string& item, const std::vector<std::string> vct);
//Binary File Function
UTILITY_API int getfilebits( std::ifstream& ifin );
UTILITY_API void getbits( std::ifstream& ifin, int iBegin, int iEnd, char* cRcrd );
UTILITY_API void Setbits( std::ofstream& ofout, int iBegin, int iEnd, char* cRcrd );
UTILITY_API int getbitsInt( std::ifstream& ifin, int iBegin, int iEnd );
UTILITY_API void setbitsInt( std::ofstream& ofout, int iBegin, int iEnd, int Num );
UTILITY_API char getbitsChar( std::ifstream& ifin, int iBegin, int iEnd );
UTILITY_API void setbitsChar( std::ofstream& ofout, int iBegin, int iEnd, char cData );
UTILITY_API void setbitsStr( std::ofstream& ofout, int iBegin, int iEnd, std::string strData );
UTILITY_API void setbitsZero( std::ofstream& ofout, int iBegin );
UTILITY_API std::string getbitsStr( std::ifstream& ifin, int iBegin, int iEnd );
UTILITY_API int getbitsNextZeroPos( std::ifstream& ifin, int iBegin );
UTILITY_API void copybits( std::ifstream& ifin, std::ofstream& ofout, int iBegin, int iEnd, int iDelta );
//Time
struct tm;
UTILITY_API tm* getCurrentTime();