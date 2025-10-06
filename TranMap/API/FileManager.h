// FileManager.h : Declaration of the CFileManager
#pragma once
#include "Shapefile.h"
#include "OgrDataSource.h"
#include "OgrLayer.h"
// CFileManager
class  CFileManager
{
public:
	CFileManager()
	{
		_key = SysAllocString(L"");
		_lastErrorCode = tkNO_ERROR;
		_globalCallback = NULL;
		_lastOpenStrategy = fosAutoDetect;
		_lastOpenIsSuccess = true;
		_lastOpenFilename = L"";
	};
	~CFileManager()
	{
		SysFreeString(_key);
		if (_globalCallback)
			delete _globalCallback;
	};

public:
	void get_IsSupportedBy(BSTR Filename, tkSupportType supportType, VARIANT_BOOL* retVal);
	void get_IsSupported(BSTR Filename, VARIANT_BOOL* retVal);
	void get_IsRgbImage(BSTR Filename, VARIANT_BOOL* retVal);
	void get_IsGrid(BSTR Filename, VARIANT_BOOL* retVal);
	void get_IsVectorLayer(BSTR Filename, VARIANT_BOOL* retVal);
	void get_OpenStrategy(BSTR Filename, tkFileOpenStrategy* retVal);
	void get_CanOpenAs(BSTR Filename, tkFileOpenStrategy strategy, VARIANT_BOOL* retVal);
	void get_HasProjection(BSTR Filename, VARIANT_BOOL* retVal);
	void get_GeoProjection(BSTR Filename, CGeoProjection** retVal);
	void get_IsSameProjection(BSTR Filename, CGeoProjection* projection, VARIANT_BOOL* retVal);
	void Open(BSTR Filename, tkFileOpenStrategy openStrategy, ICallback* callback, ILayer** retVal);
	void OpenShapefile(BSTR Filename, ICallback* callback, CShapefile** retVal);
	void OpenRaster(BSTR Filename, tkFileOpenStrategy openStrategy, ICallback* callback, CImageClass** retVal);
	void DeleteDatasource(BSTR Filename, VARIANT_BOOL* retVal);
	void get_ErrorMsg(/*[in]*/ long ErrorCode, /*[out, retval]*/ BSTR *pVal);
	void get_LastErrorCode(/*[out, retval]*/ long *pVal);
	void get_Key(/*[out, retval]*/ BSTR *pVal);
	void put_Key(/*[in]*/ BSTR newVal);
	void get_GlobalCallback(/*[out, retval]*/ ICallback * *pVal);
	void put_GlobalCallback(/*[in]*/ ICallback * newVal);
	void get_LastOpenStrategy(tkFileOpenStrategy* retVal);
	void get_LastOpenFilename(BSTR* retVal);
	void get_LastOpenIsSuccess(VARIANT_BOOL* retVal);
	void get_HasGdalOverviews(BSTR Filename, VARIANT_BOOL* retVal);
	void ClearGdalOverviews(BSTR Filename, VARIANT_BOOL* retVal);
	void BuildGdalOverviews(BSTR Filename, VARIANT_BOOL* retVal);
	void get_NeedsGdalOverviews(BSTR Filename, VARIANT_BOOL* retVal);
	void RemoveProxyForGrid(BSTR Filename, VARIANT_BOOL* retVal);
	void get_HasValidProxyForGrid(BSTR Filename, VARIANT_BOOL* retVal);
	void OpenFromDatabase(BSTR connectionString, BSTR layerNameOrQuery, COgrLayer** retVal);
	void OpenVectorLayer(BSTR Filename, ShpfileType preferedShapeType, VARIANT_BOOL forUpdate, COgrLayer** retVal);
	void OpenVectorDatasource(BSTR Filename, COgrDatasource** retVal);
	void get_CdlgFilter(BSTR* pVal);
	void get_CdlgRasterFilter(BSTR* pVal);
	void get_CdlgVectorFilter(BSTR* pVal);
	void get_SupportedGdalFormats(BSTR* pVal);

private:
	tkFileOpenStrategy _lastOpenStrategy;
	bool _lastOpenIsSuccess;
	CStringW _lastOpenFilename;
	CString test;
	ICallback * _globalCallback;
	long _lastErrorCode;
	BSTR _key;

private:
	tkFileOpenStrategy CFileManager::get_OpenStrategyCore(BSTR Filename);
	void ErrorMessage(long ErrorCode);
	bool IsShapefile(CStringW filename);
	CString GetFilter(OpenFileDialogFilter filter);
	
};


