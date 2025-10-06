#include "stdafx.h"
#include "Vector.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CVector
void  CVector::get_i(double *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*pVal = _v.geti();
	return ;
}

void  CVector::put_i(double newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	_v.seti( newVal );
	return ;
}

void  CVector::get_j(double *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = _v.getj();
	return ;
}

void  CVector::put_j(double newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	_v.setj( newVal );
	return ;
}

void  CVector::get_k(double *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = _v.getk();
	return ;
}

void  CVector::put_k(double newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	_v.setk( newVal );
	return ;
}

void  CVector::Normalize()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	_v.Normalize();
	return ;
}

void  CVector::Dot(CVector *V, double *result)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	double i = 0, j = 0, k = 0;
	V->get_i( &i );
	V->get_j( &j );
	V->get_k( &k );
	*result = _v.dot( cppVector( i, j, k ) );
	return ;
}

void  CVector::CrossProduct(CVector *V, CVector **result)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CVector * normal = new CVector();
	
	double i = 0, j = 0, k = 0;
	V->get_i( &i );
	V->get_j( &j );
	V->get_k( &k );
	cppVector cp = _v.crossProduct( cppVector( i, j, k ) );

	normal->put_i( cp.geti() );
	normal->put_j( cp.getj() );
	normal->put_k( cp.getk() );

	*result = normal;

	return ;
}



void  CVector::get_LastErrorCode(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = _lastErrorCode;
	_lastErrorCode = tkNO_ERROR;

	return ;
}

void  CVector::get_ErrorMsg(long ErrorCode, BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = A2BSTR(ErrorMsg(ErrorCode));

	return ;
}

void  CVector::get_GlobalCallback(ICallback **pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*pVal = _globalCallback;
	//if( _globalCallback != NULL )
	//{	
	//	_globalCallback->AddRef();
	//}
	return ;
}

void  CVector::put_GlobalCallback(ICallback *newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	_globalCallback = newVal;
	return ;
}

void  CVector::get_Key(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	USES_CONVERSION;

	*pVal = OLE2BSTR(_key);
	return ;
}

void  CVector::put_Key(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	USES_CONVERSION;

	::SysFreeString(_key);
	_key = OLE2BSTR(newVal);

	return ;
}

