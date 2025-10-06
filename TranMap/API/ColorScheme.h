#pragma once

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

// CColorScheme
class  CColorScheme 
{
public:
	CColorScheme()
	{
		_key = SysAllocString(L"");
		_globalCallback = NULL;
		_lastErrorCode = tkNO_ERROR;
		_type = ctSchemeGraduated;
	}

	~CColorScheme()
	{
		::SysFreeString(_key);
	}


public:
	void SetColors(OLE_COLOR Color1, OLE_COLOR Color2);
	void SetColors2(tkMapColor Color1, tkMapColor Color2);
	void SetColors3(short MinRed, short MinGreen, short MinBlue, short MaxRed, short MaxGreen, short MaxBlue);
	void SetColors4(PredefinedColorScheme Scheme);
	void get_NumBreaks(long * retVal);
	void AddBreak(double Value, OLE_COLOR Color);//, VARIANT_BOOL* retVal);
	void Remove(long Index, VARIANT_BOOL* retVal);
	void Clear();
	void get_RandomColor(double Value, OLE_COLOR* retVal);
	void get_GraduatedColor(double Value, OLE_COLOR* retVal);
	void get_BreakColor(long Index, OLE_COLOR* retVal);
	void put_BreakColor(long Index, OLE_COLOR newVal);
	void get_BreakValue(long Index, double* retVal);

	void get_Key(/*[out, retval]*/ BSTR *pVal);
	void put_Key(/*[in]*/ BSTR newVal);
	void get_GlobalCallback(/*[out, retval]*/ ICallback * *pVal);
	void put_GlobalCallback(/*[in]*/ ICallback * newVal);
	void get_ErrorMsg(/*[in]*/ long ErrorCode, /*[out, retval]*/ BSTR *pVal);
	void get_LastErrorCode(/*[out, retval]*/ long *pVal);
	
private:
	struct ColorBreak
	{
		double value;
		OLE_COLOR color;
	};

private:	
	std::vector<ColorBreak> _breaks;
	BSTR _key;
	long _lastErrorCode;
	ICallback* _globalCallback;
	tkColorSchemeType _type;

public:
	void ErrorMessage(long ErrorCode);
};


