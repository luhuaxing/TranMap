#pragma once
#include "Shape.h"

class ShapeHelper
{
public:

	static BOOL get_MemShapePoints(CShape* shape, ShpfileType & ShapeType, std::vector<long> & parts, std::vector<double> & xPts, std::vector<double> & yPts);
	static bool PointWithinShape(CShape* shape, double projX, double projY, double Tolerance);
	static bool PointInThisPoly(CShape* shp, double x, double y);
	static bool BoundPointsInPolygon(CShape* shp, double b_minX, double b_minY, double b_maxX, double b_maxY);
	static bool ForceProperShapeType(CShape* shp, ShpfileType sfType);
	static ShpfileType GetShapeType(CShape* shp);
	static ShpfileType GetShapeType2D(CShape* shp);
	static bool SplitByPolyline(CShape* subject, CShape* polyline, vector<CShape*>& shapes);
	static bool IsEmpty(CShape* shp);
	static long GetNumParts(CShape* shp);
	static int GetLargestPart(CShape* shp);
	static void AddLabelToShape(CShape* shp, CLabels* labels, BSTR text, tkLabelPositioning method, tkLineLabelOrientation orientation);
};

