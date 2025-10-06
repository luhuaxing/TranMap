#include "stdafx.h"
#include "PointClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void  CPointClass::get_X(double *pVal)
{
	*pVal = _x;
	return ;
}

void  CPointClass::put_X(double newVal)
{
	_x = newVal;
	return ;
}

void  CPointClass::get_Y(double *pVal)
{
	*pVal = _y;
	return ;
}

void  CPointClass::put_Y(double newVal)
{
	_y = newVal;
	return ;
}

void  CPointClass::get_Z(double *pVal)
{
	*pVal = _z;
	return ;
}

void  CPointClass::put_Z(double newVal)
{
	_z = newVal;
	return ;
}

void  CPointClass::get_LastErrorCode(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*pVal = tkNO_ERROR;
	return ;
}

void  CPointClass::get_ErrorMsg(long ErrorCode, BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	*pVal = A2BSTR(ErrorMsg(ErrorCode));
	return ;
}

void  CPointClass::get_GlobalCallback(ICallback **pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*pVal = NULL;
	return ;
}

void  CPointClass::put_GlobalCallback(ICallback *newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	// do nothing
	return ;
}

void  CPointClass::get_Key(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*pVal = SysAllocString(L"");
	return ;
}

void  CPointClass::put_Key(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	// do nothing
	return ;
}

void  CPointClass::get_M(double *pVal)
{
	*pVal = _m;
	return ;
}

void  CPointClass::put_M(double newVal)
{
	_m = newVal;
	return ;
}

// ************************************************************
//		Clone()
// ************************************************************
void  CPointClass::Clone(CPointClass** retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = new CPointClass();
	(*retVal)->put_X(_x);
	(*retVal)->put_Y(_y);
	(*retVal)->put_Z(_z);
	(*retVal)->put_M(_m);
	return ;
}
