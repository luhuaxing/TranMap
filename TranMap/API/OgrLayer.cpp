#include "stdafx.h"
#include "OgrLayer.h"
#include "OgrHelper.h"
#include "UtilityFunctions.h"
#include "GeoProjection.h"
#include "OgrConverter.h"
#include "Shapefile.h"
#include "OgrStyle.h"
#include "Shape2Ogr.h"
#include "Ogr2Shape.h"
#include "FieldClassification.h"
#include "ShapefileCategories.h"

// *************************************************************
//		InjectShapefile()
// *************************************************************
void COgrLayer::InjectShapefile(CShapefile* sfNew)
{
	CloseShapefile();
	_shapefile = sfNew;
}

// *************************************************************
//		InitOpenedLayer()
// *************************************************************
void COgrLayer::InitOpenedLayer()
{
	if (Utility::FileExistsW(_connectionString)) {
		_sourceType = ogrFile;
	}

	ClearCachedValues();
	VARIANT_BOOL vb;

	// Let's cache some values, as the driver can be busy with background loading
	// later on, so the value may not be readily available
	int featureCount;
	get_FeatureCount(VARIANT_FALSE, &featureCount);
	
	CExtents* extents = NULL;
	get_Extents(&extents, VARIANT_FALSE, &vb);

	if (m_globalSettings.autoChooseOgrLoadingMode) {
		long maxCount;
		get_MaxFeatureCount(&maxCount);
		put_DynamicLoading(featureCount > maxCount ? VARIANT_TRUE : VARIANT_FALSE);
	}

	ForceCreateShapefile();
	RestartBackgroundLoader();
}

//***********************************************************************
//*		ClearCachedValues()
//***********************************************************************
void COgrLayer::ClearCachedValues()
{
	if (_envelope)
	{
		delete _envelope;
		_envelope = NULL;
	}
	if (_featureCount != -1) 
		_featureCount = -1;
}

//***********************************************************************
//*		StopBackgroundLoading()
//***********************************************************************
void COgrLayer::StopBackgroundLoading()
{
	_loader.AddWaitingTask(true);  // notify working thread that it's time is over
	_loader.LockLoading(true);     // wait until it is finished
	_loader.LockLoading(false);
}

//***********************************************************************
//*		LoadShapefile()
//***********************************************************************
CShapefile* COgrLayer::LoadShapefile()
{ 
	bool isTrimmed = false;	
	CShapefile* sf = Ogr2Shape::Layer2Shapefile(_layer, _loader.GetMaxCacheCount(), isTrimmed, &_loader, _globalCallback); 
	if (isTrimmed) {
		ErrorMessage(tkOGR_LAYER_TRIMMED);
	}
	return sf;
}

//***********************************************************************
//*		get/put_Key()
//***********************************************************************
void  COgrLayer::get_Key(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	*pVal = OLE2BSTR(_key);
	return ;
}
void  COgrLayer::put_Key(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	::SysFreeString(_key);
	USES_CONVERSION;
	_key = OLE2BSTR(newVal);
	return ;
}

//***********************************************************************/
//*			ErrorMessage()
//***********************************************************************/
void COgrLayer::ErrorMessage(long ErrorCode)
{
	_lastErrorCode = ErrorCode;
	CallbackHelper::ErrorMsg("OgrLayer", _globalCallback, _key, ErrorMsg(_lastErrorCode));
}

void  COgrLayer::get_LastErrorCode(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*pVal = _lastErrorCode;
	_lastErrorCode = tkNO_ERROR;
	return ;
}

void  COgrLayer::get_ErrorMsg(long ErrorCode, BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	*pVal = A2BSTR(ErrorMsg(ErrorCode));
	return ;
}

//***********************************************************************/
//*		get/put_GlobalCallback()
//***********************************************************************/
void  COgrLayer::get_GlobalCallback(ICallback **pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*pVal = _globalCallback;
	return ;
}

void  COgrLayer::put_GlobalCallback(ICallback *newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	_globalCallback = newVal;
	return ;
}

// *************************************************************
//		CheckState()
// *************************************************************
bool COgrLayer::CheckState()
{
	if (!_dataset)
	{
		ErrorMessage(tkOGR_LAYER_UNINITIALIZED);
		return false;
	}
	return true;
}

// *************************************************************
//		get_SourceType()
// *************************************************************
void  COgrLayer::get_SourceType(tkOgrSourceType* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = _sourceType;
	return ;
}

// *************************************************************
//		Close()
// *************************************************************
void  COgrLayer::Close()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	StopBackgroundLoading();
	if (_dataset)
	{
		if (_sourceType == ogrQuery && _layer)
			_dataset->ReleaseResultSet(_layer);
	
		// this will release memory for table layer as well
		GDALClose(_dataset);
		_dataset = NULL;
		_layer = NULL;
	}
	CloseShapefile();
	_updateErrors.clear();
	_sourceType = ogrUninitialized;
	_connectionString = L"";
	_sourceQuery = L"";
	_forUpdate = false;
	return ;
}

// *************************************************************
//		CloseShapefile()
// *************************************************************
void COgrLayer::CloseShapefile()
{
	if (_shapefile)
	{
		VARIANT_BOOL vb;
		_shapefile->Close(&vb);
		delete _shapefile;
		_shapefile = NULL;
	}
}

// *************************************************************
//		OpenDataset()
// *************************************************************
GDALDataset* COgrLayer::OpenDataset(BSTR connectionString, bool forUpdate)
{
	GDALDataset* ds = GdalHelper::OpenOgrDatasetW(OLE2W(connectionString), forUpdate);
	if (!ds)
	{
		// clients should extract last GDAL error
		ErrorMessage(tkFAILED_TO_OPEN_OGR_DATASOURCE);
	}
	return ds;
}

// *************************************************************
//		OpenDatabaseLayer()
// *************************************************************
void  COgrLayer::OpenDatabaseLayer(BSTR connectionString, int layerIndex, VARIANT_BOOL forUpdate, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = VARIANT_FALSE;
	Close();

	GDALDataset* ds = OpenDataset(connectionString, forUpdate ? true: false);
	if (ds)
	{
		if (layerIndex < 0 || layerIndex >= ds->GetLayerCount())
		{
			ErrorMessage(tkINDEX_OUT_OF_BOUNDS);
			GDALClose(ds);
			return ;
		}
		
		OGRLayer* layer = ds->GetLayer(layerIndex);
		if (!layer)
		{
			ErrorMessage(tkFAILED_TO_OPEN_OGR_LAYER);
			GDALClose(ds);
			return ;
		}
	
		USES_CONVERSION;
		_connectionString = OLE2W(connectionString);
		_sourceQuery = OgrHelper::OgrString2Unicode(layer->GetName());
		_dataset = ds;
		_layer = layer;
		_forUpdate = forUpdate == VARIANT_TRUE;
		_sourceType = ogrDbTable;
		InitOpenedLayer();

		*retVal = VARIANT_TRUE;
		return ;
	}
	return ;
}

// *************************************************************
//		OpenFromQuery()
// *************************************************************
void  COgrLayer::OpenFromQuery(BSTR connectionString, BSTR sql, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = VARIANT_FALSE;
	Close();

	GDALDataset* ds = OpenDataset(connectionString, false);
	if (ds)
	{
		OGRLayer* layer = ds->ExecuteSQL(OgrHelper::Bstr2OgrString(sql), NULL, NULL);
		if (layer)
		{
			_connectionString = OLE2W(connectionString);
			_sourceQuery = OLE2W(sql);
			_dataset = ds;
			_layer = layer;
			_sourceType = ogrQuery;
			_forUpdate = VARIANT_FALSE;
			InitOpenedLayer();
			*retVal = VARIANT_TRUE;
			return ;
		}
		else
		{
			ErrorMessage(tkOGR_QUERY_FAILED);
			GDALClose(_dataset);
		}
	}
	return ;
}

// *************************************************************
//		OpenFromDatabase()
// *************************************************************
void  COgrLayer::OpenFromDatabase(BSTR connectionString, BSTR layerName, VARIANT_BOOL forUpdate, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = VARIANT_FALSE;
	Close();

	GDALDataset* ds = OpenDataset(connectionString, forUpdate ? true : false);
	if (ds)
	{
		OGRLayer* layer = ds->GetLayerByName(OgrHelper::Bstr2OgrString(layerName));
		if (layer)
		{
			_connectionString = OLE2W(connectionString);
			_sourceQuery = OLE2W(layerName);
			_sourceType = ogrDbTable;
			_dataset = ds;
			_layer = layer;
			_forUpdate = forUpdate == VARIANT_FALSE;
			InitOpenedLayer();
			*retVal = VARIANT_TRUE;
			return ;
		}
		else
		{
			ErrorMessage(tkFAILED_TO_OPEN_OGR_LAYER);
			GDALClose(_dataset);
		}
	}
	return ;
}

// *************************************************************
//		OpenFromFile()
// *************************************************************
void  COgrLayer::OpenFromFile(BSTR Filename, VARIANT_BOOL forUpdate, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	*retVal = VARIANT_FALSE;
	Close();

	GDALDataset* ds = OpenDataset(Filename, forUpdate ? true : false);
	if (ds)
	{
		OGRLayer* layer = ds->GetLayer(0);
		if (layer)
		{
			_connectionString = OLE2W(Filename);
			_sourceQuery = OgrHelper::OgrString2Unicode(layer->GetName());
			_sourceType = ogrFile;
			_dataset = ds;
			_layer = layer;
			_forUpdate = forUpdate == VARIANT_FALSE;
			InitOpenedLayer();
			*retVal = VARIANT_TRUE;
			return ;
		}
		else
		{
			ErrorMessage(tkFAILED_TO_OPEN_OGR_LAYER);
			GDALClose(_dataset);
		}
	}
	return ;
}

// *************************************************************
//		get_Name()
// *************************************************************
void  COgrLayer::get_Name(BSTR* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if (!CheckState())
	{
		*retVal = A2BSTR("");
		return ;
	}
	else
	{
		CStringW name = OgrHelper::OgrString2Unicode(_layer->GetName());
		*retVal = W2BSTR(name);
		return ;
	}
}

// *************************************************************
//		get_Data()
// *************************************************************
void  COgrLayer::GetBuffer(CShapefile** retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	*retVal = NULL;
	if (!CheckState()) return ;
	
	CSingleLock sfLock(&_loader.ShapefileLock);
	sfLock.Lock();
	
	if (!_shapefile)
	{
		if (_dynamicLoading) {
			CSingleLock lock(&_loader.ProviderLock);
			lock.Lock();
			_shapefile = Ogr2Shape::CreateShapefile(_layer);
		}
		else {
			_shapefile = LoadShapefile();
		}
	}
	
	sfLock.Unlock();

	if (_shapefile)
	{
		*retVal = _shapefile;
	//	_shapefile->AddRef();
	}
	return ;
}

// *************************************************************
//		ReloadFromSource()
// *************************************************************
void  COgrLayer::ReloadFromSource(VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = VARIANT_FALSE;
	if (!CheckState()) return ;

	if (_dynamicLoading) 
	{
		ErrorMessage(tkNOT_ALLOWED_IN_OGR_DYNAMIC_MODE);
		return ;		
	}
	
	CSingleLock sfLock(&_loader.ShapefileLock);
	sfLock.Lock();

	CloseShapefile();
	_shapefile = LoadShapefile();
	
	sfLock.Unlock();
	*retVal = _shapefile ? VARIANT_TRUE : VARIANT_FALSE;
	return ;
}

// *************************************************************
//		RedefineQuery()
// *************************************************************
void  COgrLayer::RedefineQuery(BSTR newSql, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = VARIANT_FALSE;
	if (!CheckState()) return ;

	if (_dynamicLoading)
	{
		ErrorMessage(tkNOT_ALLOWED_IN_OGR_DYNAMIC_MODE);
		return ;
	}

	if (_sourceType == ogrQuery)
	{
		OGRLayer* layer = _dataset->ExecuteSQL(OgrHelper::Bstr2OgrString(newSql), NULL, NULL);
		if (layer)
		{
			_dataset->ReleaseResultSet(_layer);
			_sourceQuery = OLE2W(newSql);
			_layer = layer;
			ClearCachedValues();
			CloseShapefile();
			return ;
		}
		else
		{
			ErrorMessage(tkOGR_QUERY_FAILED);
			return ;
		}
	}
	else
	{
		ErrorMessage(tkUNEXPECTED_OGR_SOURCE_TYPE);
		return ;
	}
	return ;
}

// *************************************************************
//		get_ConnectionString()
// *************************************************************
void  COgrLayer::GetConnectionString(BSTR* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	USES_CONVERSION;
	*retVal = W2BSTR(_connectionString);
	return ;
}

// *************************************************************
//		get_SourceQuery()
// *************************************************************
void  COgrLayer::GetSourceQuery(BSTR* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	USES_CONVERSION;
	*retVal = W2BSTR(_sourceQuery);
	return ;
}

// *************************************************************
//		get_GeoProjection()
// *************************************************************
void  COgrLayer::get_GeoProjection(CGeoProjection** retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CGeoProjection* gp = new CGeoProjection();
	*retVal = gp;

	if (!CheckState()) return ;

	CSingleLock lock(&_loader.ProviderLock);
	if (_dynamicLoading) lock.Lock();

	OGRSpatialReference* sr = _layer->GetSpatialRef();// owned by OGRLayer
	if (sr)((CGeoProjection*)gp)->InjectSpatialReference(sr);
	return ;
}

// *************************************************************
//		get_ShapeType()
// *************************************************************
void  COgrLayer::get_ShapeType(ShpfileType* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = SHP_NULLSHAPE;
	if (!CheckState()) return ;
	*retVal = OgrConverter::GeometryType2ShapeType(_layer->GetGeomType());
	return ;
}

// *************************************************************
//		get_ShapeType2D()
// *************************************************************
void  COgrLayer::get_ShapeType2D(ShpfileType* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	get_ShapeType(pVal);
	*pVal = Utility::ShapeTypeConvert2D(*pVal);
	return ;
}

// *************************************************************
//		get_DataIsReprojected()
// *************************************************************
void  COgrLayer::get_DataIsReprojected(VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = VARIANT_FALSE;
	if (!CheckState()) return ;
	if (!_shapefile) return ;		// data wasn't loaded yet

	CGeoProjection* gp = NULL;
	get_GeoProjection(&gp);
	if (!gp) return ;

	CShapefile* sf = NULL;
	GetBuffer(&sf);
	if (sf)
	{
		CGeoProjection* gp2 = NULL;
		sf->get_GeoProjection(&gp2);
		VARIANT_BOOL isSame;
		gp->get_IsSame(gp2, &isSame);
		*retVal = !isSame;
	}
	return ;
}

// *************************************************************
//		get_FIDColumn()
// *************************************************************
void  COgrLayer::get_FIDColumnName(BSTR* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (CheckState())
	{
		CStringW s = OgrHelper::OgrString2Unicode(_layer->GetFIDColumn());
		*retVal = W2BSTR(s);
		return ;
	}
	*retVal = A2BSTR("");
	return ;
}

// *************************************************************
//		SaveChanges()
// *************************************************************
void  COgrLayer::SaveChanges(int* savedCount, tkOgrSaveType saveType, VARIANT_BOOL validateShapes, tkOgrSaveResult* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*savedCount = 0;
	*retVal = osrNoChanges;
	_updateErrors.clear();

	if (!CheckState()) return ;
	if (!_shapefile)
	{
		ErrorMessage(tkNO_OGR_DATA_WAS_LOADED);
		return ;
	}
	
	VARIANT_BOOL hasChanges;
	HasLocalChanges(&hasChanges);
	if (!hasChanges)
	{
		ErrorMessage(tkOGR_NO_MODIFICATIONS);
		return ;
	}

	*retVal = osrNoneSaved;
	long shapeCmnId = GetFidForShapefile();
	if (shapeCmnId == -1)
	{
		ErrorMessage(tkFID_COLUMN_NOT_FOUND);
		return ;
	}

	VARIANT_BOOL reprojected;
	get_DataIsReprojected(&reprojected);
	if (reprojected && saveType != tkOgrSaveType::ostAttributesOnly)
	{
		ErrorMessage(tkCANT_SAVE_REPROJECTED_GEOMETRIES);
		return ;
	}

	CSingleLock lock(&_loader.ProviderLock);
	if (_dynamicLoading) lock.Lock();

	*savedCount = Shape2Ogr::SaveShapefileChanges(_layer, _shapefile, shapeCmnId, saveType, validateShapes ? true : false, _updateErrors);

	lock.Unlock();

	if (*savedCount == 0)
	{
		*retVal = osrNoneSaved;
	}
	else
	{
		HasLocalChanges(&hasChanges);
		*retVal = hasChanges ? osrSomeSaved : osrAllSaved;
	}
	return ;
}

// *************************************************************
//		HasLocalChanges()
// *************************************************************
void  COgrLayer::HasLocalChanges(VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = VARIANT_FALSE;
	if (!CheckState()) return ;

	if (_shapefile)
	{
		long numShapes;
		_shapefile->get_NumShapes(&numShapes);

		CSingleLock lock(&_loader.ProviderLock);
		if (_dynamicLoading) lock.Lock();

		int featureCount = static_cast<int>(_layer->GetFeatureCount());
		if (numShapes != featureCount)	   // i.e. deleted or inserted feature
		{
			*retVal = VARIANT_TRUE;
		}
		else
		{
			VARIANT_BOOL modified;
			for (long i = 0; i < numShapes; i++)
			{
				_shapefile->get_ShapeModified(i, &modified);
				if (modified)
				{
					*retVal = VARIANT_TRUE;
					break;
				}
			}
		}
	}
	return ;
}

// *************************************************************
//		GetFidMapping()
// *************************************************************
long COgrLayer::GetFidForShapefile()
{
	if (!_layer || !_shapefile) return -1;
	CComBSTR bstr;
	get_FIDColumnName(&bstr);
	CTableClass* table = NULL;
	_shapefile->get_Table(&table);
	long shapeCmnId;
	table->get_FieldIndexByName(bstr, &shapeCmnId);
	return shapeCmnId;
}

// *************************************************************
//		TestCapability()
// *************************************************************
void  COgrLayer::TestCapability(tkOgrLayerCapability capability, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = VARIANT_FALSE;
	if (!CheckState()) return ;
	int val = _layer->TestCapability(OgrHelper::GetLayerCapabilityString(capability));
	*retVal = val ? VARIANT_TRUE : VARIANT_FALSE;
	return ;
}

// *************************************************************
//		UpdateSourceErrorCount()
// *************************************************************
void  COgrLayer::get_UpdateSourceErrorCount(int* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = _updateErrors.size();
	return ;
}

// *************************************************************
//		UpdateSourceErrorMsg()
// *************************************************************
void  COgrLayer::get_UpdateSourceErrorMsg(int errorIndex, BSTR* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if (errorIndex < 0 || errorIndex >= (int)_updateErrors.size())
	{
		*retVal = A2BSTR("");
		ErrorMessage(tkINDEX_OUT_OF_BOUNDS);
		return ;
	}
	*retVal = W2BSTR(_updateErrors[errorIndex].ErrorMsg);
	return ;
}

// *************************************************************
//		UpdateSourceErrorShapeIndex()
// *************************************************************
void  COgrLayer::get_UpdateSourceErrorShapeIndex(int errorIndex, int* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if (errorIndex < 0 || errorIndex >= (int)_updateErrors.size())
	{
		*retVal = -1;
		ErrorMessage(tkINDEX_OUT_OF_BOUNDS);
		return ;
	}
	*retVal = _updateErrors[errorIndex].ShapeIndex;
	return ;
}

// *************************************************************
//		get_FeatureCount()
// *************************************************************
void  COgrLayer::get_FeatureCount(VARIANT_BOOL forceLoading, int* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = 0;
	if (!CheckState()) return ;
	CSingleLock lock(&_loader.ProviderLock);
	if (_dynamicLoading) lock.Lock();
	if (_featureCount == -1 || forceLoading) {
		_featureCount = static_cast<int>(_layer->GetFeatureCount(forceLoading == VARIANT_TRUE));
	}
	*retVal = _featureCount;
	return ;
}


// *************************************************************
//		ForceCreateShapefile()
// *************************************************************
void COgrLayer::ForceCreateShapefile()
{
	tkOgrSourceType sourceType;
	get_SourceType(&sourceType);
	if (_dynamicLoading && (!_shapefile) && (sourceType != ogrUninitialized)) {
		CSingleLock lock(&_loader.ProviderLock);
		lock.Lock();
		_shapefile = Ogr2Shape::CreateShapefile(_layer);
	}
}

// *************************************************************
//		get_Extents()
// *************************************************************
void  COgrLayer::get_Extents(CExtents** extents, VARIANT_BOOL forceLoading, VARIANT_BOOL *retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*extents = NULL;
	*retVal = VARIANT_FALSE;
	if (!CheckState()) return ;

	if (_envelope && forceLoading) {
		delete _envelope;
		_envelope = NULL;
	}

	if (!_envelope) {
		
		_envelope = new OGREnvelope();
		CSingleLock lock(&_loader.ProviderLock);
		if (_dynamicLoading) lock.Lock();
		_layer->GetExtent(_envelope, forceLoading == VARIANT_TRUE);
	}
	
	if (_envelope) {
		CExtents* ext = new CExtents();
		ext->SetBounds(_envelope->MinX, _envelope->MinY, 0.0, _envelope->MaxX, _envelope->MaxY, 0.0);
		*extents = ext;
		*retVal = VARIANT_TRUE;
		return ;
	}
	
	return ;
}

// *************************************************************
//		get_GeometryColumnName()
// *************************************************************
void  COgrLayer::get_GeometryColumnName(BSTR* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if (!CheckState()) return ;
	CStringW name = OgrHelper::OgrString2Unicode(_layer->GetGeometryColumn());
	USES_CONVERSION;
	*retVal = W2BSTR(name);
	return ;
}

// *************************************************************
//		get_SupportsEditing()
// *************************************************************
void  COgrLayer::get_SupportsEditing(tkOgrSaveType editingType, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = VARIANT_FALSE;
	if (!CheckState()) return ;
	
	// is it supported by driver?
	VARIANT_BOOL randomWrite;
	TestCapability(tkOgrLayerCapability::olcRandomWrite, &randomWrite);
	if (!randomWrite)
	{
		ErrorMessage(tkOGR_RANDOM_WRITE_NOT_SUPPORTED);
		return ;
	}
		
	// do we have FID column?
	if (_shapefile)
	{
		long fid = GetFidForShapefile();
		if (fid == -1)
		{
			ErrorMessage(tkFID_COLUMN_NOT_FOUND);
			return ;
		}
	}
	else
	{
		CString cmn = _layer->GetFIDColumn();
		if (cmn.GetLength() == 0)
		{
			ErrorMessage(tkFID_COLUMN_NOT_FOUND);
			return ;
		}
	}

	// do we have reprojected data?
	VARIANT_BOOL reprojected;
	get_DataIsReprojected(&reprojected);
	if (reprojected && editingType != ostAttributesOnly)
	{
		ErrorMessage(tkCANT_SAVE_REPROJECTED_GEOMETRIES);
		return ;
	}
	*retVal = VARIANT_TRUE;
	return ;
}

// *************************************************************
//		Serialize()
// *************************************************************
void  COgrLayer::Serialize(BSTR* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	CPLXMLNode* psTree = this->SerializeCore("OgrLayerClass");
	Utility::SerializeAndDestroyXmlTree(psTree, retVal);
	return ;
}

// *************************************************************
//		SerializeCore()
// *************************************************************
CPLXMLNode* COgrLayer::SerializeCore(CString ElementName)
{
	CPLXMLNode* psTree = CPLCreateXMLNode(NULL, CXT_Element, ElementName);

	USES_CONVERSION;
	CStringW skey = OLE2W(_key);
	if (skey.GetLength() != 0) Utility::CPLCreateXMLAttributeAndValue(psTree, "Key", skey);
	Utility::CPLCreateXMLAttributeAndValue(psTree, "ConnectionString", Utility::ConvertToUtf8(_connectionString));
	Utility::CPLCreateXMLAttributeAndValue(psTree, "SourceQuery", Utility::ConvertToUtf8(_sourceQuery));
	Utility::CPLCreateXMLAttributeAndValue(psTree, "SourceType", CPLString().Printf("%d", (int)_sourceType));
	Utility::CPLCreateXMLAttributeAndValue(psTree, "ForUpdate", CPLString().Printf("%d", (int)_forUpdate));
	if (_loader.LabelExpression.GetLength() > 0)
		Utility::CPLCreateXMLAttributeAndValue(psTree, "LabelExpression", Utility::ConvertToUtf8(_loader.LabelExpression));
	if (_loader.LabelPosition != lpNone)
		Utility::CPLCreateXMLAttributeAndValue(psTree, "LabelPosition", CPLString().Printf("%d", (int)_loader.LabelPosition));
	if (_loader.LabelOrientation != lorParallel)
		Utility::CPLCreateXMLAttributeAndValue(psTree, "LabelOrientation", CPLString().Printf("%d", (int)_loader.LabelOrientation));
	if (_loader.GetMaxCacheCount() != m_globalSettings.ogrLayerMaxFeatureCount)
		Utility::CPLCreateXMLAttributeAndValue(psTree, "MaxFeatureCount", CPLString().Printf("%d", (int)_loader.GetMaxCacheCount()));
	
	if (_shapefile)
	{
		CSingleLock sfLock(&_loader.ShapefileLock);
		sfLock.Lock();
		CPLXMLNode* sfNode = ((CShapefile*)_shapefile)->SerializeCore(VARIANT_FALSE, "ShapefileData", true);
		sfLock.Unlock();
		CPLAddXMLChild(psTree, sfNode);
	}
	return psTree;
}

// *************************************************************
//		Deserialize()
// *************************************************************
void  COgrLayer::Deserialize(BSTR newVal, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = VARIANT_FALSE;

	USES_CONVERSION;
	CString s = OLE2CA(newVal);
	CPLXMLNode* node = CPLParseXMLString(s.GetString());
	if (node)
	{
		CPLXMLNode* nodeLayer = CPLGetXMLNode(node, "=OgrLayerClass");
		if (nodeLayer)
		{
			if (DeserializeCore(nodeLayer))
				*retVal = VARIANT_TRUE;
		}
		CPLDestroyXMLNode(node);
	}
	return ;
}

// *************************************************************
//		DeserializeCore()
// *************************************************************
bool COgrLayer::DeserializeCore(CPLXMLNode* node)
{
	if (!node) return false;

	Close();

	CString s = CPLGetXMLValue(node, "SourceType", NULL);
	tkOgrSourceType sourceType = (s != "") ? (tkOgrSourceType)atoi(s.GetString()) : ogrUninitialized;

	CStringW connectionString = Utility::ConvertFromUtf8(CPLGetXMLValue(node, "ConnectionString", ""));
	CStringW sourceQuery = Utility::ConvertFromUtf8(CPLGetXMLValue(node, "SourceQuery", ""));

	s = CPLGetXMLValue(node, "ForUpdate", NULL);
	bool forUpdate = (s != "") ? (atoi(s.GetString()) == 0 ? false : true) : false;

	CComBSTR bstrConnection(connectionString);
	CComBSTR bstrQuery(sourceQuery);

	VARIANT_BOOL vb = VARIANT_FALSE;
	if (sourceType == ogrDbTable || sourceType == ogrFile)
	{
		OpenFromDatabase(bstrConnection, bstrQuery, forUpdate ? VARIANT_TRUE : VARIANT_FALSE, &vb);
	}
	else if (sourceType == ogrQuery)
	{
		OpenFromQuery(bstrConnection, bstrQuery, &vb);
	}
	
	vb = DeserializeOptions(node) ? VARIANT_TRUE : VARIANT_FALSE;

	return vb == VARIANT_TRUE;
}

// *************************************************************
//		DeserializeOptions()
// *************************************************************
bool COgrLayer::DeserializeOptions(CPLXMLNode* node)
{
	bool success = true;
	_loader.LabelExpression = Utility::ConvertFromUtf8(CPLGetXMLValue(node, "LabelExpression", ""));

	CString s = CPLGetXMLValue(node, "LabelPosition", NULL);
	_loader.LabelPosition = (s != "") ? (tkLabelPositioning)atoi(s.GetString()) : lpNone;

	s = CPLGetXMLValue(node, "LabelOrientation", NULL);
	_loader.LabelOrientation = (s != "") ? (tkLineLabelOrientation)atoi(s.GetString()) : lorParallel;

	s = CPLGetXMLValue(node, "MaxFeatureCount", NULL);
	_loader.SetMaxCacheCount((s != "") ? atoi(s.GetString()) : m_globalSettings.ogrLayerMaxFeatureCount);

	// let's populate data (in case it was populated before serialization)
	if (_sourceType != ogrUninitialized && _layer != NULL)
	{
		CPLXMLNode* psChild = CPLGetXMLNode(node, "ShapefileData");
		if (psChild)
		{
			if (!_shapefile) {
				CShapefile* sf = LoadShapefile();
				CSingleLock sfLock(&_loader.ShapefileLock);
				sfLock.Lock();
				_shapefile = sf;
				sfLock.Unlock();
			}
			bool result = ((CShapefile*)_shapefile)->DeserializeCore(VARIANT_FALSE, psChild);
			if (!result) success = false;
		}
	}

	CString key = CPLGetXMLValue(node, "Key", "");
	if (key.GetLength() > 0)
	{
		SysFreeString(_key);
		_key = A2BSTR(key);
	}
	return success;
}

// *************************************************************
//		get_GdalLastErrorMsg()
// *************************************************************
void  COgrLayer::get_GdalLastErrorMsg(BSTR* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CStringW s = OgrHelper::OgrString2Unicode(CPLGetLastErrorMsg());
	*pVal = W2BSTR(s);
	return ;
}

// *************************************************************
//		DynamicLoading()
// *************************************************************
void  COgrLayer::get_DynamicLoading(VARIANT_BOOL* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _dynamicLoading;
	return ;
}
void  COgrLayer::put_DynamicLoading(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_dynamicLoading = newVal;
	if (newVal) {
		ForceCreateShapefile();
	}
	return ;
}

// *************************************************************
//		MaxFeatureCount()
// *************************************************************
void  COgrLayer::get_MaxFeatureCount(LONG* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _loader.GetMaxCacheCount();
	return ;
}
void  COgrLayer::put_MaxFeatureCount(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_loader.SetMaxCacheCount(newVal <= 0 ? m_globalSettings.ogrLayerMaxFeatureCount : newVal);
	return ;
}

// *************************************************************
//		HasStyleTable()
// *************************************************************
bool COgrLayer::HasStyleTable()
{
	if (!CheckState()) return false;
	return OgrStyleHelper::HasStyleTable(_dataset, GetLayerName());
}

// *************************************************************
//		GetLayerName()
// *************************************************************
CStringW COgrLayer::GetLayerName()
{
	return OgrHelper::OgrString2Unicode(_layer->GetName());
}

// *************************************************************
//		GetStyleTableName()
// *************************************************************
CStringW COgrLayer::GetStyleTableName()
{
	return OgrStyleHelper::GetStyleTableName(GetLayerName());
}

// *************************************************************
//		SupportsStyles()
// *************************************************************
void  COgrLayer::get_SupportsStyles(VARIANT_BOOL* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = VARIANT_FALSE;

	if (!CheckState()) return ;

	if (_sourceType == ogrQuery) 
	{
		ErrorMessage(tkOGR_NO_STYLE_FOR_QUERIES);
		return ;
	}

	if (HasStyleTable()){
		*pVal = VARIANT_TRUE;
		return ;
	}
	
	OgrStyleHelper::CreateStyleTable(_dataset, GetLayerName());
	return ;
}

// *************************************************************
//		SaveStyle()
// *************************************************************
void  COgrLayer::SaveStyle(BSTR Name, CStringW xml, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = VARIANT_FALSE;
	if (!CheckState()) return ;

	if (!_shapefile) {
		ErrorMessage(tkNO_OGR_DATA_WAS_LOADED);
		return ;
	}

	VARIANT_BOOL vb;
	get_SupportsStyles(&vb);
	if (!vb) {
		return ;
	}

	RemoveStyle(Name, &vb);
	CPLErrorReset();

	CStringW styleName = OLE2W(Name);

	bool result = OgrStyleHelper::SaveStyle(_dataset, xml, GetLayerName(), styleName);
	*retVal = result ? VARIANT_TRUE : VARIANT_FALSE;
	return ;
}

// *************************************************************
//		StyleCount()
// *************************************************************
void  COgrLayer::GetNumStyles(LONG* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = -1;

	if (!CheckState()) return ;

	CStringW sql;
	sql.Format(L"SELECT COUNT(*) FROM %s WHERE layername = '%s'", GetStyleTableName(), GetLayerName());

	CPLErrorReset();
	OGRLayer* layer = _dataset->ExecuteSQL(OgrHelper::String2OgrString(sql), NULL, NULL);
	if (layer) {
		layer->ResetReading();
		OGRFeature* ft = layer->GetNextFeature();
		if (ft) {
			*pVal = ft->GetFieldAsInteger(0);
			OGRFeature::DestroyFeature(ft);
		}
		_dataset->ReleaseResultSet(layer);
	}
	return ;
}

// *************************************************************
//		StyleName()
// *************************************************************
void  COgrLayer::get_StyleName(LONG styleIndex, BSTR* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!CheckState()) return ;

	CStringW sql;
	sql.Format(L"SELECT styleName FROM %s WHERE layername = '%s'", GetStyleTableName(), GetLayerName());

	bool found = false;
	CPLErrorReset();
	OGRLayer* layer = _dataset->ExecuteSQL(OgrHelper::String2OgrString(sql), NULL, NULL);
	if (layer) {
		layer->ResetReading();
		OGRFeature* ft = NULL;
		int count = 0;
		while ((ft = layer->GetNextFeature()) != NULL)
		{
			if (count == styleIndex) {
				CStringW name = OgrHelper::OgrString2Unicode(ft->GetFieldAsString(0));
				*pVal = W2BSTR(name);
				found = true;
			}
			count++;
			OGRFeature::DestroyFeature(ft);
		}
		_dataset->ReleaseResultSet(layer);
	}

	if (!found) {
		*pVal = SysAllocString(L"");
	}
	return ;
}


// *************************************************************
//		LoadStyleXML()
// *************************************************************
CStringW COgrLayer::LoadStyleXML(CStringW name)
{
	if (!CheckState()) return L"";
	CSingleLock lock(&_loader.ProviderLock);
	lock.Lock();
	return OgrStyleHelper::LoadStyle(_dataset, GetStyleTableName(), GetLayerName(), name);
}

// *************************************************************
//		ClearStyles()
// *************************************************************
void  COgrLayer::ClearStyles(VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!CheckState() || !HasStyleTable())
	{
		*retVal = VARIANT_TRUE;
		return ;
	}
		
	USES_CONVERSION;
	CStringW sql;
	sql.Format(L"DELETE FROM %s WHERE layername = '%s'", GetStyleTableName(), GetLayerName());

	CPLErrorReset();
	_dataset->ExecuteSQL(OgrHelper::String2OgrString(sql), NULL, NULL);
	*retVal = CPLGetLastErrorNo() == OGRERR_NONE ? VARIANT_TRUE : VARIANT_FALSE;

	return ;
}

// *************************************************************
//		RemoveStyle()
// *************************************************************
void  COgrLayer::RemoveStyle(BSTR styleName, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = VARIANT_FALSE;

	if (!CheckState()) return ;

	CStringW name = OLE2W(styleName);
	bool result = OgrStyleHelper::RemoveStyle(_dataset, GetStyleTableName(), GetLayerName(), name);
	*retVal = result ? VARIANT_TRUE : VARIANT_FALSE;
	return ;
}

// *************************************************************
//		LabelExpression()
// *************************************************************
void  COgrLayer::get_LabelExpression(BSTR* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	USES_CONVERSION;
	*pVal = W2BSTR(_loader.LabelExpression);
	return ;
}
void  COgrLayer::put_LabelExpression(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	USES_CONVERSION;
	_loader.LabelExpression = OLE2W(newVal);
	return ;
}

// *************************************************************
//		LabelPosition()
// *************************************************************
void  COgrLayer::get_LabelPosition(tkLabelPositioning* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _loader.LabelPosition;
	return ;
}
void  COgrLayer::put_LabelPosition(tkLabelPositioning newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_loader.LabelPosition = newVal;
	return ;
}

// *************************************************************
//		LabelOrientation()
// *************************************************************
void  COgrLayer::get_LabelOrientation(tkLineLabelOrientation* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _loader.LabelOrientation;
	return ;
}
void  COgrLayer::put_LabelOrientation(tkLineLabelOrientation newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_loader.LabelOrientation = newVal;
	return ;
}

// *************************************************************
//		GetFieldValues()
// *************************************************************
void COgrLayer::GetFieldValues(OGRFieldType fieldType, BSTR& fieldName, vector<VARIANT*>& values)
{
	if (_sourceType == ogrDbTable || _sourceType == ogrFile) 
	{
		// load only the necessary column
		CStringW sql;
		sql.Format(L"SELECT %s FROM %s;", fieldName, GetLayerName());
		OGRLayer* layer = _dataset->ExecuteSQL(OgrHelper::String2OgrString(sql), NULL, NULL);
		if (layer)
		{
			OgrHelper::GetFieldValues(layer, static_cast<int>(_layer->GetFeatureCount()), fieldType, values, _globalCallback);
			_dataset->ReleaseResultSet(layer);
		}
	}
	else
	{
		// the column can be a computed one, so have to load feature as a whole
		OgrHelper::GetFieldValues(_layer, static_cast<int>(_layer->GetFeatureCount()), fieldType, values, _globalCallback);
	}
}

// *************************************************************
//		GenerateCategories()
// *************************************************************
void  COgrLayer::GenerateCategories(BSTR FieldName, tkClassificationType classificationType, 
				long numClasses, tkMapColor colorStart, tkMapColor colorEnd, 
				tkColorSchemeType schemeType, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = VARIANT_FALSE;
	if (!CheckState()) return ;

	OGRFeatureDefn* fields = _layer->GetLayerDefn();
	int fieldIndex = fields->GetFieldIndex(OgrHelper::Bstr2OgrString(FieldName));
	if (fieldIndex == -1) {
		ErrorMessage(tkOGR_INVALID_FIELD_NAME);
		return ;
	}

	CStringW fid = OgrHelper::OgrString2Unicode(_layer->GetFIDColumn());
	bool hasFid = fid.GetLength() > 0;

	CShapefile* sf = NULL;
	GetBuffer(&sf);
	if (!sf) {
		ErrorMessage(tkOGR_NO_SHAPEFILE);
		return ;
	}

	OGRFieldDefn* fld = fields->GetFieldDefn(fieldIndex);
	OGRFieldType ogrType = fld->GetType();
	FieldType fieldType = OgrHelper::GetFieldType(ogrType);
	
	vector<VARIANT*>* values = new vector<VARIANT*>();
	GetFieldValues(ogrType, FieldName, *values);
	
	long errorCode = tkNO_ERROR;
	USES_CONVERSION;
	vector<CategoriesData>* categories = FieldClassification::GenerateCategories(OLE2A(FieldName), 
									fieldType, *values, classificationType, numClasses, errorCode);
	
	for (size_t i = 0; i < values->size(); i++) {
		VariantClear((*values)[i]);
		delete (*values)[i];
	}
	delete values;

	if (!categories) {
		ErrorMessage(errorCode);
		return ;
	}

	CShapefileCategories* ct = NULL;
	sf->get_Categories(&ct);
	if (ct)
	{
		if (hasFid)
			fieldIndex++;   // there will be the first FID column in the data

		VARIANT_BOOL vb;
		((CShapefileCategories*)ct)->GenerateCore(categories, fieldIndex, classificationType, &vb);
		if (vb) {
			*retVal = VARIANT_TRUE;
		}

		CColorScheme* scheme = new CColorScheme();
		if (scheme) {
			scheme->SetColors2(colorStart, colorEnd);
			ct->ApplyColorScheme(schemeType, scheme);
		}

		long numShapes;
		sf->get_NumShapes(&numShapes);
		if (numShapes > 0)
		{
			ct->ApplyExpressions();
		}
		delete ct;
	}
	delete categories;
	return ;
}

// *************************************************************
//		DriverName()
// *************************************************************
void  COgrLayer::get_DriverName(BSTR* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (!CheckState())
	{
		*pVal = A2BSTR("");
		return ;
	}
	else
	{
		*pVal = A2BSTR(_dataset->GetDriverName());   // no need to convert from UTF-8: it's in ASCII
		return ;
	}
	return ;
}
