#ifndef COMMAND_API_H
#define COMMAND_API_H
/*------------------------------------------------------------------------------
                           GENERAL INCLUDES
------------------------------------------------------------------------------*/
#include "Common.h"
#include "Error.h"
#include "Parameter.h"
#include "Connection.h"
/*---------------------------------------------------------------------
                                TYPES
---------------------------------------------------------------------*/
/*---------------------------------------------------------------------
                                CLASSES
---------------------------------------------------------------------*/
class GCommand : public GError
{
//---------------------------------------------------------------------
public: // FUNCTIONS
//--------------------------------------------------------------------
	GCommand();
	~GCommand();
	std::string getCommand() const;
	int getCountRecordsAffected() const;
	void setOption2Procedure();
	void setOption2CmdText();
	void setExecuteNoRecords();
	bool setCommandTimeout(long lTimeout);
	void clearExecuteNoRecords();
	bool setDBConnection(GConnection* pDB);
	bool setCommand(const std::string& strCommand);
	_RecordsetPtr getReturnedRecordset() const;
	void setRecordset(_RecordsetPtr rsp);
	bool addParameter(GParameter* pParam);
	bool reset();
	bool cancel();
	bool execute(const std::string& strCommandStr, const std::string& strFilter = "");
//---------------------------------------------------------------------
private: // FUNCTIONS
//--------------------------------------------------------------------
	void _init();
//---------------------------------------------------------------------
private: // DATA
//-------------------------------------------------------------------
	_CommandPtr _pCommand;
	_RecordsetPtr _pRecordset;
	std::string _strCommand;
	int _iRecordsAffected;
	long _lOptions;
};
#endif