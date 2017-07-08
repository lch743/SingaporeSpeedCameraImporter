#pragma once
#include <string>
#include <vector>

struct Coordinate
{
	double Lat;
	double Lon;
	Coordinate::Coordinate(const Coordinate&other)
	{
		Lat = other.Lat;
		Lon = other.Lon;
	}
	Coordinate::Coordinate()
	{
		Lat = 0;
		Lon = 0;
	}
	void operator = (const Coordinate&other)
	{
		Lat = other.Lat;
		Lon = other.Lon;
	}
	bool operator == (const Coordinate &other) const
	{
		return 	Lat == other.Lat && Lon == other.Lon;
	}
};
struct IntCoordinate
{
	int Lat;
	int Lon;
	IntCoordinate::IntCoordinate(const IntCoordinate&other)
	{
		Lat = other.Lat;
		Lon = other.Lon;
	}
	IntCoordinate::IntCoordinate()
	{
		Lat = 0;
		Lon = 0;
	}
	void operator = (const IntCoordinate&other)
	{
		Lat = other.Lat;
		Lon = other.Lon;
	}
	bool operator == (const IntCoordinate &other) const
	{
		return 	Lat == other.Lat && Lon == other.Lon;
	}
};
struct LineInfo
{
	int LineId;
	int MinLat;
	int MinLon;
	int MaxLat;
	int MaxLon;
	int OneWay;
	int LineCode;
	int pntCount;
	std::string Name;
	std::vector<Coordinate> vctPts;
	std::vector<IntCoordinate> vctPtsInt;
	LineInfo::LineInfo(const LineInfo&other)
	{
		LineId = other.LineId;		
		MinLat = other.MinLat;			
		MinLon = other.MinLon;		
		MaxLat = other.MaxLat;				
		MaxLon = other.MaxLon;			
		OneWay = other.OneWay;				
		LineCode = other.LineCode;				
		pntCount = other.pntCount;
		Name = other.Name;	

		vctPts.clear();
		vctPts.resize(other.vctPts.size());
		copy(other.vctPts.begin(),other.vctPts.end(),vctPts.begin());
		vctPtsInt.clear();
		vctPtsInt.resize(other.vctPtsInt.size());
		copy(other.vctPtsInt.begin(),other.vctPtsInt.end(),vctPtsInt.begin());
	}
	LineInfo::LineInfo()
	{
	}
	void operator = (const LineInfo&other)
	{
		LineId = other.LineId;		
		MinLat = other.MinLat;			
		MinLon = other.MinLon;		
		MaxLat = other.MaxLat;				
		MaxLon = other.MaxLon;			
		OneWay = other.OneWay;				
		LineCode = other.LineCode;		
		pntCount = other.pntCount;
		Name = other.Name;	

		vctPts.clear();
		vctPts.resize(other.vctPts.size());
		copy(other.vctPts.begin(),other.vctPts.end(),vctPts.begin());
		vctPtsInt.clear();
		vctPtsInt.resize(other.vctPtsInt.size());
		copy(other.vctPtsInt.begin(),other.vctPtsInt.end(),vctPtsInt.begin());
	}
	void LineInfo:: Reset()
	{
		LineId = 0;		
		MinLat = 0;			
		MinLon = 0;		
		MaxLat = 0;				
		MaxLon = 0;			
		OneWay = 0;				
		LineCode = 0;				
		pntCount = 0;
		Name = "";	
		vctPts.clear();
		vctPtsInt.clear();
	}
};
