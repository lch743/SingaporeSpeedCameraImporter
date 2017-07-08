#include "Importer.h"
#include "Util.h"
#include "GDeliver.h"
#include "LineInfo.h"
#include "AlertPoint.h"
#include "StrComparer.h"
#include <fstream>

Importer::~Importer()
{
	if(NULL != __pSpatialIndex)
	{
		delete __pSpatialIndex;
		__pSpatialIndex = NULL;
	}
}
bool Importer::go()
{
	if(!__init())
	{
		return false;
	}
	if(!__read())
	{
		return false;
	}
	if(!__preprocess())
	{
		return false;
	}
	if(!__writeRawData())
	{
		return false;
	}
	if(__strOldDatabase.size() > 0)
	{
		if(!__createRawdataDiff())
		{
			return false;
		}
		if(!__copyAlertPointsFromOld())
		{
			return false;
		}
		if(!__updateRecordsWithDiff())
		{
			return false;
		}
	}
	if(!__addAngleInfo())
	{
		return false;
	}
	if(!__process())
	{
		return false;
	}
	if(!__createGpx())	
	{
		return false;
	}
	//if(!__show())
	//{
	//	return false;
	//}
	LOG(INFO) << "Done.";
	return true;
}
bool Importer::__init()
{
	if(!__checkArguments())	
	{
		return false;
	}
	LOG(INFO) << "Arguments: ";
	LOG(INFO) << "Server: " + __strServer;
	LOG(INFO) << "Database: " + __strDatabase;
	if(__strOldDatabase.length() > 0)
	{
		LOG(INFO) << "Old Database: " + __strOldDatabase;
	}
	LOG(INFO) << "RoadDatabase: " + __strRoadDatabase;
	LOG(INFO) << "Input File: " + __strFile;
	if(!ChkFileExist(__strFile))
	{
		LOG(ERROR) << "Input File Not Exist!";
		return false;
	}
	if(!ChkFileExist(exe_angle_parser))
	{
		LOG(ERROR) << "Angle Parser Not Exist!";
		return false;
	}
	if(!ChkFileExist(exe_gpxbuilder))
	{
		LOG(ERROR) << "Angle Parser Not Exist!";
		return false;
	}
	return true;
}
bool Importer::__checkArguments()
{
	bool bRtn = true;
	if(0 == __strServer.size())
	{
		LOG(ERROR) << "Server Name Null";
		bRtn &= false;
	}
	if(0 == __strDatabase.size())
	{
		LOG(ERROR) << "Database Name Null";
		bRtn &= false;
	}
	if(0 == __strRoadDatabase.size())
	{
		LOG(ERROR) << "Road Database Name Null";
		bRtn &= false;
	}
	if(0 == __strFile.size())
	{
		LOG(ERROR) << "File Name Null";
		bRtn &= false;
	}
	return bRtn;
}
bool Importer::__getSpliter(const std::string& strLine, std::string& strSpliter)
{
	int iLength = static_cast<int>(vctSpliters.size());
	for(int i = 0; i < iLength; ++i)
	{
		if(-1 < StrFindNoCase(strLine, vctSpliters[i]))
		{
			strSpliter = vctSpliters[i];
			break;
		}
	}
	return true;
}
bool Importer::__getLineInfoById(int iLineId, LineInfo& li)
{
	li.LineId = iLineId;
	_RecordsetPtr _rst = NULL;
	_variant_t curValue;
	try
	{
		int _iLineId = 0;
		int _iMaxLat = 0;
		int _iMaxLon = 0;
		int _iMinLat = 0;
		int _iMinLon = 0;
		int _iOneWay = 0;
		int _iLineCode = 0;
		std::string _strName = "";
		__gdbRoad.execute(sql_fetch_line_info + " where Line_Id = " + Num2Str(iLineId));
		GRecordSet* pRst;
		pRst = __gdbRoad.getRecordset();
		_rst = pRst->getRecordsetPtr();
		if(_rst == NULL)
		{
			return false;
		}
		if(_rst->GetRecordCount() != 1)
		{
			return false;
		}
		_rst->MoveFirst();
		curValue = _rst->GetFields()->GetItem("Max_Lat")->GetValue();
		if(curValue.vt != VT_NULL && curValue.vt != VT_EMPTY)
		{
			li.MaxLat = curValue.intVal;
		}
		curValue = _rst->GetFields()->GetItem("Max_Lon")->GetValue();
		if(curValue.vt != VT_NULL && curValue.vt != VT_EMPTY)
		{
			li.MaxLon = curValue.intVal;
		}
		curValue = _rst->GetFields()->GetItem("Min_Lat")->GetValue();
		if(curValue.vt != VT_NULL && curValue.vt != VT_EMPTY)
		{
			li.MinLat = curValue.intVal;
		}
		curValue = _rst->GetFields()->GetItem("Min_Lon")->GetValue();
		if(curValue.vt != VT_NULL && curValue.vt != VT_EMPTY)
		{
			li.MinLon = curValue.intVal;
		}
		curValue = _rst->GetFields()->GetItem("One_Way")->GetValue();
		if(curValue.vt != VT_NULL && curValue.vt != VT_EMPTY)
		{
			li.OneWay = curValue.intVal;
		}
		curValue = _rst->GetFields()->GetItem("Line_Code")->GetValue();
		if(curValue.vt != VT_NULL && curValue.vt != VT_EMPTY)
		{
			li.LineCode = curValue.iVal;
		}
		curValue = _rst->GetFields()->GetItem("Name")->GetValue();
		if(curValue.vt != VT_NULL && curValue.vt != VT_EMPTY)
		{
			li.Name = (char*)(_bstr_t)curValue;
		}
	}
	catch(_com_error* e)
	{
		LOG(ERROR) << WString2String((wchar_t*)(e->Description()));
		return false;
	}
	return true;
}
bool Importer::__addLinePtsInfo(LineInfo& li)
{
	_RecordsetPtr _rst = NULL;
	_variant_t curValue;
	try
	{
		Coordinate pnt;
		IntCoordinate pntInt;
		__gdbRoad.execute(sql_fetch_linepts_info + " where Line_Id = " + Num2Str(li.LineId));
		GRecordSet* pRst;
		pRst = __gdbRoad.getRecordset();
		_rst = pRst->getRecordsetPtr();
		if(_rst == NULL)
		{
			return false;
		}
		if(_rst->GetRecordCount() == 0)
		{
			return false;
		}
		_rst->MoveFirst();
		int iCount = 0;
		while(!_rst->EndOfFile)
		{
			pnt.Lat = 0.0;
			pnt.Lon = 0.0;
			curValue = _rst->GetFields()->GetItem("Lat")->GetValue();
			if(curValue.vt != VT_NULL && curValue.vt != VT_EMPTY)
			{
				pntInt.Lat = curValue.intVal;
				pnt.Lat = static_cast<double>(curValue.intVal / 1000000.0);
			}	
			curValue = _rst->GetFields()->GetItem("Lon")->GetValue();
			if(curValue.vt != VT_NULL && curValue.vt != VT_EMPTY)
			{
				pntInt.Lon = curValue.intVal;
				pnt.Lon = static_cast<double>(curValue.intVal / 1000000.0);
			}
			li.vctPtsInt.push_back(pntInt);
			li.vctPts.push_back(pnt);
			++iCount;
			_rst->MoveNext();
		}
		li.pntCount = iCount;
	}
	catch(_com_error* e)
	{
		LOG(ERROR) << WString2String((wchar_t*)(e->Description()));
		return false;
	}
	return true;
}
double Importer::__getNeastLine(double dLat, double dLon, std::vector<LineInfo>& vctResults, LineInfo& li, int& iSegPntStart, int& iSegPntEnd, int iCompareAngle)
{
	Coordinate pnt;
	pnt.Lat = dLat;
	pnt.Lon = dLon;
	double dDistanceCur = 9999.0;
	double dDistance = 0.0;
	int _iSegPntStart = 0;
	int _iSegPntEnd = 0;
	bool bCompareAngle;
	int _iAngleTmpLeft = 0;
	int _iAngleTmpRight = 0;
	for(int i = 0; i < static_cast<int>(vctResults.size()); ++i)
	{
		bCompareAngle = false;
		__addLinePtsInfo(vctResults[i]);
		if(0 == static_cast<int>(vctResults.size()))
		{
			LOG(WARNING) << Num2Str(vctResults[i].LineId) + ": No line pnts info";
			continue;
		}
		_iSegPntStart = 0;
		_iSegPntEnd = 0;
		dDistance = __distance2Line(pnt, vctResults[i], _iSegPntStart, _iSegPntEnd);	
		Coordinate pntStart = vctResults[i].vctPts[_iSegPntStart];
		Coordinate pntEnd = vctResults[i].vctPts[_iSegPntEnd];
		if(iCompareAngle == -9999)
		{
			bCompareAngle = true;
		}
		else
		{
			_iAngleTmpLeft = __diffAngle(iCompareAngle, static_cast<int>(__calcAngleBy2Pnt(pntStart, pntEnd)));
			_iAngleTmpRight = __diffAngle(iCompareAngle, static_cast<int>(__calcAngleBy2Pnt(pntEnd, pntStart)));
			if(li.OneWay == 1)
			{
				if(_iAngleTmpLeft < _iAngleTmpRight)	
				{
					bCompareAngle = true;
				}
			}
			else if(li.OneWay == 2)
			{
				if(_iAngleTmpRight < _iAngleTmpLeft)	
				{
					bCompareAngle = true;
				}
			}
			else
			{
				if(_iAngleTmpLeft <= _iAngleTmpRight)	
				{
					bCompareAngle = true;
				}
				else if(_iAngleTmpLeft > _iAngleTmpRight)
				{
					int iTmp = _iSegPntStart;
					_iSegPntStart = _iSegPntEnd;
					_iSegPntEnd = iTmp;
					bCompareAngle = true;
				}
			}
		}
		if(bCompareAngle && dDistance < dDistanceCur && !CheckItemInVector(vctResults[i].LineId, __vctProcessedIds))
		{
			dDistanceCur = dDistance;
			li = vctResults[i]; 
			iSegPntStart = _iSegPntStart;	
			iSegPntEnd = _iSegPntEnd;
		}
	}
	if(li.LineId > 0)
	{
		__vctProcessedIds.push_back(li.LineId);
	}
	return dDistanceCur;
}
double Importer::__distance2Line(const Coordinate& pnt, LineInfo& li, int& iSegPntStart, int& iSegPntEnd)
{
	double dDistanceCur = 9999.0;
	double dDistance = 0.0;
	for(int i = 0; i < static_cast<int>(li.vctPts.size()) - 1; ++i)
	{
		dDistance = __distanct2Segment(pnt, li.vctPts[i], li.vctPts[i + 1]); 
		if(dDistance < dDistanceCur)
		{
			dDistanceCur = dDistance;
			iSegPntStart = i;
			iSegPntEnd = i + 1;
		}
	}
	return dDistanceCur;
}
double Importer::__distanct2Segment(const Coordinate& pnt, const Coordinate& pntStart, const Coordinate& pntEnd)
{
	if( !(fabs((double)(pntStart.Lon - pntEnd.Lon)) < 1e-6) ||
		!(fabs((double)(pntStart.Lat - pntEnd.Lat)) < 1e-6) )
	{
		double dDaltaLon = pntEnd.Lon - pntStart.Lon;
		double dDaltaLat = pntEnd.Lat - pntStart.Lat;

		double dDist = dDaltaLat * (pnt.Lon - pntStart.Lon) + dDaltaLon * ( pntStart.Lat - pnt.Lat);

		return sqrt((double)(dDist * dDist / (dDaltaLon * dDaltaLon + dDaltaLat * dDaltaLat)));
	}
	else 
	{
		return sqrt((double)((pntStart.Lon - pnt.Lon) * (pntStart.Lon - pnt.Lon) + (pntStart.Lat - pnt.Lat) * (pntStart.Lat - pnt.Lat)));
	}
}
bool Importer::__isPrjOnSegment(const Coordinate& pnt, const Coordinate& pntStart, const Coordinate& pntEnd)
{
	if ((((pntStart.Lon - pntEnd.Lon) < 1e-13) && ((pntStart.Lon - pntEnd.Lon) > -1e-13)) &&
		(((pntStart.Lat - pntEnd.Lat) < 1e-13) && ((pntStart.Lat - pntEnd.Lat) > -1e-13)) )
	{
		return true;
	}
	double dX = pnt.Lon - pntStart.Lon;
	double dY = pnt.Lat - pntStart.Lat;
	double a2 = dX*dX + dY*dY;
	dX = pnt.Lon - pntEnd.Lon;
	dY = pnt.Lat - pntEnd.Lat;
	double b2 = dX*dX + dY*dY;
	dX = pntEnd.Lon - pntStart.Lon;
	dY = pntEnd.Lat - pntStart.Lat;
	double c2 = dX*dX + dY*dY;
	if( (b2+c2<a2) || (a2+c2<b2) )
	{
		return false;
	}
	return true;
}
bool Importer::__searchAlongRoadByLocationName(int iLineId, int iLat, int iLon, const std::string& strSearch, LineInfo& liOut)
{
	std::list<int> _lstLineIds;
	_lstLineIds.clear();
	bool bFound = false;
	LineInfo li;
	StrComparer sc;
	__fetchLineIdsByPnt(iLineId, iLat, iLon, _lstLineIds);
	std::list<int>::iterator it;
	it = _lstLineIds.begin();
	int iCount = 0;
	while(!bFound)
	{
		if(iCount >= 2000)
		{
			return false;
		}
		li.Reset();
		__getLineInfoById(*it, li);
		__addLinePtsInfo(li);
		sc.setLeft(li.Name);
		sc.setRight(strSearch);
		if(sc.compare())
		{
			bFound = true;
			break;
		}
		__fetchLineIdsByPnt(li.LineId, li.vctPtsInt[0].Lat, li.vctPtsInt[0].Lon, _lstLineIds);
		__fetchLineIdsByPnt(li.LineId, li.vctPtsInt[li.pntCount - 1].Lat, li.vctPtsInt[li.pntCount - 1].Lon, _lstLineIds);
		it = _lstLineIds.erase(it);
		iCount++;
	}
	liOut = li;
	return true;
}
bool Importer::__searchCircleByLocationName(double dLat, double dLon, const std::string& strSearch, LineInfo& liOut, int& iSegPntStart, int& iSegPntEnd, int iCompareAngle)
{
	StrComparer sc;
	LineInfo li;
	std::vector<int> vctIds;
	std::vector<int> vctCheckedIds;
	std::vector<LineInfo> vctResults;
	vctIds.clear();
	vctCheckedIds.clear();
	vctResults.clear();
	li.Reset();
	liOut.Reset();
	for(int k = 1;;k += 10)
	{
		if(!__pSpatialIndex->QueryByPoint(static_cast<float>(dLat), static_cast<float>(dLon), k * 20, vctIds))
		{
			continue;
		}
		for(int i = 0; i < static_cast<int>(vctIds.size()); ++i)
		{
			if(CheckItemInVector(vctIds[i], vctCheckedIds))
			{
				continue;
			}
			vctCheckedIds.push_back(vctIds[i]);
			li.Reset();
			__getLineInfoById(vctIds[i], li);
			sc.setLeft(li.Name);
			sc.setRight(strSearch);
			if(sc.compare())
			{
				vctResults.push_back(li);
			}
		}
		if(k > 50)
		{
			break;
		}
	}
	li.Reset();
	__getNeastLine(dLat, dLon, vctResults, li, iSegPntStart, iSegPntEnd, iCompareAngle);
	liOut = li;	
	if(liOut.LineId == 0)
	{
		return false;
	}
	return true;
}
bool Importer::__fetchLineIdsByPnt(int iLineId, int iPntLat, int iPntLon, std::list<int>& lstLineIds)
{
	_RecordsetPtr _rst = NULL;
	_variant_t curValue;
	try
	{
		int _iLineId = 0;
		int _iMaxLat = 0;
		int _iMaxLon = 0;
		int _iMinLat = 0;
		int _iMinLon = 0;
		int _iOneWay = 0;
		int _iLineCode = 0;
		std::string _strName = "";
		__gdbRoad.execute(sql_fetch_lineids_by_latlon + " and lp.Lat = " + Num2Str(iPntLat) + " and lp.Lon = " + Num2Str(iPntLon));
		GRecordSet* pRst;
		pRst = __gdbRoad.getRecordset();
		_rst = pRst->getRecordsetPtr();
		if(_rst == NULL)
		{
			return false;
		}
		_rst->MoveFirst();
		curValue = _rst->GetFields()->GetItem("Line_Id")->GetValue();
		while(!_rst->EndOfFile)
		{
			curValue = _rst->GetFields()->GetItem("Line_Id")->GetValue();
			if(curValue.vt != VT_NULL && curValue.vt != VT_EMPTY)
			{
				if(!(std::find(lstLineIds.begin(), lstLineIds.end(), curValue.intVal) != lstLineIds.end()))
				{
					lstLineIds.push_back(curValue.intVal);
				}
			}
			_rst->MoveNext();
		}
	}
	catch(_com_error* e)
	{
		LOG(ERROR) << WString2String((wchar_t*)(e->Description()));
		return false;
	}
	return true;
}
bool Importer::__projectPointOnRoad(double dLat, double dLon, AlertPoint& ap, LineInfo& li, int iSegPntStart, int iSegPntEnd)
{
	Coordinate pntStart = li.vctPts[iSegPntStart];
	Coordinate pntEnd = li.vctPts[iSegPntEnd];
	double dDaltaLon = pntEnd.Lon-pntStart.Lon;
	double dDaltaLat = pntStart.Lat-pntEnd.Lat;
	double dDaltaLon2 = dDaltaLon * dDaltaLon;
	double dDaltaLat2 = dDaltaLat * dDaltaLat;
	double dDeltaLonLat = dDaltaLon * dDaltaLat;
	double dLineSectDist = dDaltaLon * dDaltaLon + dDaltaLat * dDaltaLat;
	ap.Lon = static_cast<int>((dDeltaLonLat * (pntStart.Lat - dLat) +
		pntStart.Lon * dDaltaLat2 + dLon * dDaltaLon2) / dLineSectDist * 1000000);
	ap.Lat = static_cast<int>((dDeltaLonLat * ( pntStart.Lon - dLon ) +
		pntStart.Lat * dDaltaLon2 + dLat * dDaltaLat2) / dLineSectDist * 1000000);
	return true;
}
int Importer::__calcAngleFromToLineInfo(LineInfo& li_from, LineInfo& li_to)
{
	Coordinate pntStartFrom = li_from.vctPts[0];
	Coordinate pntEndFrom = li_from.vctPts[li_from.pntCount - 1];
	Coordinate pntStartTo = li_from.vctPts[0];
	Coordinate pntEndTo = li_from.vctPts[li_to.pntCount - 1];
	Coordinate pntMidFrom;
	Coordinate pntMidTo;
	pntMidFrom.Lat = (pntStartFrom.Lat + pntEndFrom.Lat) / 2;
	pntMidFrom.Lon = (pntStartFrom.Lon + pntEndFrom.Lon) / 2;
	pntMidTo.Lat = (pntStartTo.Lat + pntEndTo.Lat) / 2; 
	pntMidTo.Lon = (pntStartTo.Lon + pntEndTo.Lon) / 2; 
	return static_cast<int>(__calcAngleBy2Pnt(pntMidFrom, pntMidTo));
}
bool Importer::__calcAngleConincide(AlertPoint& ap, LineInfo& li, int iSegPntStart, int iSegPntEnd)
{
	Coordinate pntStart = li.vctPts[iSegPntStart];
	Coordinate pntEnd = li.vctPts[iSegPntEnd];
	double dAngle = 0.0;
	if(li.OneWay == 1)
	{
		ap.Alert_Angle = static_cast<int>(__calcAngleBy2Pnt(pntStart, pntEnd));
		ap.Conincide = 0;
	}
	if(li.OneWay == 2)
	{
		ap.Alert_Angle = static_cast<int>(__calcAngleBy2Pnt(pntEnd, pntStart));
		ap.Conincide = 0;
	}
	else
	{
		ap.Alert_Angle = static_cast<int>(__calcAngleBy2Pnt(pntStart, pntEnd));
		ap.Conincide = 0;
	}
	return true;
}
double Importer::__calcAngleBy2Pnt(const Coordinate &Coord_Angle_F, const Coordinate &Coord_Angle_T)
{
	double dAngle = atan2((double)(Coord_Angle_T.Lat - Coord_Angle_F.Lat), (double)(Coord_Angle_T.Lon - Coord_Angle_F.Lon));
	if (dAngle < 0) 
	{
		dAngle += 2 * math_pi;
	}
	dAngle *= RTOD;
	if (dAngle >= 0 && dAngle < 90)
	{
		dAngle = 90 - dAngle;
	}
	else if (dAngle >= 90 && dAngle < 180)
	{
		dAngle = 360 - (dAngle - 90);
	}
	else if (dAngle >= 180 && dAngle < 270)
	{
		dAngle = 180 + (270 - dAngle);
	}
	else if (dAngle >= 270 && dAngle < 360)
	{
		dAngle = 90 + 360 - dAngle;
	}
	return dAngle;
}
int Importer::__diffAngle(int iAngleCompareF, int iAngleCompareT)
{
	int iTmp = iAngleCompareF - iAngleCompareT;
	if(iTmp > 180)
	{
		iTmp -= 360;
	}
	else if(iTmp < -180)
	{
		iTmp += 360;
	}
	return abs(iTmp);
}
bool Importer::__setAlertPoint(AlertPoint& ap, LineInfo& li, std::map<int, std::vector<std::string>>::iterator it)
{
	ap.AlertType = it->second[3];
	ap.Icon_File_Path = Arg_Icon_File_Path;
	ap.Point_Id = it->first;
	ap.Alert_Dist = Arg_Alert_Dist;
	ap.Name = it->second[3];
	ap.Comment = it->second[3];
	ap.GRM_Code = Arg_GRM_Code;
	ap.offset = Arg_Offset;
	ap.Line_Code = li.LineCode;
	ap.Line_flags = Arg_Line_Flags;
	ap.Line_Attribute = Arg_Line_Attribute;
	ap.Line_Id = li.LineId;
	return true;
}
bool Importer::__uploadToDB(AlertPoint& ap)
{
	std::string strSql = "insert into AlertPoint values( "\
		"N'" + ap.AlertType + "', "\
		"N'" + ap.Icon_File_Path + "', "\
		"" + Num2Str(ap.Point_Id) + ", "\
		"" + Num2Str(ap.Alert_Dist) + ", "\
		"" + Num2Str(ap.Alert_Angle) + ", "\
		"N'" + ap.Name + "', "\
		"N'" + ap.Comment + "', "\
		"" + Num2Str(ap.Lat) + ", "\
		"" + Num2Str(ap.Lon) + ", "\
		"" + Num2Str(ap.GRM_Code) + ", "\
		"" + Num2Str(ap.offset) + ", "\
		"" + Num2Str(ap.Line_flags) + ", "\
		"" + Num2Str(ap.Line_Code) + ", "\
		"" + Num2Str(ap.Line_Attribute) + ", "\
		"" + Num2Str(ap.Conincide) + ", "\
		"" + Num2Str(ap.Line_Id) + " )";
	try
	{
		if(!__gdb.execute(strSql))
		{
			LOG(ERROR) << __gdb.getLastError();
			return false;
		}
	}
	catch(_com_error* e)
	{
		LOG(ERROR) << WString2String((wchar_t*)(e->Description()));
		return false;
	}
	return true;
}
bool Importer::__updateRecordsWithDiff()
{
	if(!__gdb.isOpen())
	{
		if(!__gdb.conn())
		{
			LOG(ERROR) << "Databse Connect Error!";
			return false;
		}
	}
	_RecordsetPtr _rst = NULL;
	std::list<int> _lstIds;
	std::string strSql = "select id from " + table_rawdata_plus;
	try
	{
		if(!__gdb.execute(strSql))
		{
			LOG(ERROR) << __gdb.getLastError();	
			return false;
		}
		GRecordSet* pRst;
		pRst = __gdb.getRecordset();
		_rst = pRst->getRecordsetPtr();
		if(_rst == NULL)
		{
			return false;
		}
		_rst->MoveFirst();
		while(!_rst->EndOfFile)
		{
			_variant_t curValue;
			curValue = _rst->GetFields()->GetItem("id")->GetValue();
			if(curValue.vt != VT_NULL && curValue.vt != VT_EMPTY)
			{
				_lstIds.push_back(curValue.intVal);
			}
			_rst->MoveNext();
		}
	}
	catch(_com_error* e)
	{
		LOG(ERROR) << WString2String((wchar_t*)(e->Description()));
		return false;
	}
	std::map<int, std::vector<std::string>>::iterator it;
	for(it = __mpRecords.begin(); it != __mpRecords.end();)
	{
		if(std::find(_lstIds.begin(), _lstIds.end(), it->first) == _lstIds.end())
		{
			__mpRecords.erase(it++);
		}
		else
		{
			++it;
		}
	}
	return true;
}
bool Importer::__createSpatialIndex()
{
	LOG(INFO) << "Create Index for Line...";
	
	if(!__gdbRoad.isOpen())
	{
		if(!__gdbRoad.conn())
		{
			LOG(ERROR) << __gdbRoad.getLastError();
			return false;
		}
	}
	GDatabase _gdb;
	_gdb.setServer(__strServer);
	_gdb.setDatabase(__strRoadDatabase);
	if(!_gdb.conn())
	{
		LOG(ERROR) << _gdb.getLastError();
		return false;
	}
	if(NULL == __pSpatialIndex)
	{
		__pSpatialIndex = new SpatialIndex();
	}
	_RecordsetPtr _rst = NULL;
	_variant_t curValue;
	try
	{
		int _iLineId = 0;
		int _iMaxLat = 0;
		int _iMaxLon = 0;
		int _iMinLat = 0;
		int _iMinLon = 0;
		std::vector<GDeliverSpatialIndex> _vctIndexRecords;
		GDeliverSpatialIndex _gds;
		__gdbRoad.execute(sql_fetch_line_lat_lon);
		GRecordSet* pRst;
		pRst = __gdbRoad.getRecordset();
		_rst = pRst->getRecordsetPtr();
		if(_rst == NULL)
		{
			return false;
		}
		if(_rst->GetRecordCount() == 0)
		{
			return false;
		}
		_rst->MoveFirst();
		int _iCount = 0;
		while(!_rst->EndOfFile)
		{
			_iLineId = 0;
			_iMaxLat = 0;
			_iMaxLon = 0;
			_iMinLat = 0;
			_iMinLon = 0;
			_gds.reset();
			curValue = _rst->GetFields()->GetItem("Line_Id")->GetValue();
			if(curValue.vt != VT_NULL && curValue.vt != VT_EMPTY)
			{
				_iLineId = curValue.lVal;
			}
			curValue = _rst->GetFields()->GetItem("Max_Lat")->GetValue();
			if(curValue.vt != VT_NULL && curValue.vt != VT_EMPTY)
			{
				_iMaxLat = curValue.lVal;
			}
			curValue = _rst->GetFields()->GetItem("Max_Lon")->GetValue();
			if(curValue.vt != VT_NULL && curValue.vt != VT_EMPTY)
			{
				_iMaxLon = curValue.lVal;
			}
			curValue = _rst->GetFields()->GetItem("Min_Lat")->GetValue();
			if(curValue.vt != VT_NULL && curValue.vt != VT_EMPTY)
			{
				_iMinLat = curValue.lVal;
			}
			curValue = _rst->GetFields()->GetItem("Min_Lon")->GetValue();
			if(curValue.vt != VT_NULL && curValue.vt != VT_EMPTY)
			{
				_iMinLon = curValue.lVal;
			}
			_gds.setLineId(_iLineId);
			_gds.setGpMaxLat(static_cast<float>(_iMaxLat / 1000000.0));
			_gds.setGpMaxLon(static_cast<float>(_iMaxLon / 1000000.0));
			_gds.setGpMinLat(static_cast<float>(_iMinLat / 1000000.0));
			_gds.setGpMinLon(static_cast<float>(_iMinLon / 1000000.0));
			_vctIndexRecords.push_back(_gds);
			_rst->MoveNext();
			_iCount++;
			if(_iCount % createindex_split_num == 0)
			{
				if(!__pSpatialIndex->CreateIndex(&_vctIndexRecords[0], static_cast<int>(_vctIndexRecords.size())))
				{
					LOG(ERROR) << "Create Index Error!";
					return false;
				}
				_vctIndexRecords.clear();
				double dRate = 100 * static_cast<double>(_iCount) / static_cast<double>(_rst->GetRecordCount());
				LOG(INFO) << "Create Index " + Double2Str(dRate, 2) + "% ...";
			}
		}
		if(!__pSpatialIndex->CreateIndex(&_vctIndexRecords[0], static_cast<int>(_vctIndexRecords.size())))
		{
			LOG(ERROR) << "Create Index Error!";
			return false;
		}
		double dRate = 100 * static_cast<double>(_iCount) / static_cast<double>(_rst->GetRecordCount());
		LOG(INFO) << "Create Index " + Double2Str(dRate, 2) + "% ..."; 
	}
	catch(_com_error* e)
	{
		LOG(ERROR) << WString2String((wchar_t*)(e->Description()));
		return false;
	}
	return true;
}
bool Importer::__read()
{
	std::ifstream file(__strFile, std::ios::in);
	if(!file.is_open())
	{
		LOG(ERROR) << __strFile + " cannot open!";
		return false;
	}
	LOG(INFO) << "Read File...";
	std::string strLine = "";
	std::string strSpliter = "";
	int iLineCount = 0;
	std::vector<std::string> vctRecord;
	while(!file.eof())
	{
		iLineCount++;
		std::getline(file, strLine);
		if(1 == iLineCount)
		{
			continue;
		}
		strLine = rtrim(strLine, "\n");
		strLine = rtrim(strLine, "\r");
		if(0 == strSpliter.size())
		{
			;
		}
		if(0 == strLine.size())
		{
			continue;
		}
		if(0 == strSpliter.size())
		{
			__getSpliter(strLine, strSpliter);
		}
		vctRecord.clear();
		vctRecord = ParseStr(strLine, strSpliter);
		__mpRecords[iLineCount - 1] = vctRecord;
	}
	return true;
}
bool Importer::__preprocess()
{
	LOG(INFO) << "Connect Database...";
	__gdb.setServer(__strServer);
	__gdb.setDatabase("master");
	if(!__gdb.conn())
	{
		LOG(ERROR) << __gdb.getLastError();
		return false;
	}
	__gdbRoad.setServer(__strServer);
	__gdbRoad.setDatabase(__strRoadDatabase);
	if(!__gdbRoad.conn())
	{
		LOG(ERROR) << __gdbRoad.getLastError();
		return false;
	}
	if(__strDatabase.size() > 0)
	{
		if(!__gdb.isDbExists(__strOldDatabase))
		{
			LOG(ERROR) << __strOldDatabase + " not exist!";
			return false;
		}
		__gdb.setDatabase(__strOldDatabase);
		if(!__gdb.conn())
		{
			LOG(ERROR) << __gdb.getLastError();
			return false;
		}
		if(!__gdb.isTableExists(table_rawdata))
		{
			LOG(ERROR) << "Table: "+ table_rawdata + " not exist!";
			return false;
		}
		if(!__gdb.isTableExists(table_AlertPoint))
		{
			LOG(ERROR) << "Table: "+ table_AlertPoint + " not exist!";
			return false;
		}
		__gdb.setDatabase("master");
		if(!__gdb.conn())
		{
			LOG(ERROR) << __gdb.getLastError();
			return false;
		}
	}
	if(!__gdb.createDB(__strDatabase, true))
	{
		LOG(ERROR) << __gdb.getLastError();
		return false;
	}
	__gdb.setDatabase(__strDatabase);
	if(!__gdb.conn())
	{
		LOG(ERROR) << __gdb.getLastError();
		return false;
	}
	if(!__gdb.execute(sql_create_table_rawdata))
	{
		LOG(ERROR) << __gdb.getLastError();
		return false;
	}
	if(!__gdb.isTableExists(table_rawdata))
	{
		LOG(ERROR) << "Table: " + table_rawdata + " not exist!";
		return false;
	}
	if(!__gdb.execute(sql_create_table_AlertPoint))
	{
		LOG(ERROR) << __gdb.getLastError();
		return false;
	}
	if(!__gdb.isTableExists(table_AlertPoint))
	{
		LOG(ERROR) << "Table: " + table_AlertPoint + " not exist!";
		return false;
	}
	return true;
}
bool Importer::__writeRawData()
{
	LOG(INFO) << "Write to Database: rawdata...";
	std::map<int, std::vector<std::string>>::iterator it;
	std::string strSql = "";
	for(it = __mpRecords.begin(); it != __mpRecords.end(); ++it)
	{
		if(static_cast<int>((it->second).size()) != speedcamera_record_length)
		{
			LOG(WARNING) << "Id " + Num2Str(it->first) +  ": Record Length not equal to " + Num2Str(speedcamera_record_length);
			continue;
		}
		if(StrFindNoCase(it->second[4], "?") != -1)		
		{
			it->second[4] = ReplaceStr(it->second[4], "?", " ");
		}
		strSql = \
			"INSERT INTO " + table_rawdata + " Values ("\
			"" + Num2Str(it->first) + ", "\
			"" + Num2Str(static_cast<int>(Str2Double(it->second[0]) * 1000000)) + ", "\
			"" + Num2Str(static_cast<int>(Str2Double(it->second[1]) * 1000000)) + ", "\
			"" + it->second[2] + ", "\
			"N'" + it->second[3] + "', "\
			"N'" + it->second[4] + "', "\
			"0"\
			")";
		if(!__gdb.execute(strSql))
		{
			LOG(ERROR) << __gdb.getLastError();
			return false;
		}
	}
	strSql = "select * from " + table_rawdata;
	if(!__gdb.execute(strSql))	
	{
		LOG(ERROR) << __gdb.getLastError();
		return false;
	}
	if(__gdb.getRecordsCount() != static_cast<int>(__mpRecords.size()))
	{
		LOG(ERROR) << "Table Records Num not equal to File, please check!";
		return false;
	}
	return true;
}
bool Importer::__process()
{
	if(!__parseLocation())
	{
		return false;
	}
	if(!__createSpatialIndex())
	{
		return false;
	}
	__vctProcessedIds.clear();
	std::map<int, std::vector<std::string>>::iterator it;
	LineInfo li;
	LineInfo li_from;
	LineInfo li_to;
	AlertPoint ap;
	std::vector<LineInfo> vctResultsAt;
	std::vector<LineInfo> vctResultsFrom;
	std::vector<LineInfo> vctResultsTo;
	vctResultsAt.clear();
	vctResultsFrom.clear();
	vctResultsTo.clear();
	li.Reset();
	li_from.Reset();
	li_to.Reset();
	ap.Reset();
	LOG(INFO) << "Process one by one...";
	for(it = __mpRecords.begin(); it != __mpRecords.end(); ++it)
	{
		ap.Reset();
		if((it->second).size() != 9)
		{
			LOG(WARNING) << "Line " + Num2Str(it->first) + " not processed.";
			continue;
		}
		double dLat = Str2Double((it->second)[0]);
		double dLon = Str2Double((it->second)[1]);
		int iCompareAngle = Str2Int((it->second)[5]);
		std::string strAtPlace = (it->second)[6];
		std::string strFromPlace = (it->second)[7];
		std::string strToPlace = (it->second)[8];
		int iSegPntStart = 0;
		int iSegPntEnd = 0;
		int iSegPntStartFrom = 0;
		int iSegPntEndFrom = 0;
		int iSegPntStartTo = 0;
		int iSegPntEndTo = 0;
		li.Reset();
		if(!__searchCircleByLocationName(dLat, dLon, strAtPlace, li, iSegPntStart, iSegPntEnd, iCompareAngle))
		{
			LOG(WARNING) << "Cannot fetch Road " + strFromPlace + " during reasonable times";
		}
		else
		{
			__projectPointOnRoad(dLat, dLon, ap, li, iSegPntStart, iSegPntEnd);
			__calcAngleConincide(ap, li, iSegPntStart, iSegPntEnd);
		}
		__setAlertPoint(ap, li, it);
		if(!__uploadToDB(ap))
		{
			return false;
		}
		//LOG(INFO) << "Location: N" + Double2Str(dLat, 6) + " E" + Double2Str(dLon, 10);
		//__showAlertPoint(ap);
		//std::string strAtIds = "";
		//std::string strFromIds = "";
		//std::string strToIds = "";
		//for(int i = 0; i < static_cast<int>(vctResultsAt.size()); ++i)
		//{
		//	strAtIds += Num2Str(vctResultsAt[i].OneWay) + ", ";
		//}
		//for(int i = 0; i < static_cast<int>(vctResultsFrom.size()); ++i)
		//{
		//	strFromIds += Num2Str(vctResultsFrom[i].OneWay) + ", ";
		//}
		//for(int i = 0; i < static_cast<int>(vctResultsTo.size()); ++i)
		//{
		//	strToIds += Num2Str(vctResultsTo[i].OneWay) + ", ";
		//}
		//strAtIds = rtrim(strAtIds, ", ");
		//strFromIds = rtrim(strFromIds , ", ");
		//strToIds = rtrim(strToIds , ", ");
		//if(static_cast<int>(strAtIds.size()) == 0)
		//{
		//	LOG(INFO) << "At OneWays: NULL";
		//}
		//else
		//{
		//	LOG(INFO) << "At OneWays: " + strAtIds;
		//}
		//LOG(INFO) << "From Ids:" + strFromIds;
		//LOG(INFO) << "To Ids:" + strToIds;
	}	
	return true;
}
bool Importer::__parseLocation()
{
	std::string strAtPlace = "";
	std::string strFromPlace = "";
	std::string strToPlace = "";
	std::vector<std::string> vctLocations;
	vctLocations.clear();
	std::map<int, std::vector<std::string>>::iterator it;
	for(it = __mpRecords.begin(); it != __mpRecords.end(); ++it)
	{
		vctLocations.clear();
		strAtPlace = "";
		strFromPlace = "";
		strToPlace = "";
		if(0 < StrFindNoCase((it->second)[4], location_identifier_maj))
		{
			vctLocations = ParseStr((it->second)[4], location_identifier_maj);
			if(2 == static_cast<int>(vctLocations.size()))
			{
				std::string strFirstPart = vctLocations[0];
				strToPlace = vctLocations[1];
				if(0 < StrFindNoCase(vctLocations[0], location_identifier_min_1))
				{
					vctLocations = ParseStr(strFirstPart, location_identifier_min_1); 
					strFromPlace = vctLocations[1];
					strAtPlace = vctLocations[0];
				}
				else
				{
					strFromPlace = strFirstPart;
					strAtPlace = strFirstPart;
				}
			}
			if(0 == static_cast<int>(strAtPlace.size()))
			{
				LOG(WARNING) << "ID: " + Num2Str(it->first) + " At Place NULL";
			}
			if(0 == static_cast<int>(strFromPlace.size()))
			{
				LOG(WARNING) << "ID: " + Num2Str(it->first) + " From Place NULL";
			}
			if(0 == static_cast<int>(strToPlace.size()))
			{
				LOG(WARNING) << "ID: " + Num2Str(it->first) + " To Place NULL";
			}
			(it->second).push_back(strAtPlace);
			(it->second).push_back(strFromPlace);
			(it->second).push_back(strToPlace);
		}
	}
	return true;
}
bool Importer::__createRawdataDiff()
{
	if(!__gdb.isOpen())
	{
		if(!__gdb.conn())
		{
			LOG(ERROR) << "Databse Connect Error!";
			return false;
		}
	}
	if(__gdb.isTableExists(table_rawdata_plus))
	{
		if(!__gdb.deleteTable(table_rawdata_plus))
		{
			LOG(ERROR)  << __gdb.getLastError();
			return false;
		}
	}
	std::string strSql = \
		"select * " \
		"into " + __strDatabase + ".." + table_rawdata_plus + " "\
		"from "+ __strDatabase + ".." + table_rawdata + " "\
		"where id not in (select r2.id from " +\
		__strOldDatabase + ".." + table_rawdata + " r1, " +\
		__strDatabase + ".." + table_rawdata + " r2 " +\
		"where r1.location = r2.location and r1.name = r2.name and (abs(r1.lat - r2.lat) < 400 and abs(r1.lon - r2.lon) < 400));";
	try
	{
		if(!__gdb.execute(strSql))
		{
			LOG(ERROR) << __gdb.getLastError();
			return false;
		}
	}
	catch(_com_error* e)
	{
		LOG(ERROR) << WString2String((wchar_t*)(e->Description()));
		return false;
	}
	if(!__gdb.isTableExists(table_rawdata_plus))
	{
		LOG(ERROR) << "Table " + table_rawdata_plus + " not exist!";
		return false;
	}
	if(__gdb.isTableExists(table_rawdata_minus))
	{
		if(!__gdb.deleteTable(table_rawdata_minus))
		{
			LOG(ERROR)  << __gdb.getLastError();
			return false;
		}
	}
	strSql = \
		"select * " \
		"into " + __strDatabase + ".." + table_rawdata_minus + " "\
		"from "+ __strOldDatabase + ".." + table_rawdata + " "\
		"where id not in (select r1.id from " +\
		__strOldDatabase + ".." + table_rawdata + " r1, " +\
		__strDatabase + ".." + table_rawdata + " r2 " +\
		"where r1.location = r2.location and r1.name = r2.name and (abs(r1.lat - r2.lat) < 400 and abs(r1.lon - r2.lon) < 400));";
	try
	{
		if(!__gdb.execute(strSql))
		{
			LOG(ERROR) << __gdb.getLastError();
			return false;
		}
	}
	catch(_com_error* e)
	{
		LOG(ERROR) << WString2String((wchar_t*)(e->Description()));
		return false;
	}
	if(!__gdb.isTableExists(table_rawdata_minus))
	{
		LOG(ERROR) << "Table " + table_rawdata_minus + " not exist!";
		return false;
	}
	return true;
}
bool Importer::__copyAlertPointsFromOld()
{
	if(!__gdb.isOpen())
	{
		if(!__gdb.conn())
		{
			LOG(ERROR) << "Databse Connect Error!";
			return false;
		}
	}
	std::string strSql = \
		"insert " \
		"into " + __strDatabase + ".." + table_AlertPoint + " "\
		"select a.Alert_Type, a.Icon_File_Path, r2.id, a.Alert_Dist, a.Alert_Angle, a.Name, a.Comment, a.Lat, a.Lon, a.GRM_Code, a.Offset, a.Line_feature_flags, a.Line_code, a.Line_attribute, a.Coincide, a.Line_Id "\
		"from "+ __strOldDatabase + ".." + table_AlertPoint + " a, " +\
		__strOldDatabase + ".." + table_rawdata + " r1, " +\
		__strDatabase + ".." + table_rawdata + " r2 " \
		"where r1.location = r2.location and r1.name = r2.name and (abs(r1.lat - r2.lat) < 400 and abs(r1.lon - r2.lon) < 400) and r1.id = a.Point_Id and r1.id not in (select id from " + \
		__strDatabase + ".." + table_rawdata_minus + ");";
	try
	{
		if(!__gdb.execute(strSql))
		{
			LOG(ERROR) << __gdb.getLastError();
			return false;
		}
	}
	catch(_com_error* e)
	{
		LOG(ERROR) << WString2String((wchar_t*)(e->Description()));
		return false;
	}
	return true;
}
bool Importer::__addAngleInfo()
{
	LOG(INFO) << "Add Angle Info...";
	if(!__gdb.isOpen())
	{
		if(!__gdb.conn())
		{
			LOG(ERROR) << __gdb.getLastError();
			return false;
		}
	}
	std::string strTableBCP;
	if(__gdb.isTableExists(table_rawdata_plus))
	{
		strTableBCP = table_rawdata_plus;
	}
	else
	{
		strTableBCP = table_rawdata;
	}
	if(!BCPOut(__strServer, __strDatabase, strTableBCP, file_internal_add_angle, Spliter_DoubleString))
	{
		LOG(ERROR) << "BCP out error!";
		return false;
	}
	std::string strExe;
	strExe = exe_angle_parser + " -i " + file_internal_add_angle + " -s \"" + Spliter_DoubleString + "\" 1> angle.log";
	system(strExe.c_str());
	if(FindStrInFile("angle.log", "error"))
	{
		LOG(ERROR) << "Angle Parser run error!";
		return false;
	}
	DeleteFileM("angle.log");
	std::string strSql = "delete from " + strTableBCP;
	try
	{
		if(!__gdb.execute(strSql))
		{
			LOG(ERROR) << __gdb.getLastError();
			return false;
		}
	}
	catch(_com_error* e)
	{
		LOG(ERROR) << WString2String((wchar_t*)(e->Description()));
		return false;
	}
	if(!BCPIn(__strServer, __strDatabase, strTableBCP, file_internal_add_angle, Spliter_DoubleString))
	{
		LOG(ERROR) << "BCP in error!";
		return false;
	}
	if(ChkFileExist(file_internal_add_angle))
	{
		DeleteFileM(file_internal_add_angle);
	}
	_RecordsetPtr _rst = NULL;
	
	std::list<int> _lstIds;
	try
	{
		std::string strSql = "select id, angle from " + strTableBCP;
		if(!__gdb.execute(strSql))
		{
			LOG(ERROR) << __gdb.getLastError();	
			return false;
		}
		GRecordSet* pRst;
		pRst = __gdb.getRecordset();
		_rst = pRst->getRecordsetPtr();
		if(_rst == NULL)
		{
			return false;
		}
		_rst->MoveFirst();
		while(!_rst->EndOfFile)
		{
			_variant_t curValue;
			int iId = 0;
			int iAngle = 0;
			curValue = _rst->GetFields()->GetItem("id")->GetValue();
			if(curValue.vt != VT_NULL && curValue.vt != VT_EMPTY)
			{
				iId = curValue.intVal;
			}
			curValue = _rst->GetFields()->GetItem("angle")->GetValue();
			if(curValue.vt != VT_NULL && curValue.vt != VT_EMPTY)
			{
				iAngle = curValue.intVal;
			}
			__mpRecords[iId].push_back(Num2Str(iAngle));
			_rst->MoveNext();
		}
	}
	catch(_com_error* e)
	{
		LOG(ERROR) << WString2String((wchar_t*)(e->Description()));
		return false;
	}
	return true;
}
bool Importer::__createGpx()
{
	LOG(INFO) << "Create Gpx...";
	if(!__createGpx(sql_fetch_gpx_rawdata_plus, file_gpx_plus))
	{
		LOG(ERROR) << "Create rawdata_plus gpx error!";
		return false;
	}
	if(!__createGpx(sql_fetch_gpx_rawdata_minus, file_gpx_minus))
	{
		LOG(ERROR) << "Create rawdata_plus gpx error!";
		return false;
	}
	if(!__createGpx(sql_fetch_gpx_AleterPoint, file_gpx_AlertPoint))	
	{
		LOG(ERROR) << "Create AlertPoint gpx error!";
		return false;
	}
	return true;
}

bool Importer::__createGpx(const std::string& strSql,  const std::string& strFile)
{
	if(!__gdb.isOpen())
	{
		if(!__gdb.conn())
		{
			LOG(ERROR) << __gdb.getLastError();
			return false;
		}
	}
	if(__gdb.isTableExists(table_temp_gpx))
	{
		if(!__gdb.deleteTable(table_temp_gpx))
		{
			LOG(ERROR) << __gdb.getLastError();
			return false;
		}
	}
	if(!__gdb.execute(strSql))
	{
		LOG(ERROR) << __gdb.getLastError();
		return false;
	}
	if(!BCPOut(__strServer, __strDatabase, table_temp_gpx, file_gpx_internal, Spliter_Comma))
	{
		LOG(ERROR) << "BCP out error!";
		return false;
	}
	std::string strExe;
	strExe = exe_gpxbuilder + " -i " + file_gpx_internal + " -o " + strFile;
	system(strExe.c_str());
	if(ChkFileExist(file_gpx_internal))
	{
		DeleteFileM(file_gpx_internal);
	}
	if(__gdb.isTableExists(table_temp_gpx))
	{
		if(!__gdb.deleteTable(table_temp_gpx))
		{
			LOG(ERROR) << __gdb.getLastError();
			return false;
		}
	}	
	return true;
}

bool Importer::__show()
{
	std::map<int, std::vector<std::string>>::iterator it;
	for(it = __mpRecords.begin(); it != __mpRecords.end(); ++it)
	{
		int iLength = static_cast<int>((it->second).size());
		for(int i = 0; i < iLength; ++i)
		{
			LOG(INFO) << (it->second)[i];
		}
	}
	return true;
}
bool Importer::__showAlertPoint(AlertPoint& ap)
{
	std::string strOutput = "";
	strOutput += "AlertPoint: ";
	strOutput += " AlertType: ";
	strOutput += ap.AlertType;
	strOutput += " Icon_File_Path: ";
	strOutput += ap.Icon_File_Path;
	strOutput += " Point_Id: ";
	strOutput += Num2Str(ap.Point_Id);
	strOutput += " Alert_Dist: ";
	strOutput += Num2Str(ap.Alert_Dist);
	strOutput += " Alert_Angle: ";
	strOutput += Num2Str(ap.Alert_Angle);
	strOutput += " Name: ";
	strOutput += ap.Name;
	strOutput += " Comment: ";
	strOutput += ap.Comment;
	strOutput += " Lat: ";
	strOutput += Num2Str(ap.Lat);
	strOutput += " Lon: ";
	strOutput += Num2Str(ap.Lon);
	strOutput += " GRM_Code: ";
	strOutput += Num2Str(ap.GRM_Code);
	strOutput += " offset: ";
	strOutput += Num2Str(ap.offset);
	strOutput += " Line_Code: ";
	strOutput += Num2Str(ap.Line_Code);
	strOutput += " Line_Attribute: ";
	strOutput += Num2Str(ap.Line_Attribute);
	strOutput += " Conincide: ";
	strOutput += Num2Str(ap.Conincide);
	strOutput += " Line_Id: ";
	strOutput += Num2Str(ap.Line_Id);
	LOG(INFO) << strOutput;
	return true;
}