#ifndef SPATIALINDEX_H
#define SPATIALINDEX_H

/*********************************************************************

Copyright (C) 2012, Jason(one Dragon in China), Garmin China Shanghai Office

说明：本类是描述的空间查询的算法，是建立在R树基础上的索引。1000万数据量，创建索引的的大致时间在150秒，遍历整棵树大致时间在131秒。

**********************************************************************/
#include <vector>
#include <list>
#include <iterator>
#include "stdafx.h"
#include "GDeliver.h"
#include "RTree.h"
#include "GPoint.h"
using namespace std;

#ifdef SPATIALINDEX_EXPORTS
#define SPATIALINDEX_API __declspec( dllexport )
#else
#define SPATIALINDEX_API __declspec( dllimport )
#endif

struct RTreeNode
{
	RTreeNode()
	{
		++s_outstandingAllocs;
	}
	~RTreeNode()
	{
		--s_outstandingAllocs;
	}
	int m_creationCounter;
	GPOINT* m_min;
	GPOINT* m_max;
	static int s_outstandingAllocs;
};
typedef RTree< RTreeNode*, float, 2 >  GMRTree;
class SPATIALINDEX_API SpatialIndex
{
public:
	SpatialIndex();
	~SpatialIndex();
	bool BoxesIntersect( const GPOINT& gpBoxMinA, const GPOINT& gpBoxMaxA, const GPOINT& gpBoxMinB, const GPOINT& gpBoxMaxB );
	bool CreateIndex( const GDeliverSpatialIndex* pDelivers, int iCount );
	bool QueryByPoint(float lat, float lon, double radius, std::vector<int>& vctResult);
	bool QueryByPoint( const GPOINT& pnt, double radius, std::vector<int>& vctResult );
	bool QueryByCoordinate( const GCoordinate& pnt, double radius, std::vector<int>& vctResult  );
	bool QueryByBox( const GPOINT& pntMin, const GPOINT& pntMax, std::vector<int>& vctResult );
	bool QueryByBox( float Min_Lat, float Min_Lon, float Max_Lat, float Max_Lon, std::vector<int>& vctResult );
	static bool _cdecl QueryResultCallback( RTreeNode* a_data, void* a_context );
	bool ISNULL();
	bool RemoveSpatialIndex();
	int GetCount();
private:
	GMRTree m_RTree;
};



#endif 