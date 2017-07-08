#ifndef ERROR_API_H
#define ERROR_API_H
/*------------------------------------------------------------------------------
                           GENERAL INCLUDES
------------------------------------------------------------------------------*/
#include "Common.h"
/*---------------------------------------------------------------------
                                TYPES
---------------------------------------------------------------------*/
/*---------------------------------------------------------------------
                                CLASSES
---------------------------------------------------------------------*/
class DATABASE_API GError 
{
//---------------------------------------------------------------------
public: // FUNCTIONS
//--------------------------------------------------------------------
	GError();
	~GError();
	void reportException(_com_error &e);
	std::string getLastError();
	HRESULT getlastrHRError();
//---------------------------------------------------------------------
protected: // FUNCTIONS
//-------------------------------------------------------------------
	std::string logCrackHR(HRESULT hr);
	HRESULT logErrorImport(_ConnectionPtr pConn);
	void setLastError(const std::string& strError);
//---------------------------------------------------------------------
private: // FUNCTIONS
//-------------------------------------------------------------------
	void TRACE(const char* format, ...);
//---------------------------------------------------------------------
private: // DATA
//-------------------------------------------------------------------
	std::string _strLastError;
	HRESULT _hrLastError;
};
#endif