
#include "stdafx.h"
#include "ShapePoint.h"
#include "PointClass.h"

#pragma region CShapePoint

// *******************************************************
//		get_Point()
// *******************************************************
CPointClass* CShapePoint::get_Point()
{
	CPointClass* pnt = new CPointClass();
	return pnt;
}

// *******************************************************
//		put_Point()
// *******************************************************
bool CShapePoint::put_Point(CPointClass* newPoint)
{
	if (!newPoint)
	{
		return false;
	}
	else
	{
		newPoint->get_X(&x);
		newPoint->get_Y(&y);
		newPoint->get_Z(&z);
		newPoint->get_M(&m);
		return true;
	}
}

// *******************************************************
//		get_XY()
// *******************************************************
void CShapePoint::get_XY(double& X, double& Y)
{
	X = x;
	Y = y;
}

// *******************************************************
//		put_XY()
// *******************************************************
void CShapePoint::put_XY(double X, double Y)
{
	x = X;
	y = Y;
}
#pragma endregion

#pragma region ShapePointCOM

// *******************************************************
//		get_Point()
// *******************************************************
CPointClass* CShapePointCOM::get_Point()
{
	//if (pnt)
	//	pnt->AddRef();
	return pnt;
}

// *******************************************************
//		put_Point()
// *******************************************************
bool CShapePointCOM::put_Point(CPointClass* newPoint)
{
	if (newPoint)
	{
		if (pnt)
		{
			//pnt->Release();
			delete pnt;
			pnt = NULL;
		}
		pnt = newPoint;
		//pnt->AddRef();
		return true;
	}
	else
	{
		return false;
	}
}

// *******************************************************
//		get_XY()
// *******************************************************
void CShapePointCOM::get_XY(double& x, double& y)
{
	pnt->get_X(&x);
	pnt->get_Y(&y);
}

// *******************************************************
//		put_XY()
// *******************************************************
void CShapePointCOM::put_XY(double x, double y)
{
	pnt->put_X(x);
	pnt->put_Y(y);
}

// *******************************************************
//		get_Key()
// *******************************************************
void CShapePointCOM::get_Key(BSTR* pVal)
{
	pnt->get_Key(pVal);
}
void CShapePointCOM::put_Key(BSTR newVal)
{
	pnt->put_Key(newVal);
}

// *******************************************************
//		get_X()
// *******************************************************
double CShapePointCOM::get_X()
{
	double x;
	 pnt->get_X(&x);
	 return x;
}
double CShapePointCOM::get_Y()
{
	double y;
	pnt->get_Y(&y);
	return y;
}
double CShapePointCOM::get_Z()
{
	double z;
	pnt->get_Z(&z);
	return z;
}
double CShapePointCOM::get_M()
{
	double m;
	pnt->get_M(&m);
	return m;
}
#pragma endregion