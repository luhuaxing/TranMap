#pragma once
#include "LineSegment.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

// ------------------------------------------------------
//	   CLinePattern
// ------------------------------------------------------
class  CLinePattern
{
public:
	CLinePattern()
	{
		_key = SysAllocString(L"");
		_globalCallback = NULL;
		_lastErrorCode = tkNO_ERROR;
		_transparency = 255;
		gReferenceCounter.AddRef(idLinePattern);
	}
	~CLinePattern() 
	{
		::SysFreeString(_key);
		Clear();
		gReferenceCounter.Release(idLinePattern);
	}	
public:
	void get_Key(/*[out, retval]*/ BSTR *pVal);
	void put_Key(/*[in]*/ BSTR newVal);
	void get_GlobalCallback(/*[out, retval]*/ ICallback * *pVal);
	void put_GlobalCallback(/*[in]*/ ICallback * newVal);
	void get_ErrorMsg(/*[in]*/ long ErrorCode, /*[out, retval]*/ BSTR *pVal);
	void get_LastErrorCode(/*[out, retval]*/ long *pVal);
	
	void get_Count(int* retVal);

	void AddLine(OLE_COLOR color, float width, tkDashStyle style);
	void InsertLine(int Index, OLE_COLOR color, float width, tkDashStyle style, VARIANT_BOOL* retVal);

	void AddMarker(tkDefaultPointSymbol marker, CLineSegment** retVal );
	void InsertMarker(int Index, tkDefaultPointSymbol marker, CLineSegment** retVal );

	void RemoveItem(int Index, VARIANT_BOOL* retVal);
	void Clear();

	void get_Line(int Index, CLineSegment** retVal);
	void put_Line(int Index, CLineSegment* retVal);
	
	void Draw(int** hdc, float x, float y, int clipWidth, int clipHeight, OLE_COLOR backColor, VARIANT_BOOL* retVal);
	void DrawVB(int hdc, float x, float y, int clipWidth, int clipHeight, OLE_COLOR backColor, VARIANT_BOOL* retVal);

	void get_Transparency(BYTE *retVal);
	void put_Transparency(BYTE newVal);

	void Serialize(BSTR* retVal);
	void Deserialize(BSTR newVal);
	
private:
	std::vector <CLineSegment*> _lines;
	BSTR _key;
	long _lastErrorCode;
	ICallback* _globalCallback;
	unsigned char _transparency;

private:
	void ErrorMessage(long ErrorCode);

public:
	bool DeserializeCore(CPLXMLNode* node);
	CPLXMLNode* SerializeCore(CString ElementName);
	VARIANT_BOOL DrawCore(CDC* dc, float x, float y, int clipWidth, int clipHeight, OLE_COLOR backColor);
};
