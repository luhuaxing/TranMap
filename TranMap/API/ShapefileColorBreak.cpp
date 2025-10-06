 
#include "stdafx.h"
#include "ShapefileColorBreak.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef OLD_API
/////////////////////////////////////////////////////////////////////////////
// CShapefileColorBreak

void  CShapefileColorBreak::get_StartValue(VARIANT *pVal)
{
	VariantClear(pVal);
	VariantInit(pVal);
	VariantCopy(pVal,&_startValue);
	return ;
}

void  CShapefileColorBreak::put_StartValue(VARIANT newVal)
{
	VariantClear(&_startValue);	
	VariantInit(&_startValue);	
	VariantCopy(&_startValue,&newVal);
	return ;
}

void  CShapefileColorBreak::get_EndValue(VARIANT *pVal)
{
	VariantClear(pVal);
	VariantInit(pVal);
	VariantCopy(pVal,&_endValue);
	return ;
}

void  CShapefileColorBreak::put_EndValue(VARIANT newVal)
{
	VariantClear(&_endValue);
	VariantInit(&_endValue);	
	VariantCopy(&_endValue,&newVal);
	return ;
}

void  CShapefileColorBreak::get_StartColor(OLE_COLOR *pVal)
{
	*pVal = _startColor;
	return ;
}

void  CShapefileColorBreak::put_StartColor(OLE_COLOR newVal)
{
	_startColor = newVal;
	return ;
}

void  CShapefileColorBreak::get_EndColor(OLE_COLOR *pVal)
{
	*pVal = _endColor;
	return ;
}

void  CShapefileColorBreak::put_EndColor(OLE_COLOR newVal)
{
	_endColor = newVal;
	return ;
}

void  CShapefileColorBreak::get_Visible(VARIANT_BOOL *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	*pVal = _visible;

	return ;
}

void  CShapefileColorBreak::put_Visible(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	_visible = newVal;

	return ;
}


void  CShapefileColorBreak::get_Caption(BSTR* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	*pVal = _caption.AllocSysString();

	return ;
}

void  CShapefileColorBreak::put_Caption(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	_caption = newVal;

	return ;
}

#endif