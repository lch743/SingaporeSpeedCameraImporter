#ifndef RECORDSET_API_H
#define RECORDSET_API_H
/*------------------------------------------------------------------------------
                           GENERAL INCLUDES
------------------------------------------------------------------------------*/
#include "Common.h"
#include "Error.h"
#include "Command.h"
/*---------------------------------------------------------------------
                                TYPES
---------------------------------------------------------------------*/
/*---------------------------------------------------------------------
                                CLASSES
---------------------------------------------------------------------*/
class DATABASE_API GRecordSet : public GError
{
//---------------------------------------------------------------------
public: // FUNCTIONS
//--------------------------------------------------------------------
	GRecordSet();
	GRecordSet(_RecordsetPtr& pRecordSet);
	~GRecordSet();
	GRecordSet& operator=(const GRecordSet& f);
	_RecordsetPtr getRecordsetPtr() const;
	void setRecordsetPtr(const _RecordsetPtr& rsp);
	bool setCursorToDynamic();
	bool setCursorToKeyset();
	bool setCursorToStatic();
	bool setCursorToForwardOnly();
	bool setCursorToUnspecified();
	bool moveFirst();
	bool moveLast();
	bool moveNext();
	bool movePrev();
	bool moveTo(int iRecordsToMove);
	bool isBOF();
	bool isEOF();
	bool isNoRecords();
	bool setFilter(const std::string& strStr);
	long getRecordCount();
	bool close();
	bool getValue(LPCSTR lpName, int& iValue);
	bool getValue(LPCSTR strName, std::string& strValue);
//---------------------------------------------------------------------
private: // FUNCTIONS
//--------------------------------------------------------------------
	bool _setCursorMode(CursorTypeEnum enumCursorType);
//---------------------------------------------------------------------
private: // DATA
//-------------------------------------------------------------------
	_RecordsetPtr _pRecordSet;
};
#endif