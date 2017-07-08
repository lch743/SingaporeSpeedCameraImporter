#ifndef CONNECTION_API_H
#define CONNECTION_API_H
/*------------------------------------------------------------------------------
                           GENERAL INCLUDES
------------------------------------------------------------------------------*/
#include "Common.h"
#include "Error.h"
/*---------------------------------------------------------------------
                                TYPES
---------------------------------------------------------------------*/
/*---------------------------------------------------------------------
                                CLASSES
---------------------------------------------------------------------*/
class GConnection : public GError
{
//---------------------------------------------------------------------
public: // FUNCTIONS
//--------------------------------------------------------------------
	GConnection();
	GConnection(const std::string& strConnectionStr);
	~GConnection();
	_ConnectionPtr getConnection() const;
	long getCountRecordsAffected() const;
	void setUser(const std::string& strUser);
	void setPassword(const std::string& strPassword);
	bool setConnectionTimeout(long lTimeout);
	bool setConnectionStr(const std::string& strConnectionStr);
	bool abortRetaining(bool bMode);
	bool commitRetaining(bool bMode);
	bool beginTransaction();
	bool commit();
	bool rollback();
	bool isClosed();
	bool isOpen();
	bool open(const std::string& strConnectionStr,
		      const std::string& strUser = "",
			  const std::string& strPassword = "");
	bool open();
	bool close();
	bool cancel();
	_RecordsetPtr exec(const std::string& strCommandStr);
//---------------------------------------------------------------------
private: // FUNCTIONS
//-------------------------------------------------------------------
	void _init();
	bool setOrRemoveAttribute(bool bMode, long lAttribute);
//---------------------------------------------------------------------
private: // DATA
//-------------------------------------------------------------------
	_ConnectionPtr _pConnection;
	bool _bOpen;
	long _lNestingLevel;
	long _lRecordAffected;
	std::string _strConnectionStr;
	std::string _strUser;
	std::string _strPassword;
};
#endif