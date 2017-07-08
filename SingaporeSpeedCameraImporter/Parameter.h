#ifndef PARAMETER_API_H
#define PARAMETER_API_H
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
class GParameter: public GError
{
//---------------------------------------------------------------------
public: // FUNCTIONS
//--------------------------------------------------------------------
	GParameter();
	GParameter(const std::string& strName, 
		      ParameterDirectionEnum enumDirection,
			  int iValue);
	GParameter(const std::string& strName,
		      ParameterDirectionEnum enumDirection,
			  std::string strValue);
	GParameter(const std::string& strName,
			  ParameterDirectionEnum enumDirection,
			  FILETIME ftValue);
	~GParameter();
	_ParameterPtr getParameter() const;
	bool setNumericScale(unsigned char unNumericScale);
	bool setPrecision(unsigned char ucPrecision);
	bool setSize(int iSize);
	bool setName(const std::string& strName);
	bool setDirToInput();
	bool setDirToOutput();
	bool setDirInputOutput();
	bool setDirToReturnValue();
	bool setDirToUnknown();
	bool setBoolean(bool bValue);
	bool setValue(int iValue);
	bool setValue(BSTR& bstrValue);
	bool setValue(char* cValue, int iSize);
	bool setValue(const std::string& strValue);
	bool setValue(FILETIME ftValue);
//---------------------------------------------------------------------
private: // FUNCTIONS
//--------------------------------------------------------------------
	void _init();
	bool _setValue(VARIANT& vtValue);
	bool _setType(DataTypeEnum enumType);
	bool _setDirection(ParameterDirectionEnum enumDirection);
	bool _setTypeToArray();
	bool _setTypeToBigInt();
	bool _setTypeToBinary();
	bool _setTypeToBoolean();
	bool _setTypeToBSTR();
	bool _setTypeToChapter();
	bool _setTypeToChar();
	bool _setTypeToCurrency();
	bool _setTypeToDate();
	bool _setTypeToDBDate();
	bool _setTypeToDBTime();
	bool _setTypeToDBTimeStamp();
	bool _setTypeToDecimal();
	bool _setTypeToDouble();
	bool _setTypeToEmpty();
	bool _setTypeToError();
	bool _setTypeToFileTime();
	bool _setTypeToGUID();
	bool _setTypeToIDispatch();
	bool _setTypeToInteger();
	bool _setTypeToIUnknown();
	bool _setTypeToLongVarBinary();
	bool _setTypeToLongVarChar();
	bool _setTypeToLongVarWChar();
	bool _setTypeToNumeric();
	bool _setTypeToPropVariant();
	bool _setTypeToSingle();
	bool _setTypeToSmallInt();
	bool _setTypeToTinyInt();
	bool _setTypeToUnsignedBigInt();
	bool _setTypeToUnsignedInt();
	bool _setTypeToUnsignedTinyInt();
	bool _setTypeToUserDefined();
	bool _setTypeToVarBinary();
	bool _setTypeToVarChar();
	bool _setTypeToVariant();
	bool _setTypeToVarNumeric();
	bool _setTypeToVarWChar();
	bool _setTypeToWChar();
//---------------------------------------------------------------------
private: // DATA
//-------------------------------------------------------------------
	_ParameterPtr _pParameter;
	VARIANT _vtValue;
};
#endif