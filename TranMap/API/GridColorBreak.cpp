#include "stdafx.h"
#include "GridColorBreak.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CGridColorBreak
void  CGridColorBreak::get_HighColor(OLE_COLOR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	*pVal = _highColor;

	return ;
}

void  CGridColorBreak::put_HighColor(OLE_COLOR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	_highColor = newVal;

	return ;
}

void  CGridColorBreak::get_LowColor(OLE_COLOR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	
	*pVal = _lowColor;

	return ;
}

void  CGridColorBreak::put_LowColor(OLE_COLOR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	
	_lowColor = newVal;

	return ;
}

void  CGridColorBreak::get_HighValue(double *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	
	*pVal = _highValue;

	return ;
}

void  CGridColorBreak::put_HighValue(double newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	
	_highValue = newVal;

	return ;
}

void  CGridColorBreak::get_LowValue(double *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	
	*pVal = _lowValue;

	return ;
}

void  CGridColorBreak::put_LowValue(double newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	
	_lowValue = newVal;

	return ;
}

void  CGridColorBreak::get_ColoringType(ColoringType *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	
	*pVal = _colorType;

	return ;
}

void  CGridColorBreak::put_ColoringType(ColoringType newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	
	_colorType = newVal;

	return ;
}

void  CGridColorBreak::get_GradientModel(GradientModel *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	
	*pVal = _gradModel;

	return ;
}

void  CGridColorBreak::put_GradientModel(GradientModel newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	
	_gradModel = newVal;

	return ;
}


void  CGridColorBreak::get_LastErrorCode(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = _lastErrorCode;
	_lastErrorCode = tkNO_ERROR;

	return ;
}

void  CGridColorBreak::get_ErrorMsg(long ErrorCode, BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = A2BSTR(ErrorMsg(ErrorCode));

	return ;
}

void  CGridColorBreak::get_GlobalCallback(ICallback **pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = _globalCallback;
	return ;
}

void  CGridColorBreak::put_GlobalCallback(ICallback *newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	_globalCallback = newVal;
	return ;
}

void  CGridColorBreak::get_Key(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	USES_CONVERSION;

	*pVal = OLE2BSTR(_key);

	return ;
}

void  CGridColorBreak::put_Key(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	USES_CONVERSION;

	::SysFreeString(_key);
	_key = OLE2BSTR(newVal);

	return ;
}


void  CGridColorBreak::get_Caption(BSTR* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	*pVal = _caption.AllocSysString();

	return ;
}

void  CGridColorBreak::put_Caption(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	_caption = newVal;

	return ;
}
