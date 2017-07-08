#ifndef DATABASE_API_H
#define DATABASE_API_H
/*------------------------------------------------------------------------------
                           GENERAL INCLUDES
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Fields.h"
/*---------------------------------------------------------------------
                                TYPES
---------------------------------------------------------------------*/
/*---------------------------------------------------------------------
                                CLASSES
---------------------------------------------------------------------*/
class DATABASE_API GDatabase
{
//---------------------------------------------------------------------
public: // FUNCTIONS
//---------------------------------------------------------------------
	GDatabase();
	GDatabase(const std::string& strConnectionStr);
	GDatabase(const std::string& strServerName, const std::string& strDbName);
	~GDatabase();
	std::string getServer() const;
	std::string getDatabase() const;
	std::string getConnectionStr() const;
	int getRecordsCount() const;
	int getRecordsCountAffected() const;
	std::string getLastError() const;
	int getLastHRError() const;
	GRecordSet* getRecordset() const;
	void setServer(const std::string& strServer);
	void setDatabase(const std::string& strDb);
	void setConnectionStr(const std::string& strConnectionStr);
	void setTimeout(long lTimeout);
	bool conn();
	void close();
	bool isOpen() const;
	bool execute(const std::string& strSql);
	bool exec(const std::string& strSql, enumOperations opera = Operation_Update);
	bool isRecordsExists() const;
	bool isDbExists(const std::string& strDbName);
	bool isTableExists(const std::string& strTable);
	bool deleteDB(const std::string& strDbName);
	bool deleteTable(const std::string& strTableName);
	bool createDB(const std::string& strDbName, bool bOverwrite);
	bool createDB(const std::string& strDbName, const std::string& strDbDir, bool bOverwrite = false);
	bool createTable(const std::string& strTableName, const GFields& fields, bool bOverwrite = false);
	bool shrinkDB(const std::string& strDBName);
	bool backupDB(const std::string& strBackupName);
	bool restoreDB(const std::string& strDb, const std::string& strBackupName);
	bool alterDBSingle(const std::string& strDBName);
	bool alterDBMulti(const std::string& strDBName);
	bool killDBConn(const std::string& strDBName);
//---------------------------------------------------------------------
private: // FUNCTION 
//---------------------------------------------------------------------
	void _setConnectionStrByServerDb();
	void _setServerDbByConnectionStr();
//---------------------------------------------------------------------
private: // DATA
//---------------------------------------------------------------------
	GManager* _pGManager;
	GConnection* _pGConnection;
	std::string _strServer;
	std::string _strDb;
	std::string _strConnectionStr;
	bool _bIsQuery;
	long _lTimeout;
};
#endif 