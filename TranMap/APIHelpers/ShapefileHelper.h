#pragma once
#include "Shapefile.h"
class ShapefileHelper
{
public:
	static long GetMWShapeIdIndex(CShapefile* sf);
	static void CopyAttributes(CShapefile* sf, long sourceIndex, long targetIndex, long skipFieldIndex);
	static void CloneNoFields(CShapefile* sfSource, CShapefile** retVal, ShpfileType shpType, bool addShapeId = false);
	static void CloneNoFields(CShapefile* sf, CShapefile** retVal, bool addShapeId = false);
	static void CloneCore(CShapefile* sfSource, CShapefile** retVal, ShpfileType shpType, bool addShapeId = false);
	static bool GetSelectedExtents(CShapefile* sf, double& xMinRef, double& yMinRef, double& xMaxRef, double& yMaxRef);
	static void CopyFields(CShapefile* source, CShapefile* target);
	static vector<int>* GetSelectedIndices(CShapefile* sf);
	static long GetNumShapes(CShapefile* sf);
	static long GetNumFields(CShapefile* sf);
	static long GetNumSelected(CShapefile* sf);
	static void Rotate(CShapefile* sf, double originX, double originY, double angleDegree);
	static ShpfileType GetShapeType2D(CShapefile* sf);
	static ShpfileType GetShapeType(CShapefile* sf);
	static CShapefile* CloneSelection(CShapefile* sf);
	static bool ShapeSelected(CShapefile* sf, int shapeIndex);
	static bool GetClosestPoint(CShapefile* sf, double x, double y, double maxDistance, std::vector<long>& ids, long* shapeIndex, long* pointIndex, double& dist);
	static bool PointInPolygon(CShapefile* sf, long ShapeIndex, double x, double y);
	static bool BoundsWithinPolygon(CShapefile* sf, int shapeIndex, double b_minX, double b_minY, double b_maxX, double b_maxY);
	static bool ShapeTypeIsM(CShapefile* sf);
	static CStringW GetSymbologyFilename(CShapefile* sf);
	static bool InteractiveEditing(CShapefile* sf);
	static bool IsVolatile(CShapefile* sf);
	static void InsertMwShapeIdField(CShapefile* sf);
	static tkShapefileSourceType GetSourceType(CShapefile* sf);
};

