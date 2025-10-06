#pragma once
#include "PointClass.h"
#include "Shape.h"

// CExtents
class  CExtents
{
public:
	CExtents()
	{
		_xmin = 0.0;
		_max = 0.0;
		_ymin = 0.0;
		_ymax = 0.0;
		_zmin = 0.0;
		_zmax = 0.0;
		_mmin = 0.0;
		_mmax = 0.0;
	}


// CExtents
public:
	void SetMeasureBounds(/*[in]*/double mMin, /*[in]*/double mMax);
	void GetMeasureBounds(/*[out]*/ double * mMin, /*[out]*/ double * mMax);
	void get_mMax(/*[out, retval]*/ double *pVal);
	void get_mMin(/*[out, retval]*/ double *pVal);
	void get_zMax(/*[out, retval]*/ double *pVal);
	void get_zMin(/*[out, retval]*/ double *pVal);
	void get_yMax(/*[out, retval]*/ double *pVal);
	void get_yMin(/*[out, retval]*/ double *pVal);
	void get_xMax(/*[out, retval]*/ double *pVal);
	void get_xMin(/*[out, retval]*/ double *pVal);
	void SetBounds(/*[in]*/ double xMin, /*[in]*/ double yMin, /*[in]*/ double zMin, /*[in]*/ double xMax, /*[in]*/ double yMax, /*[in]*/ double zMax);
	void GetBounds(/*[out]*/ double * xMout, /*[out]*/ double * yMout, /*[out]*/ double * zMout, /*[out]*/ double * xMax, /*[out]*/ double * yMax, /*[out]*/ double * zMax);
	void ToShape(CShape** retVal);
	void Disjoint(CExtents* ext, VARIANT_BOOL* retVal);
	void GetIntersection(CExtents* ext, CExtents** retVal);
	void Union(CExtents* ext);
	void Intersects(CExtents* ext, VARIANT_BOOL* retVal);
	void ToDebugString(BSTR* retVal);
	void PointIsWithin(double x, double y, VARIANT_BOOL* retVal);
	void get_Center(CPointClass** retVal);
	void MoveTo(double x, double y);
	
private:
	double _xmin;
	double _max;
	double _ymin;
	double _ymax;
	double _zmin;
	double _zmax;
	double _mmin;
	double _mmax;
};


