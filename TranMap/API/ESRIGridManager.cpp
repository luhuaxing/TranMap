#include "stdafx.h"
#include "ESRIGridManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CESRIGridManager
void  CESRIGridManager::get_LastErrorCode(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = tkNO_ERROR;

	return ;
}

void  CESRIGridManager::get_ErrorMsg(long ErrorCode, BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	USES_CONVERSION;
	*pVal = A2BSTR(ErrorMsg(ErrorCode));

	return ;
}

void  CESRIGridManager::get_GlobalCallback(ICallback **pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = _globalCallback;

	return ;
}

void  CESRIGridManager::put_GlobalCallback(ICallback *newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
		_globalCallback = newVal;
	return ;
}

void  CESRIGridManager::CanUseESRIGrids(VARIANT_BOOL *retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (_gm.canUseEsriGrids())
		*retval = VARIANT_TRUE;
	else
        *retval = VARIANT_FALSE;

	return ;
}

void  CESRIGridManager::DeleteESRIGrids(BSTR Filename, VARIANT_BOOL *retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	USES_CONVERSION;
	
	if (_gm.deleteGrid(OLE2CA(Filename),ESRI_GRID))
        *retval = VARIANT_TRUE;
	else
		*retval = VARIANT_FALSE;

	return ;
}

void  CESRIGridManager::IsESRIGrid(BSTR Filename, VARIANT_BOOL *retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	USES_CONVERSION;
	if( _gm.getGridType(OLE2CA(Filename)) == ESRI_GRID ) 
		*retval = VARIANT_TRUE;
	else
		*retval = VARIANT_FALSE;

	return ;
}
