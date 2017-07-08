#pragma once
#include <string>

struct AlertPoint
{
	std::string AlertType;
	std::string Icon_File_Path;
	int Point_Id;
	int Alert_Dist;
	int Alert_Angle;
	std::string Name;
	std::string Comment;
	int Lat;
	int Lon;
	int GRM_Code;
	int offset;
	int Line_flags;
	int Line_Code;
	int Line_Attribute;
	int Conincide;
	int Line_Id;
	AlertPoint::AlertPoint(const AlertPoint&other)
	{
		AlertType = other.AlertType;
		Icon_File_Path = other.Icon_File_Path;		
		Point_Id = other.Point_Id;			
		Alert_Dist = other.Alert_Dist;		
		Alert_Angle = other.Alert_Angle;				
		Name = other.Name;			
		Comment = other.Comment;				
		Lat = other.Lat;	
		Lon = other.Lon;	
		GRM_Code = other.GRM_Code;	
		offset = other.offset;	
		Line_flags = other.Line_flags;
		Line_Code = other.Line_Code;	
		Line_Attribute = other.Line_Attribute;	
		Conincide = other.Conincide;	
		Line_Id = other.Line_Id;	
	}
	AlertPoint::AlertPoint()
	{
	}
	void operator = (const AlertPoint&other)
	{
		AlertType = other.AlertType;
		Icon_File_Path = other.Icon_File_Path;		
		Point_Id = other.Point_Id;			
		Alert_Dist = other.Alert_Dist;		
		Alert_Angle = other.Alert_Angle;				
		Name = other.Name;			
		Comment = other.Comment;				
		Lat = other.Lat;	
		Lon = other.Lon;	
		GRM_Code = other.GRM_Code;	
		offset = other.offset;	
		Line_flags = other.Line_flags;
		Line_Code = other.Line_Code;	
		Line_Attribute = other.Line_Attribute;	
		Conincide = other.Conincide;	
		Line_Id = other.Line_Id;
	}
	void AlertPoint:: Reset()
	{
		AlertType = "";
		Icon_File_Path = "";
		Point_Id = 0;			
		Alert_Dist = 0;
		Alert_Angle = 0;
		Name = "";
		Comment = "";
		Lat = 0;
		Lon = 0;
		GRM_Code = 0;
		offset = 0;
		Line_flags = 0;
		Line_Code = 0;
		Line_Attribute = 0;
		Conincide = 0;
		Line_Id = 0;
	}
};