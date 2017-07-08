#ifndef IMPORTER_H
#define IMPORTER_H
/*------------------------------------------------------------------------------
                           GENERAL INCLUDES
------------------------------------------------------------------------------*/
#include "Comn.h"
#include "Database.h"
#include "SpatialIndex.h"
/*---------------------------------------------------------------------
                                TYPES
---------------------------------------------------------------------*/
struct Coordinate;
struct LineInfo;
struct AlertPoint;
/*---------------------------------------------------------------------
                                CLASSES
---------------------------------------------------------------------*/
class Importer
{
//---------------------------------------------------------------------
public: // FUNCTIONS
//-------------------------------------------------------------------
	Importer():
		__strServer(""),
		__strDatabase(""),
		__strOldDatabase(""),
		__strRoadDatabase(""),
		__strFile(""),
		__pSpatialIndex(NULL){};
	Importer(const std::string& strServer, const std::string& strDatabase, const std::string& strRoadDatabase, const std::string& strFile, const std::string& strOldDatabase=""):
		__strServer(strServer),
		__strDatabase(strDatabase),
		__strOldDatabase(strOldDatabase),
		__strRoadDatabase(strRoadDatabase),
		__strFile(strFile),
		__pSpatialIndex(NULL){};
	~Importer();
	bool go();
//---------------------------------------------------------------------
private: // FUNCTION 
//--------------------------------------------------------------------
	bool __init();
	bool __checkArguments();
	bool __getSpliter(const std::string& strLine, std::string& strSpliter);
	bool __getLineInfoById(int iLineId, LineInfo& li);
	bool __addLinePtsInfo(LineInfo& li);
	double __getNeastLine(double dLat, double dLon, std::vector<LineInfo>& vctResults, LineInfo& li, int& iSegPntStart, int& iSegPntEnd, int iCompareAngle = -9999);
	double __distance2Line(const Coordinate& pnt, LineInfo& li, int& iSegPntStart, int& iSegPntEnd);
	double __distanct2Segment(const Coordinate& pnt, const Coordinate& pntStart, const Coordinate& pntEnd);
	bool __isPrjOnSegment(const Coordinate& pnt, const Coordinate& pntStart, const Coordinate& pntEnd);
	bool __searchAlongRoadByLocationName(int iLineId, int iLat, int iLon, const std::string& strSearch, LineInfo& liOut);
	bool __searchCircleByLocationName(double dLat, double dLon, const std::string& strSearch, LineInfo& liOut, int& iSegPntStart, int& iSegPntEnd, int iCompareAngle);
	bool __fetchLineIdsByPnt(int iLineId, int iPntLat, int iPntLon, std::list<int>& lstLineIds);
	bool __projectPointOnRoad(double dLat, double dLon, AlertPoint& ap, LineInfo& li, int iSegPntStart, int iSegPntEnd);
	int __calcAngleFromToLineInfo(LineInfo& li_from, LineInfo& li_to);
	bool __calcAngleConincide(AlertPoint& ap, LineInfo& li, int iSegPntStart, int iSegPntEnd);
	double __calcAngleBy2Pnt(const Coordinate &Coord_Angle_F, const Coordinate &Coord_Angle_T);
	int __diffAngle(int iAngleCompareF, int iAngleCompareT);
	bool __setAlertPoint(AlertPoint& ap, LineInfo& li, std::map<int, std::vector<std::string>>::iterator it);
	bool __uploadToDB(AlertPoint& ap);
	bool __updateRecordsWithDiff();
	bool __createSpatialIndex();
	bool __read();
	bool __preprocess();
	bool __writeRawData();
	bool __process();
	bool __parseLocation();
	bool __createRawdataDiff();
	bool __copyAlertPointsFromOld();
	bool __addAngleInfo();
	bool __createGpx();
	bool __createGpx(const std::string& strSql, const std::string& strFile);
	bool __show();
	bool __showAlertPoint(AlertPoint& ap);
//---------------------------------------------------------------------
private: // Data
//--------------------------------------------------------------------
	std::string __strServer;
	std::string __strDatabase;
	std::string __strOldDatabase;
	std::string __strRoadDatabase;
	std::string __strFile;
	std::map<int, std::vector<std::string>> __mpRecords;
	std::vector<int> __vctProcessedIds;
	GDatabase __gdb;
	GDatabase __gdbRoad;
	SpatialIndex* __pSpatialIndex;
};

#endif