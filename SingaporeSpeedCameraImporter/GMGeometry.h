#pragma once

#ifdef GMGEOMETRY_EXPORTS
#define GMGEOMETRY_API  __declspec(dllexport)
#else
#define GMGEOMETRY_API __declspec(dllimport)
#endif

class GMGEOMETRY_API GMGeometry
{
public:
	enum GMGeoType
	{
		GM_Point = 1,
		GM_Line = 2,
		GM_Area = 3
	};
};