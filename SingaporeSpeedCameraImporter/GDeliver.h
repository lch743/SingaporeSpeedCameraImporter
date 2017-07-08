#pragma once
#include "GPoint.h"

#ifdef DELIVER_EXPORTS
#define DELIVER_API __declspec( dllexport )
#else
#define DELIVER_API __declspec( dllimport )
#endif

class DELIVER_API GDeliverSpatialIndex
{
public:
	GDeliverSpatialIndex();
	GDeliverSpatialIndex( int iLineId, const GPOINT& gpMin, const GPOINT& gpMax );
	GDeliverSpatialIndex( int iPointId, const GPOINT& gp );
	int getLineId() const;
	int getPointId() const;
	GPOINT getGpMin() const;
	GPOINT getGpMax() const;
	GPOINT getGp() const;
	void setLineId( int iLineId );
	void setGpMin( const GPOINT& other );
	void setGpMax( const GPOINT& other );
	void setGpMinLat(float fLat);
	void setGpMinLon(float fLon);
	void setGpMaxLat(float fLat);
	void setGpMaxLon(float fLon);
	void setPointId(int iPointId);
	void setGp(const GPOINT& other);
	void operator = ( const GDeliverSpatialIndex& other );
	bool operator == ( const GDeliverSpatialIndex& other) const;
	void reset();
private:
	int m_iLineId;
	int m_iPointId;
	GPOINT m_gp;
	GPOINT m_gpMin;
	GPOINT m_gpMax;
};