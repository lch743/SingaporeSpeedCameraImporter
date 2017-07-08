#include "Comn.h"
const std::string Spliter_Comma = ",";
const std::string Spliter_Space = " ";
const std::string Spliter_Semicolon = ";";
const std::string Spliter_DoubleString = "||";
const std::string Spliter_SingleString = "|";

const char *array_Spliters[] =
{
	Spliter_Comma.c_str(), 
	Spliter_Space.c_str(), 
	Spliter_Semicolon.c_str(),
	Spliter_DoubleString.c_str(),
	Spliter_SingleString.c_str()
};

const std::vector<std::string> vctSpliters(array_Spliters, &array_Spliters[sizeof(array_Spliters)/sizeof(array_Spliters[0])]);

const int speedcamera_record_length = 5;
const int createindex_split_num = 100000;
const double math_pi = 3.141592653589793;
const double RTOD = 57.29577951308232;

const std::string table_rawdata = "rawdata";
const std::string table_rawdata_plus = "rawdata_plus";
const std::string table_rawdata_minus = "rawdata_minus";
const std::string table_temp_gpx = "temp_gpx";
const std::string table_AlertPoint = "AlertPoint";

const std::string sql_create_table_rawdata = \
"CREATE TABLE [dbo].[" + table_rawdata + "](			"\
"	[id] [int] NOT NULL,				"\
"	[lat] [int] NOT NULL,				"\
"	[lon] [int] NOT NULL,				"\
"	[sub_id] [int] NOT NULL,			"\
"	[name] [varchar](100) NOT NULL,		"\
"	[location] [varchar](500) NOT NULL,	"\
"   [Angle] [int] NOT NULL"
") ON [PRIMARY]";

const std::string sql_create_table_AlertPoint = \
"CREATE TABLE [dbo].[" + table_AlertPoint + "](			"\
"	[Alert_Type] [nvarchar](100) NULL,		"\
"	[Icon_File_Path] [nvarchar](500) NULL,	"\
"	[Point_Id] [int] NULL,					"\
"	[Alert_Dist] [int] NULL,				"\
"	[Alert_Angle] [int] NULL,				"\
"	[Name] [nvarchar](100) NULL,			"\
"	[Comment] [nvarchar](100) NULL,			"\
"	[Lat] [int] NULL,						"\
"	[Lon] [int] NULL,						"\
"	[GRM_Code] [int] NULL,					"\
"	[offset] [int] NULL,					"\
"	[Line_feature_flags] [int] NULL,		"\
"	[Line_code] [int] NULL,					"\
"	[Line_attribute] [int] NULL,			"\
"	[Coincide] [int] NULL,					"\
"	[Line_ID] [int] NULL					"\
") ON [PRIMARY]";

const std::string sql_fetch_line_lat_lon = \
"SELECT Line_Id,Min_Lat,Max_Lat,Min_Lon,Max_Lon "\
"FROM Line "\
"WHERE Deleted  = 0 AND Line_code>=2010 AND Line_code<2040";

const std::string sql_fetch_line_info = \
"select * "\
"from "\
"( "\
"select Line_Id, Min_Lat, Min_Lon, Max_Lat, Max_Lon, One_Way, Line_Code, SUBSTRING(Name, 0, CHARINDEX('{L', Name, 0)) as Name "\
"from Line "\
"where Name not like N'%{M%' "\
"and LEN(Name) > 0 "\
"union all "\
"select Line_Id, Min_Lat, Min_Lon, Max_Lat, Max_Lon, One_Way, Line_Code, SUBSTRING(Name, CHARINDEX('@', Name, 0) + 1, CHARINDEX('{L', Name, CHARINDEX('@', Name, 0) + 1) - CHARINDEX('@', Name, 0) - 1) as Name "\
"from Line "\
"where Name like N'%{M%' "\
")A";

const std::string sql_fetch_linepts_info = \
"select lat, lon "\
"from Line_pts ";

const std::string sql_fetch_lineids_by_latlon = \
"select l.Line_Id " \
"from Line l, line_pts lp " \
"where l.Line_Id = lp.Line_Id";

const std::string sql_fetch_gpx_rawdata_plus = \
"select cast(id as varchar(10)) + '_' + name + '_' + location + '_' + cast(Angle as varchar(3)) as Name, lat, lon "\
"into " + table_temp_gpx + " "\
"from " + table_rawdata_plus;

const std::string sql_fetch_gpx_rawdata_minus = \
"select cast(id as varchar(10)) + '_' + name + '_' + location as Name, lat, lon "\
"into " + table_temp_gpx + " "\
"from " + table_rawdata_minus;

const std::string sql_fetch_gpx_AleterPoint = \
"select cast(Point_Id as varchar(15)) + '_' + Alert_Type + '_' + cast(Alert_Angle as varchar(3))  + '_' + cast(Alert_Dist as varchar(4)) + '_' + cast(offset as varchar(3)) + '_' + cast(Coincide as varchar(1)) as Name, lat, lon "\
"into " + table_temp_gpx + " "\
"from " + table_AlertPoint;

const std::string location_identifier_maj = " towards ";
const std::string location_identifier_min_1 = " by ";

const std::string file_internal_add_angle = "temp_add_angle.txt";
const std::string file_gpx_internal = "temp_gpx.txt";
const std::string file_gpx_plus = "rawdata_plus.gpx";
const std::string file_gpx_minus = "rawdata_minus.gpx";
const std::string file_gpx_AlertPoint = "AlertPoint.gpx";
const std::string exe_angle_parser = "SpeedCameraAngleParser.exe";
const std::string exe_gpxbuilder = "GpxBuilder.exe";

const std::string Arg_Icon_File_Path = "\\\\apollo\\Dev_Auto\\BuildMapProcessing\\MapProcessFiles\\Common\\SpeedCamara_Icon\\SC.bmp";
const int Arg_Alert_Dist = 400;
const int Arg_GRM_Code = 4102;
const int Arg_Offset = 10;
const int Arg_Line_Attribute = 0;
const int Arg_Line_Flags = 0;