#include "stdafx.h"
#include "ShapefileCategory.h"
#include "Expression.h"
#include "Shapefile.h"
#include "Table.h"

// *****************************************************************
//		get/put_Name
// *****************************************************************
void  CShapefileCategory::get_Name(BSTR* retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	*retval = OLE2BSTR(_name);
	return ;
}
void  CShapefileCategory::put_Name(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	::SysFreeString(_name);
	_name = OLE2BSTR(newVal);
	return ;
}

// *****************************************************************
//		get/put_Expression
// *****************************************************************
void  CShapefileCategory::get_Expression(BSTR* retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	*retval = OLE2BSTR(_expression);
	return ;
}
void  CShapefileCategory::put_Expression(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	::SysFreeString(_expression);
	_expression = OLE2BSTR(newVal);
	_categoryValue = cvExpression;
	return ;
}

// ***************************************************************
//		get/put_DrawingOptions()
// ***************************************************************
void  CShapefileCategory::get_DrawingOptions(CShapeDrawingOptions** retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retval = _drawingOptions;
	return ;
}
void  CShapefileCategory::put_DrawingOptions(CShapeDrawingOptions* newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if (!newVal)
	{
		// TODO: report error
	}
	else
	{
		_drawingOptions = newVal;
	}
	return ;
}

// ***************************************************************
//		get/put_underlyingOptions()
// ***************************************************************
CDrawingOptionsEx* CShapefileCategory::get_UnderlyingOptions()
{
	if(_drawingOptions != NULL)
	{
		return ((CShapeDrawingOptions*)_drawingOptions)->get_UnderlyingOptions();
	}
	else
		return NULL;
}
void CShapefileCategory::put_underlyingOptions(CDrawingOptionsEx* newVal)
{
	if (_drawingOptions == NULL)
	{
		// this should not happen, but we still add it to be sure
		_drawingOptions = new CShapeDrawingOptions();
	}
	((CShapeDrawingOptions*)_drawingOptions)->put_underlyingOptions(newVal);
}

// ***************************************************************
//		ValueType()
// ***************************************************************
void  CShapefileCategory::get_ValueType(tkCategoryValue* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _categoryValue;
	return ;
}
void  CShapefileCategory::put_ValueType(tkCategoryValue newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_categoryValue = newVal;
	return ;
}

// ***************************************************************
//		MinValue()
// ***************************************************************
void  CShapefileCategory::get_MinValue(VARIANT* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (_minValue.vt == VT_EMPTY)
		VariantInit(pVal);
	else
		VariantCopy(pVal, &_minValue);
	return ;
}
void  CShapefileCategory::put_MinValue(VARIANT newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (newVal.vt == VT_I4 || newVal.vt == VT_R8 || newVal.vt == VT_BSTR)
		VariantCopy(&_minValue, &newVal);
	return ;
}

// ***************************************************************
//		MaxValue()
// ***************************************************************
void  CShapefileCategory::get_MaxValue(VARIANT* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (_maxValue.vt == VT_EMPTY)
		VariantInit(pVal);
	else
		VariantCopy(pVal, &_maxValue);
	return ;
}
void  CShapefileCategory::put_MaxValue(VARIANT newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (newVal.vt == VT_I4 || newVal.vt == VT_R8 || newVal.vt == VT_BSTR)
		VariantCopy(&_maxValue, &newVal);
	return ;
}


