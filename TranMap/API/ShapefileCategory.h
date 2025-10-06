#pragma once
#include "ShapeDrawingOptions.h"
#include "ShapefileCategories.h"
class CShapefileCategories;

// CShapefileCategory
class  CShapefileCategory
{
public:
	CShapefileCategory()
	{
		_name = SysAllocString(L"");
		_expression = SysAllocString(L"");
		_priority = - 1;
		_drawingOptions = new CShapeDrawingOptions();
		
		_categories = NULL;
		_categoryValue = cvExpression;
		gReferenceCounter.AddRef(tkInterface::idShapefileCategory);
	}
	
	~CShapefileCategory()
	{
		::SysFreeString(_name);
		::SysFreeString(_expression);
		if (_drawingOptions != NULL)
		{
			delete _drawingOptions;
			_drawingOptions = NULL;
		}
	
		gReferenceCounter.Release(tkInterface::idShapefileCategory);
	}


public:
	void get_Name(BSTR* retval);
	void put_Name(BSTR newVal);
	void get_Expression(BSTR* retval);
	void put_Expression(BSTR newVal);
	void get_DrawingOptions(CShapeDrawingOptions** retval);
	void put_DrawingOptions(CShapeDrawingOptions* newVal);
	void get_Priority(LONG* retval)							{*retval = _priority;			return ;};
	void put_Priority(LONG newVal)							{_priority = newVal;			return ;};
	void get_ValueType(tkCategoryValue* pVal);
	void put_ValueType(tkCategoryValue newVal);
	void get_MinValue(VARIANT* pVal);
	void put_MinValue(VARIANT newVal);
	void get_MaxValue(VARIANT* pVal);
	void put_MaxValue(VARIANT newVal);

private:
	BSTR _name;
	BSTR _expression;
	CShapeDrawingOptions* _drawingOptions;
	long _priority;
	CShapefileCategories* _categories;
	tkCategoryValue _categoryValue;
	CComVariant _minValue;
	CComVariant _maxValue;
	
public:
	CDrawingOptionsEx* get_UnderlyingOptions();
	void put_underlyingOptions(CDrawingOptionsEx*);
	void put_parentCollection(CShapefileCategories* categories) {_categories = categories;}
	tkCategoryValue GetCategoryValue() {return _categoryValue; }
	CComVariant* GetMinValue() { return &_minValue; }
	CComVariant* GetMaxValue() { return &_maxValue; }
};