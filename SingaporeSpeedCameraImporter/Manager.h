#ifndef DB_MANAGER_H
#define DB_MANAGER_H

/*------------------------------------------------------------------------------
                           GENERAL INCLUDES
------------------------------------------------------------------------------*/
#include "Common.h"
#include "Connection.h"
#include "Command.h"
#include "Parameter.h"
#include "RecordSet.h"
/*---------------------------------------------------------------------
                                TYPES
---------------------------------------------------------------------*/
enum enumQueryCommands
{
	SQL_USERQUERY = 0,
	SQL_DELETE,
	SQL_INSERT,
	SQL_SELECT,
	SQL_UPDATE
};
enum enumQueryTypes
{
	QueryType_Table = 0,
	QueryType_Procedure,
	QueryType_SQLQuery
};
enum enumOperations
{
	Operation_Delete = 0,
	Operation_Insert,
	Operation_SelectInto,
	Operation_Select,
	Operation_Update,
	Operation_Create,
	Operation_Alter,
	Operation_Drop,
	Operation_Truncate
};
enum enumParameters
{
	Parameter_int = 0,
	Parameter_string,
	Parameter_datetime
};
#define PARAMETER_INT_SIZE 4
#define PARAMETER_DATETIME_SIZE 8
/*---------------------------------------------------------------------
                                CLASSES
---------------------------------------------------------------------*/
class GManager
{
//---------------------------------------------------------------------
public: // FUNCTIONS
//--------------------------------------------------------------------
	GManager();
	GManager(GConnection* pDB);
	~GManager();
	GRecordSet* getRecordSet() const;
	int getLastHRError() const;
	std::string getLastError() const;
	std::string getTable() const;
	std::string getQuery() const;
	int getReturnParameter() const;
	int getRecordCount() const;
	long getCountRecordsAffected() const;
	bool getNextRecord();
	void setTable(const std::string& strTable);
	void setConnection(GConnection* pdbc);
	void setCommandTimeout(long lTimeout);
	void setCommand(const std::string& strCmd);
	bool adoUserProc(const std::string& strProc, enumOperations opera, const std::string& strFilter = "");
	bool adoUserSql(const std::string& strQuery, enumOperations opera, const std::string& strFilter = "");
	bool adoProcOperation(enumOperations opera, const std::string& strFilter = "");
//---------------------------------------------------------------------
protected: // FUNCTIONS
//--------------------------------------------------------------------
	bool getValue(const std::string& strColumnName, int& iValue);
	bool getValue(const std::string& strColumnName, std::string& strValue);
	bool getValue(const std::string& strColumnName, FILETIME& ftValue);
	void addInputParameter(const std::string& strParameterName, int iValue);
	void addInputParameter(const std::string& strParameterName, const std::string& strValue);
	void addInputParameter(const std::string& strParameterName, FILETIME ftValue);
//---------------------------------------------------------------------
private: // FUNCTIONS
//--------------------------------------------------------------------
	void _init();
	int _getQueryType() const;
	int _getOperation() const;
	std::string _getProcedureName() const;
	std::string _getCmdText() const;
	void _getRecordValues();
	void _setQueryType(int iQueryType);
	void _setOperation(int iOperation);
	bool _runTableOperation(enumOperations opera, const std::string& strFilter = "");
	bool _runSqlOperation(enumOperations opera, const std::string& strFilter = "");
	bool _runOperation(enumOperations opera, enumQueryTypes query, const std::string& strFilter = "");
//---------------------------------------------------------------------
private: // DATA
//--------------------------------------------------------------------
	GConnection* _pConnection;
	GCommand* _pCommand;
	GRecordSet* _pRecordSet;
	int _iQueryType;
	int _iOperation;
	int _iLastError;
	int _iReturnParameter;
	std::string _strLastError;
	std::string _strQuery;
	std::string _strTable;
};
#endif