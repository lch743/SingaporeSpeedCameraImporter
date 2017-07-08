#pragma once
#include "GMGeometry.h"

#ifdef GPOINT_EXPORTS
#define GPOINT_API __declspec( dllexport )
#else
#define GPOINT_API __declspec( dllimport )
#endif

class GPOINT_API GPOINT
{
public:
	GPOINT();
	GPOINT( float fLat, float fLon);
	GPOINT( const GPOINT& other );
	float getLat() const;
	float getLon() const;
	void setLat( float fLat );
	void setLon( float fLon );
	void operator = ( const GPOINT& other );
	bool operator == ( const GPOINT& other) const;
private:
	float m_fLat;
	float m_fLon;
	float m_fZlevel;
};

class GCoordinate
{
public:
	GCoordinate();
	GCoordinate( double dLat, double dLon);
	double getLat() const;
	double getLon() const;
	void setLat( const double dLat );
	void setLon( const double dLon );
	void operator = ( const GCoordinate& other );
	bool operator == ( const GCoordinate& other) const;
private:
	double m_dLat;
	double m_dLon;
};