#pragma once

// CGridColorBreak
class  CGridColorBreak
{
public:
	CGridColorBreak()
	{
		_colorType = Hillshade;
		_gradModel = Linear;
		_lastErrorCode = tkNO_ERROR;
		_globalCallback = NULL;
		_key = SysAllocString(L"");
	}
	~CGridColorBreak()
	{
		::SysFreeString(_key);
	}



// CGridColorBreak
public:
	void get_Key(/*[out, retval]*/ BSTR *pVal);
	void put_Key(/*[in]*/ BSTR newVal);
	void get_GlobalCallback(/*[out, retval]*/ ICallback * *pVal);
	void put_GlobalCallback(/*[in]*/ ICallback * newVal);
	void get_ErrorMsg(/*[in]*/ long ErrorCode, /*[out, retval]*/ BSTR *pVal);
	void get_LastErrorCode(/*[out, retval]*/ long *pVal);
	void get_GradientModel(/*[out, retval]*/ GradientModel *pVal);
	void put_GradientModel(/*[in]*/ GradientModel newVal);
	void get_ColoringType(/*[out, retval]*/ ColoringType *pVal);
	void put_ColoringType(/*[in]*/ ColoringType newVal);
	void get_LowValue(/*[out, retval]*/ double *pVal);
	void put_LowValue(/*[in]*/ double newVal);
	void get_HighValue(/*[out, retval]*/ double *pVal);
	void put_HighValue(/*[in]*/ double newVal);
	void get_LowColor(/*[out, retval]*/ OLE_COLOR *pVal);
	void put_LowColor(/*[in]*/ OLE_COLOR newVal);
	void get_HighColor(/*[out, retval]*/ OLE_COLOR *pVal);
	void put_HighColor(/*[in]*/ OLE_COLOR newVal);
	void get_Caption(BSTR* pVal);
	void put_Caption(BSTR newVal);
	
private:
	long _lastErrorCode;
	ICallback * _globalCallback;
	BSTR _key;
	OLE_COLOR _highColor;
	OLE_COLOR _lowColor;
	double _lowValue;
	double _highValue;
	CString _caption;
	ColoringType _colorType;
	GradientModel _gradModel;
};
