#ifndef DB_SQL_H
#define DB_SQL_H
/*------------------------------------------------------------------------------
                           GENERAL INCLUDES
------------------------------------------------------------------------------*/
#include "Manager.h"
/*---------------------------------------------------------------------
                                TYPES
---------------------------------------------------------------------*/
/*---------------------------------------------------------------------
                                CLASSES
---------------------------------------------------------------------*/
class Sql
{
//---------------------------------------------------------------------
public: // FUNCTIONS
//--------------------------------------------------------------------
	enumOperations getSqlType(const std::string& strSql);
//---------------------------------------------------------------------
private: // FUNCTIONS
//--------------------------------------------------------------------
};
#endif