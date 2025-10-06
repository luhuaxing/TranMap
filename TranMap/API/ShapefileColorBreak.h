
#ifdef OLD_API 

#ifndef __SHAPEFILELEGENDBREAK_H_
#define __SHAPEFILELEGENDBREAK_H_

/////////////////////////////////////////////////////////////////////////////
// CShapefileColorBreak
class  CShapefileColorBreak
public:
	CShapefileColorBreak()
	{
		VariantInit(&_startValue);
		VariantInit(&_endValue);
		_visible = VARIANT_TRUE;
	}
	~CShapefileColorBreak()
	{	
		VariantClear(&_startValue);
		VariantClear(&_endValue);
	}


// CShapefileColorBreak
public:
	void get_EndColor(/*[out, retval]*/ OLE_COLOR *pVal);
	void put_EndColor(/*[in]*/ OLE_COLOR newVal);
	void get_StartColor(/*[out, retval]*/ OLE_COLOR *pVal);
	void put_StartColor(/*[in]*/ OLE_COLOR newVal);
	void get_EndValue(/*[out, retval]*/ VARIANT *pVal);
	void put_EndValue(/*[in]*/ VARIANT newVal);
	void get_StartValue(/*[out, retval]*/ VARIANT *pVal);
	void put_StartValue(/*[in]*/ VARIANT newVal);
	void get_Visible(/*[out, retval]*/ VARIANT_BOOL *pVal);
	void put_Visible(/*[in]*/ VARIANT_BOOL newVal);
	void get_Caption(BSTR* pVal);
	void put_Caption(BSTR newVal);

private:
	VARIANT _startValue;
	VARIANT _endValue;
	CString _caption;
	OLE_COLOR _startColor;
	OLE_COLOR _endColor;
	VARIANT_BOOL _visible;
	
};

#endif //__SHAPEFILELEGENDBREAK_H_

#endif