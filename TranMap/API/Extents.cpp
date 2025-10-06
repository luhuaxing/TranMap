#include "stdafx.h"
#include "Extents.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CExtents
void  CExtents::SetBounds(double xMin, double yMin, double zMin, double xMax, double yMax, double zMax)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	_xmin = xMin;
	_max = xMax;
	_ymin = yMin;
	_ymax = yMax;
	_zmin = zMin;
	_zmax = zMax;

	double tmp;
	if( _xmin > _max )
	{	tmp = _max;
		_max = _xmin;
		_xmin = tmp;
	}
	if( _ymin > _ymax )
	{	tmp = _ymax;
		_ymax = _ymin;
		_ymin = tmp;
	}
	if( _zmin > _zmax )
	{	tmp = _zmax;
	_zmax = _zmin;
		_zmin = tmp;
	}

	return ;
}

void  CExtents::GetBounds(double * xMin, double * yMin, double * zMin, double * xMax, double * yMax, double * zMax)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*xMin = _xmin;
	*xMax = _max;
	*yMin = _ymin;
	*yMax = _ymax;
	*zMin = _zmin;
	*zMax = _zmax;

	return ;
}


void  CExtents::get_xMin(double *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*pVal = _xmin;
	return ;
}

void  CExtents::get_xMax(double *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*pVal = _max;
	return ;
}

void  CExtents::get_yMin(double *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = _ymin;
	return ;
}

void  CExtents::get_yMax(double *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = _ymax;
	return ;
}

void  CExtents::get_zMin(double *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = _zmin;
	return ;
}

void  CExtents::get_zMax(double *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = _zmax;
	return ;
}

void  CExtents::get_mMin(double *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = _mmin;

	return ;
}

void  CExtents::get_mMax(double *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = _mmax;

	return ;
}

void  CExtents::GetMeasureBounds(double *mMin, double *mMax)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*mMin = _mmin;
	*mMax = _mmax;

	return ;
}

void  CExtents::SetMeasureBounds(double mMin, double mMax)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	_mmin = mMin;
	_mmax = mMax;

	double tmp;
	if( _mmin > _mmax )
	{	tmp = _mmax;
		_mmax = _mmin;
		_mmin = tmp;
	}

	return ;
}

//*********************************************************************
//*	 Extents2Shape()				              
//*********************************************************************
// Creates polygon shape which is equivalent to the input extents
// return shape pointer on success, or NULL otherwise
 
void  CExtents::ToShape(CShape** retVal)
{
	CShape* shp = new CShape();
	long PartIndex = 0;
	VARIANT_BOOL vbretval;
	
	shp->put_ShapeType(SHP_POLYGON);
	shp->InsertPart(0, &PartIndex, &vbretval);
	
	CPointClass* pnt = NULL;
	
	for (long i = 0; i<=4; i++)
	{
		pnt = new CPointClass();

		if (i == 0 || i ==4)
		{	
			pnt->put_X(this->_xmin);
			pnt->put_Y(this->_ymin);
		}
		else if (i ==1)
		{
			pnt->put_X(this->_xmin);
			pnt->put_Y(this->_ymax);
			
		}
		else if (i ==2)
		{
			pnt->put_X(this->_max);
			pnt->put_Y(this->_ymax);
		}
		else if (i ==3)
		{
			pnt->put_X(this->_max);
			pnt->put_Y(this->_ymin);
		}

		shp->InsertPoint(pnt, &i, &vbretval);
		delete pnt;
	}

	*retVal = shp;
	return ;
}

void  CExtents::Disjoint(CExtents* ext, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = VARIANT_TRUE;
	if (ext) {
		double xMin, yMin, zMin, xMax, yMax, zMax;
		ext->GetBounds(&xMin, &yMin, &zMin, &xMax, &yMax, &zMax);
		*retVal = (this->_max < xMin || this->_xmin > xMax || this->_ymax < yMax || this->_ymin > yMax);
	}
	return ;
}

void  CExtents::Union(CExtents* ext)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if (ext) {
		double xMin, yMin, zMin, xMax, yMax, zMax;
		ext->GetBounds(&xMin, &yMin, &zMin, &xMax, &yMax, &zMax);
		this->_xmin = MIN(xMin, this->_xmin);
		this->_ymin = MIN(yMin, this->_ymin);
		this->_zmin = MIN(zMin, this->_zmin);
		this->_max = MAX(xMax, this->_max);
		this->_ymax = MAX(yMax, this->_ymax);
		this->_zmax = MAX(zMax, this->_zmax);
	}
	return ;
}

void  CExtents::GetIntersection(CExtents* ext, CExtents** retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = NULL;
	if (ext) {
		double xMin, yMin, zMin, xMax, yMax, zMax;
		ext->GetBounds(&xMin, &yMin, &zMin, &xMax, &yMax, &zMax);
		xMin = MAX(xMin, this->_xmin);
		yMin = MAX(yMin, this->_ymin);
		zMin = MAX(zMin, this->_zmin);
		xMax = MIN(xMax, this->_max);
		yMax = MIN(yMax, this->_ymax);
		zMax = MIN(zMax, this->_zmax);
		
		if (xMin <= xMax && yMin <= yMax && zMin <= zMax)
		{
			*retVal= new CExtents();
			(*retVal)->SetBounds(xMin, yMin, zMin, xMax, yMax, zMax);
		}
	}
	return ;
}

void  CExtents::Intersects(CExtents* ext, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = VARIANT_FALSE;
	if (ext) {
		double xMin, yMin, zMin, xMax, yMax, zMax;
		ext->GetBounds(&xMin, &yMin, &zMin, &xMax, &yMax, &zMax);
		xMin = MAX(xMin, this->_xmin);
		yMin = MAX(yMin, this->_ymin);
		zMin = MAX(zMin, this->_zmin);
		xMax = MIN(xMax, this->_max);
		yMax = MIN(yMax, this->_ymax);
		zMax = MIN(zMax, this->_zmax);
		*retVal = (xMin <= xMax && yMin <= yMax && zMin <= zMax) ? VARIANT_TRUE : VARIANT_FALSE;
	}
	return ;
}

// ******************************************************
//		ToDebugString()
// ******************************************************
void  CExtents::ToDebugString(BSTR* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	CString s;
	s.Format("xMin=%f; xMax=%f; yMin=%f; yMax=%f", _xmin, _max, _ymin, _ymax);
	USES_CONVERSION;
	*retVal = A2BSTR(s);
	return ;
}

// ******************************************************
//		PointIsWithin()
// ******************************************************
void  CExtents::PointIsWithin(double x, double y, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal =  x >= _xmin && x <= _max && y >= _ymin && y <= _ymax ? VARIANT_TRUE: VARIANT_FALSE;
	return ;
}

// ******************************************************
//		GetCenter()
// ******************************************************
void  CExtents::get_Center(CPointClass** retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = new CPointClass();
	(*retVal)->put_X((_xmin + _max) / 2);
	(*retVal)->put_Y((_ymin + _ymax) / 2);
	return ;
}

// ******************************************************
//		MoveTo()
// ******************************************************
void  CExtents::MoveTo(double x, double y)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	double dx = _max - _xmin;
	double dy = _ymax - _ymin;
	_xmin = x - dx / 2.0;
	_max = x + dx / 2.0;
	_ymin = y - dy / 2.0;
	_ymax = y + dy / 2.0;
	return ;
}


