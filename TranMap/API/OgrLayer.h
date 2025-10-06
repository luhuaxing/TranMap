// OgrLayer.h : Declaration of the COgrLayer

#pragma once
#include "ogrsf_frmts.h"
#include "afxmt.h"
#include "OgrLoader.h"
#include "ILayer.h"

class  COgrLayer:public ILayer
{
public:
	COgrLayer()
	{
		_globalCallback = NULL;
		_key = SysAllocString(L"");
		_lastErrorCode = tkNO_ERROR;
		_dataset = NULL;
		_layer = NULL;
		_shapefile = NULL;
		_sourceType = ogrUninitialized;
		_forUpdate = false;
		_encoding = m_globalSettings.ogrEncoding;
		_dynamicLoading = VARIANT_FALSE;
		_envelope = NULL;
		_featureCount = -1;
		_loader.SetMaxCacheCount(m_globalSettings.ogrLayerMaxFeatureCount);
		gReferenceCounter.AddRef(tkInterface::idOgrLayer);
	}
	~COgrLayer()
	{
		SysFreeString(_key);
		if (_envelope)
			delete _envelope;
		Close();
		if (_globalCallback)
			delete _globalCallback;
		gReferenceCounter.Release(tkInterface::idOgrLayer);
	}


public:
	eLayerType GetLayerType() { return eLayerType::OGR_LAYER; }
	void get_ErrorMsg(/*[in]*/ long ErrorCode, /*[out, retval]*/ BSTR *pVal);
	void get_LastErrorCode(/*[out, retval]*/ long *pVal);
	void get_Key(/*[out, retval]*/ BSTR *pVal);
	void put_Key(/*[in]*/ BSTR newVal);
	void get_GlobalCallback(/*[out, retval]*/ ICallback * *pVal);
	void put_GlobalCallback(/*[in]*/ ICallback * newVal);
	void OpenDatabaseLayer(BSTR connectionString, int layerIndex, VARIANT_BOOL forUpdate, VARIANT_BOOL* retVal);   // not in public API
	void OpenFromQuery(BSTR connectionString, BSTR sql, VARIANT_BOOL* retVal);
	void OpenFromDatabase(BSTR connectionString, BSTR layerName, VARIANT_BOOL forUpdate, VARIANT_BOOL* retVal);
	void Close();
	void get_Name(BSTR* retVal);
	void GetBuffer(CShapefile** retVal);
	void get_SourceType(tkOgrSourceType* retVal);
	void ReloadFromSource(VARIANT_BOOL* retVal);
	void RedefineQuery(BSTR newSql, VARIANT_BOOL* retVal);
	void GetConnectionString(BSTR* retVal);
	void GetSourceQuery(BSTR* retVal);
	void get_GeoProjection(CGeoProjection** retVal);
	void get_ShapeType(ShpfileType* retVal);
	void get_DataIsReprojected(VARIANT_BOOL* retVal);
	void get_FIDColumnName(BSTR* retVal);
	void SaveChanges(int* savedCount, tkOgrSaveType saveType, VARIANT_BOOL validateShapes,  tkOgrSaveResult* retVal);
	void HasLocalChanges(VARIANT_BOOL* retVal);
	void TestCapability(tkOgrLayerCapability capability, VARIANT_BOOL* retVal);
	void get_UpdateSourceErrorCount(int* retVal);
	void get_UpdateSourceErrorMsg(int errorIndex, BSTR* retVal);
	void get_UpdateSourceErrorShapeIndex(int errorIndex, int* retVal);
	void get_FeatureCount(VARIANT_BOOL forceLoading, int* retVal);
	void get_Extents(CExtents** extents, VARIANT_BOOL forceLoading, VARIANT_BOOL *retVal);
	void get_GeometryColumnName(BSTR* retVal);
	void get_SupportsEditing(tkOgrSaveType editingType, VARIANT_BOOL* retVal);
	void Serialize(BSTR* retVal);
	void Deserialize(BSTR newVal, VARIANT_BOOL* retVal);
	void get_GdalLastErrorMsg(BSTR* pVal);
	void get_DynamicLoading(VARIANT_BOOL* pVal);
	void put_DynamicLoading(VARIANT_BOOL newVal);
	void get_MaxFeatureCount(LONG* pVal);
	void put_MaxFeatureCount(LONG newVal);
	void SaveStyle(BSTR Name, CStringW xml, VARIANT_BOOL* retVal);
	void get_SupportsStyles(VARIANT_BOOL* pVal);
	void GetNumStyles(LONG* pVal);
	void get_StyleName(LONG styleIndex, BSTR* pVal);
	void ClearStyles(VARIANT_BOOL* retVal);
	void RemoveStyle(BSTR styleName, VARIANT_BOOL* retVal);
	void get_LabelExpression(BSTR* pVal);
	void put_LabelExpression(BSTR newVal);
	void get_LabelPosition(tkLabelPositioning* pVal);
	void put_LabelPosition(tkLabelPositioning newVal);
	void get_LabelOrientation(tkLineLabelOrientation* pVal);
	void put_LabelOrientation(tkLineLabelOrientation newVal);
	void GenerateCategories(BSTR FieldName, tkClassificationType classificationType, long numClasses, tkMapColor colorStart, tkMapColor colorEnd, tkColorSchemeType schemeType, VARIANT_BOOL* retVal);
	void get_ShapeType2D(ShpfileType* pVal);
	void get_DriverName(BSTR* pVal);
	void OpenFromFile(BSTR Filename, VARIANT_BOOL forUpdate, VARIANT_BOOL* retVal);

private:
	VARIANT_BOOL _dynamicLoading;
	tkOgrSourceType _sourceType;
	CShapefile* _shapefile;
	GDALDataset* _dataset;
	OGRLayer* _layer;		// it's owned by dataset object
	ICallback * _globalCallback;
	long _lastErrorCode;
	BSTR _key;
	bool _forUpdate;
	CStringW _connectionString;
	CStringW _sourceQuery;
	vector<OgrUpdateError> _updateErrors;
	tkOgrEncoding _encoding;
	OGREnvelope* _envelope;
	int _featureCount;
	OgrDynamicLoader _loader;
	
private:
	bool CheckState();
	void ErrorMessage(long ErrorCode);
	void CloseShapefile();
	GDALDataset* OpenDataset(BSTR connectionString, bool forUpdate);
	long GetFidForShapefile();
	CShapefile* LoadShapefile();
	void InitOpenedLayer();
	void ForceCreateShapefile();
	void StopBackgroundLoading();
	void RestartBackgroundLoader() { _loader.Restart(); }
	void ClearCachedValues();
	bool HasStyleTable();
	CStringW GetDbSchemeName(bool withTrailingPoint);
	CStringW GetLayerName();
	CStringW GetStyleTableName();
	void GetFieldValues(OGRFieldType fieldType, BSTR& fieldName, vector<VARIANT*>& values);

public:
	void InjectShapefile(CShapefile* sfNew);
	CShapefile* GetShapefileNoRef() { return _shapefile; }
	OGRLayer* GetDatasource() { return _layer; }
	CPLXMLNode* SerializeCore(CString ElementName);
	bool DeserializeCore(CPLXMLNode* node);
	OgrDynamicLoader* GetDynamicLoader() { return &_loader; }
	CStringW LoadStyleXML(CStringW name);
	GDALDataset* GetDataset() { return _dataset; }
	bool DeserializeOptions(CPLXMLNode* node);
	
	
};