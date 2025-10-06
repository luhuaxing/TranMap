
#pragma once
# include "TinHeap.h"
# include "heap_node.h"
# include "point_table.h"
# include "table_row.h"
# include "table_row_node.h"
# include "tintypes.h"
# include "triangle_table.h"
# include "GridHeader.h"
# include "Grid.h"
# include "varH.h"
# include "vertex.h"
# include "vertex_table.h"
# include <math.h>
# include <stdlib.h>
# include <string>

// CTin
enum PerpSplitMethod 
{ 
	TinEdge, 
	SubsetEdge 
};

//structures
typedef struct {
   int p1,p2,p3;
   int b1,b2,b3;
} ITRIANGLE;

typedef struct {
   int p1,p2;
} IEDGE;

typedef struct {
   double x,y,z;
} XYZ;

class  CTin 
{
public:
	CTin();
	~CTin();




// CTin
public:
	void TriangleNeighbors(/*[in]*/ long TriIndex, /*[in, out]*/ long * triIndex1, /*[in, out]*/ long * triIndex2, /*[in, out]*/ long * triIndex3);
	void get_IsNDTriangle(/*[in]*/ long TriIndex, /*[out, retval]*/ VARIANT_BOOL *pVal);
	void get_Filename(/*[out, retval]*/ BSTR *pVal);
	void Min(/*[out]*/ double * X, /*[out]*/ double * Y, /*[out]*/ double * Z);
	void Max(/*[out]*/ double * X, /*[out]*/ double * Y, /*[out]*/ double * Z);
	void Vertex(/*[in]*/ long VtxIndex, /*[out]*/ double * X, /*[out]*/ double * Y, /*[out]*/ double * Z);
	void Triangle(/*[in]*/ long TriIndex, /*[out]*/ long * vtx1Index, /*[out]*/ long * vtx2Index, /*[out]*/ long * vtx3Index);
	void get_Key(/*[out, retval]*/ BSTR *pVal);
	void put_Key(/*[in]*/ BSTR newVal);
	void get_GlobalCallback(/*[out, retval]*/ ICallback * *pVal);
	void put_GlobalCallback(/*[in]*/ ICallback * newVal);
	void get_CdlgFilter(/*[out, retval]*/ BSTR *pVal);
	void get_ErrorMsg(/*[in]*/ long ErrorCode, /*[out, retval]*/ BSTR *pVal);
	void get_LastErrorCode(/*[out, retval]*/ long *pVal);
	void get_NumVertices(/*[out, retval]*/ long *pVal);
	void get_NumTriangles(/*[out, retval]*/ long *pVal);
	void Select(/*[in, out]*/ long * TriangleHint, /*[in]*/ double X, /*[in]*/ double Y, /*[out]*/ double * Z, /*[out, retval]*/ VARIANT_BOOL * retval);
	void Close(/*[out, retval]*/ VARIANT_BOOL * retval);
	void Save(/*[in]*/ BSTR TinFilename, /*[in, optional]*/ ICallback * cBack, /*[out, retval]*/ VARIANT_BOOL * retval);
	void CreateNew(/*[in]*/ CGrid * Grid, /*[in]*/ double Deviation, /*[in]*/ SplitMethod SplitTest, /*[in]*/ double STParam, /*[in]*/ long MeshDivisions, /*[in, optional, defaultvalue(1073741824)]*/ long MaximumTriangles, /*[in, optional]*/ ICallback * cBack, /*[out, retval]*/ VARIANT_BOOL * retval);
	void Open(/*[in]*/ BSTR TinFile, /*[in, optional]*/ ICallback * cBack, /*[out, retval]*/ VARIANT_BOOL * retval);
	void CreateTinFromPoints(SAFEARRAY * Points, VARIANT_BOOL* retval);

private:
	ICallback * _globalCallback;
	long _lastErrorCode;
	BSTR _key;
	BSTR _filename;
	
	triangleTable _triTable;
	vertexTable _vtxTable;
	vertex _min;
	vertex _max;

	TinHeap _devHeap;
	SplitMethod _splitMethod;
	double _splitParam;
	CGrid * _grid;
	CGridHeader * _gridHeader;
	char * _dTriangles;

private:
	inline long gridNcols();
	inline long gridNrows();
	inline void createTin( double deviation, long meshDivisions, long maxTriangles );
	inline void computeTriangleEquation( long triIndex, double & A, double & B, double & C, double & D );
	inline bool vtxInTriangle( vertex * triangle, vertex testVertex );
	inline bool doesCross( int SH, int NSH, vertex & corner_one, vertex & corner_two );
	inline void setSign( double val, int & SH );
	inline vertex gridCoord( long column, long row );
	inline double gridValue( long column, long row );
	inline double gridMin();
	inline double gridMax();
	void createMesh( long numDivisions, vertex v1, vertex v2, vertex v3, vertex v4 );
	inline void splitTriangle( long triIndex );
	inline void gridRaster( double x, double y, long & column, long & row );
	inline void computeMaxDev( long triIndex );
	inline bool splitPerp( long triIndex, long perpPointIndex, PerpSplitMethod perpSplitMethod );
	inline bool splitQuad( long triOneIndex, long triTwoIndex );
	inline void changeBorder( long triIndex, long oldValue, long newValue );
	/*inline double MIN( double one, double two );
	inline double MAX( double one, double two );*/
	bool willCreateBadTriangles( vertex * triangle, vertex testVertex );
	inline long indexAfterClockwise( long triIndex, long vtxIndex );
	inline void setBorders( long triIndex, long possBorder1, long possBorder2, long possBorder3,
							long possBorder4, long possBorder5, long possBorder6, long possBorder7 );
	inline long unsharedIndex( long triOne, long triTwo );
	inline void sharedIndexes( long triOne, long triTwo, long & index1, long & index2 );
	bool canSplitQuad( long p1_unshared, long p2_unshared, long p1_shared, long p2_shared );
	inline double inscribedCircleRad( vertex one, vertex two, vertex three );
	inline bool hasVertices( long triIndex, long vtxOne, long vtxTwo );

	void Prune(int &nv,XYZ v[]);
	void BuildTin(int nv, XYZ v[],int ntri,ITRIANGLE *tri,XYZ vMax,XYZ vMin);
	void FindAllBorders(int ntri,ITRIANGLE *tri);
	int FindBorder(int ntri,int curIndex,ITRIANGLE *tri,int v1,int v2);
	void FindMaxXYZAndMinXYZ(int nv,XYZ v[],XYZ &vMax,XYZ &vMin);
	int CreateTin(int nv,XYZ *pxyz);
	int Triangulate(int nv,XYZ *pxyz,ITRIANGLE *v,int *ntri);
	int CircumCircle(double xp,double yp,
		double x1,double y1,double x2,double y2,double x3,double y3,
		double *xc,double *yc,double *r);
};
