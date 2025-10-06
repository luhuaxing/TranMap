// OgrDatasource.h : Declaration of the COgrDatasource
#pragma once

// COgrDatasource
#include "OgrLayer.h"
class  COgrDatasource
{
public:
	COgrDatasource()
	{
		_dataset = NULL;
		_key = SysAllocString(L"");
		_lastErrorCode = tkNO_ERROR;
		_globalCallback = NULL;
		_encoding = m_globalSettings.ogrEncoding;
		gReferenceCounter.AddRef(tkInterface::idOgrDatasource);
	}
	~COgrDatasource()
	{
		SysFreeString(_key);
		Close();
		if (_globalCallback)
			delete _globalCallback;
		gReferenceCounter.Release(tkInterface::idOgrDatasource);
	}

public:
	void get_ErrorMsg(/*[in]*/ long ErrorCode, /*[out, retval]*/ BSTR *pVal);
	void get_LastErrorCode(/*[out, retval]*/ long *pVal);
	void get_Key(/*[out, retval]*/ BSTR *pVal);
	void put_Key(/*[in]*/ BSTR newVal);
	void get_GlobalCallback(/*[out, retval]*/ ICallback * *pVal);
	void put_GlobalCallback(/*[in]*/ ICallback * newVal);
	void Close();
	void Open(BSTR connectionString, VARIANT_BOOL* retVal);
	void get_LayerCount(int* retVal);
	void GetLayer(int index, VARIANT_BOOL forUpdate, COgrLayer** retVal);
	void get_DriverName(BSTR* retVal);
	void GetLayerName(int index, BSTR* retVal);
	void GetLayerByName(BSTR name, VARIANT_BOOL forUpdate, COgrLayer** retVal);
	void RunQuery(BSTR sql, COgrLayer** retVal);
	void DeleteLayer(int layerIndex, VARIANT_BOOL* retVal);
	void CreateLayer(BSTR layerName, ShpfileType shpType, CGeoProjection* projection, BSTR creationOptions, VARIANT_BOOL* retVal);
	void ImportShapefile(CShapefile* shapefile, BSTR newLayerName, BSTR creationOptions, 
		tkShapeValidationMode validationMode, VARIANT_BOOL* retVal);
	void TestCapability(tkOgrDSCapability capability, VARIANT_BOOL* retVal);
	void LayerIndexByName(BSTR layerName, int* retVal);
	void get_DriverMetadata(tkGdalDriverMetadata metadata, BSTR* retVal);
	void get_DriverMetadataCount(int* retVal);
	void get_DriverMetadataItem(int metadataIndex, BSTR* retVal);
	void ExecuteSQL(BSTR sql, BSTR* errorMessage, VARIANT_BOOL* retVal);
	void get_GdalLastErrorMsg(BSTR* pVal);

private:
	ICallback * _globalCallback;
	long _lastErrorCode;
	BSTR _key;
	GDALDataset* _dataset;
	CStringW _connectionString;
	tkOgrEncoding _encoding;

private:
	bool CheckState();
	void DumpDriverInfo();
	void ErrorMessage(long ErrorCode);
	void GetMetaData(GDALDriver* driver);
	char** ParseLayerCreationOptions(BSTR creationOptions);

public:
	GDALDataset* GetDataset() { return _dataset; }
};

