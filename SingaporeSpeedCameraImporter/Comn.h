#pragma once
#include "ThirdParty/easylogging++.h"
#include <string>
#include <vector>
#include <list>
#include <map>

extern const std::string Spliter_Comma;
extern const std::string Spliter_Space;
extern const std::string Spliter_Semicolon;
extern const std::string Spliter_DoubleString;
extern const std::string Spliter_SingleString;

extern const int speedcamera_record_length;
extern const int createindex_split_num;
extern const double math_pi;
extern const double RTOD;

extern const std::string table_rawdata;
extern const std::string table_rawdata_plus;
extern const std::string table_rawdata_minus;
extern const std::string table_temp_gpx;
extern const std::string table_AlertPoint;
extern const std::string sql_create_table_rawdata;
extern const std::string sql_create_table_AlertPoint;
extern const std::string sql_fetch_line_lat_lon;
extern const std::string sql_fetch_line_info;
extern const std::string sql_fetch_linepts_info;
extern const std::string sql_fetch_lineids_by_latlon;
extern const std::string sql_fetch_gpx_rawdata_plus;
extern const std::string sql_fetch_gpx_rawdata_minus;
extern const std::string sql_fetch_gpx_AleterPoint;
extern const std::string location_identifier_maj;
extern const std::string location_identifier_min_1;
extern const std::string file_internal_add_angle;
extern const std::string file_gpx_internal;
extern const std::string file_gpx_plus;
extern const std::string file_gpx_minus;
extern const std::string file_gpx_AlertPoint;
extern const std::string exe_angle_parser;
extern const std::string exe_gpxbuilder;

extern const std::string Arg_Icon_File_Path;
extern const int Arg_Alert_Dist;
extern const int Arg_Offset;
extern const int Arg_GRM_Code;
extern const int Arg_Line_Attribute;
extern const int Arg_Line_Flags;

extern const char *array_Spliters[]; 

extern const std::vector<std::string> vctSpliters;

enum Option
{
	Server = 0,
	Database,
	OldDatabase,
	RoadDatabase,
	File,
	Other
};
