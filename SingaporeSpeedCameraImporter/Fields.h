#ifndef FIELD_API_H
#define FIELD_API_H
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
class DATABASE_API GField
{
//---------------------------------------------------------------------
public: // FUNCTIONS
//---------------------------------------------------------------------
	GField();
	GField(const std::string& strName,
		   const std::string& strType,
		   const std::string& strLen = "",
		   const std::string& strNull = "");
	GField(const GField& f);
	GField& operator=(const GField& f);
	std::string getName() const;
	std::string getType() const;
	std::string getLen() const;
	std::string getNull() const;
	void set(const std::string& strName,
		     const std::string& strType,
			 const std::string& strLen = "",
			 const std::string& strNull = "");
	void setName(const std::string& strName);
	void setType(const std::string& strType);
	void setLen(const std::string& strLen);
	void setNull(const std::string& strNull);
//---------------------------------------------------------------------
private: // DATA
//---------------------------------------------------------------------
	std::string _strName;
	std::string _strType;
	std::string _strLen;
	std::string _strNull;
};
class DATABASE_API GFields
{
//---------------------------------------------------------------------
public: // FUNCTIONS
//---------------------------------------------------------------------
	GFields();
	~GFields();
	GFields(const GFields& fs);
	void add(const GField& f);
	GField get(int i) const;
	int getSize() const;
	void clear();
//---------------------------------------------------------------------
private: // DATA
//---------------------------------------------------------------------
	std::vector<GField> _vctFields;
};
#endif