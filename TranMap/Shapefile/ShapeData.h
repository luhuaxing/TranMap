#pragma once
#include "ShapeInterfaces.h"

// CShapeData declaration
class CShapeData: public IShapeData
{
public:
	// Constructors
	CShapeData(char* shpData)
	{
		_shapeType = SHP_NULLSHAPE;
		_lastErrorCode = tkNO_ERROR;
		_bounds = NULL;
		_points = NULL;
		_parts = NULL;
		put_ShapeData(shpData);
	}

	// Destructor
	~CShapeData()
	{
		this->Clear();
	}
	
private:	
	ShpfileType _shapeType;	// actually 2D type will be used, as Z, M values aren't supported here
	int _lastErrorCode;
	// size
	int _pointCount;
	int _partCount;
	// data
	double* _bounds;	// we don't need bounds for points; so memory will be allocated dynamically
	double* _points;
	int* _parts;
	// functions
	ShpfileType ShapeTypeConvert2D(ShpfileType type);
	void Clear();
public:
	ShpfileType get_shapeType()
	{
		return _shapeType;
	}
	int get_PointCount()
	{	
		return _pointCount;
	}
	int get_PartCount()
	{
		return _partCount;
	}
	int get_LastErrorCode()
	{
		int code = _lastErrorCode;
		_lastErrorCode = tkNO_ERROR;
		return code;
	}

	bool get_BoundsXY(double& xMin, double& xMax, double& yMin, double& yMax);

	// data
	bool put_ShapeData(char* shapeData);

	// parts
	int get_PartStartPoint(int PartIndex);
	int get_PartEndPoint(int PartIndex);

	// points
	double* get_PointsXY();
	void get_XYFast(int PointIndex, double& x, double& y);
	bool get_PointXY(int PointIndex, double& x, double& y);
	
	// utility
	bool PointInRing(int partIndex, double pointX, double pointY);
};

