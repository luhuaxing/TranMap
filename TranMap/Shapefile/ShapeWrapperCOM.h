#pragma once
#include "ShapeInterfaces.h"

class CShapeWrapperCOM : public IShapeWrapper //, public IShapeData
{
public:
	//	Constructor
	CShapeWrapperCOM(ShpfileType shpType)
	{
		_ShapeType = shpType;
		_ShapeType2D = ShapeTypeConvert2D(_ShapeType);
		_boundsChanged = true;
		_lastErrorCode = tkNO_ERROR;
		_xMin = _yMin = _xMax = _yMax = _zMin = _mMin = _zMax = _mMax = 0.0;
	}

	//	Destructor
	~CShapeWrapperCOM()
	{
		this->Clear();
	}
	
private:	
	// type
	ShpfileType _ShapeType;
	ShpfileType _ShapeType2D;
	// bounds
	double _xMin;
	double _yMin;
	double _xMax;
	double _yMax;

	double _zMin;
	double _mMin;
	double _zMax;
	double _mMax;
	bool _boundsChanged;
	int _lastErrorCode;
	
public:
	// Actually this breaks encapsulation, but in many cases we want faster access
	std::deque<CPointClass *> _allPoints;
	std::deque<long> _allParts;

public:
	int get_PointCount(){return _allPoints.size();}
	int get_PartCount(){return _allParts.size();}

	// type
	ShpfileType get_ShapeType(){return _ShapeType;}
	ShpfileType get_ShapeType2D(){return _ShapeType2D;}
	bool put_ShapeType(ShpfileType shpType);
	
	// bounds
	void RefreshBounds();
	void RefreshBoundsXY();
	bool get_BoundsXY(double& xMin, double& xMax, double& yMin, double& yMax);
	bool get_Bounds(double& xMin, double& xMax, double& yMin, double& yMax, 
					double& zMin, double& zMax, double& mMin, double& mMax);
	
	void get_XYFast(int PointIndex, double& x, double& y);
	bool get_PointXY(int PointIndex, double& x, double& y);
	bool put_PointXY(int PointIndex, double x, double y);
	bool get_PointZ(int PointIndex, double& z);
	bool get_PointM(int PointIndex, double& m);
	bool put_PointZ(int PointIndex, double z);
	bool put_PointM(int PointIndex, double m);
	
	// COM points
	CPointClass* get_Point(long Index);
	bool put_Point(long Index, CPointClass* pnt);

	// changing size
	void Clear();
	bool InsertPoint(int PointIndex, CPointClass* pnt);
	bool InsertPointXY(int PointIndex, double x, double y);
	bool InsertPointXYZM(int PointIndex, double x, double y, double z, double m);
	bool DeletePoint(int Pointindex);
	
	// parts
	bool InsertPart(int PartIndex, int PointIndex);
	bool DeletePart(int PartIndex);
	int get_PartStartPoint(int PartIndex);
	int get_PartEndPoint(int PartIndex);
	bool put_PartStartPoint(long PartIndex, long newVal);
	
	bool PointInRing(int partIndex, double pointX, double pointY);

	int get_LastErrorCode()
	{
		int code = _lastErrorCode;
		_lastErrorCode = tkNO_ERROR;
		return code;
	}
	
	static ShpfileType ShapeTypeConvert2D(ShpfileType type);
private:
};