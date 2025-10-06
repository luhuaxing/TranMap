
#include "stdafx.h"
#include "ChartField.h"

// **********************************************************
//		get/put_Index
// **********************************************************
void CChartField::get_Index(long* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = _index;
	return ;
}
void CChartField::put_Index(long newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	_index = newVal;
	return ;
}

// **********************************************************
//		get/put_Color
// **********************************************************
void CChartField::get_Color(OLE_COLOR* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = _color;
	return ;
}
void CChartField::put_Color(OLE_COLOR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	_color = newVal;
	return ;
}

// **********************************************************
//		get/put_Name
// **********************************************************
void CChartField::get_Name(BSTR* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	*retVal = OLE2BSTR(_name);
	return ;
}
void CChartField::put_Name(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	::SysFreeString(_name);
	USES_CONVERSION;
	_name = OLE2BSTR(newVal);
	return ;
}
