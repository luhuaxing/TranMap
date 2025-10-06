#pragma once
class SelectionHelper
{
public:
	static bool PolylineIntersection(std::vector<double>& xPts, std::vector<double>& yPts, std::vector<long>& parts, double& b_minX, double& b_maxX, double& b_minY, double& b_maxY, double& Tolerance);
	static bool PolygonIntersection(std::vector<double>& xPts, std::vector<double>& yPts, std::vector<long>& parts,
		double& b_minX, double& b_maxX, double& b_minY, double& b_maxY, double& Tolerance);
	static bool SelectWithShapeBounds(CShapefile* sf, CShape* shp, vector<long>& indices);
	static int SelectByPolygon(CShapefile* sf, CShape* polygon, int& errorCode);
	static bool SelectByPoint(CShapefile* sf, Extent& box, bool clearPrevious);
	static bool SelectByRectangle(CShapefile* sf, Extent& box);
	static bool SelectSingleShape(CShapefile* sf, Extent& box, long& shapeIndex);
	static bool SelectSingleShape(CShapefile* sf, Extent& box, SelectMode mode, long& shapeIndex);
private:
	static bool SelectShapes(CShapefile* sf, Extent& extents, double Tolerance, SelectMode SelectMode, std::vector<long>& selectResult);
	
};

