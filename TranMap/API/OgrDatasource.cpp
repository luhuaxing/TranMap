#include "stdafx.h"
#include "OgrDatasource.h"
#include "OgrHelper.h"
#include "OgrLayer.h"
#include "GeoProjection.h"
#include "OgrConverter.h"
#include "Shapefile.h"
#include "OgrStyle.h"
#include "Shape2Ogr.h"

//***********************************************************************
//*		get/put_Key()
//***********************************************************************
void  COgrDatasource::get_Key(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	*pVal = OLE2BSTR(_key);
	return ;
}
void  COgrDatasource::put_Key(BSTR newVal)
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
void COgrDatasource::ErrorMessage(long ErrorCode)
{
	_lastErrorCode = ErrorCode;
	CallbackHelper::ErrorMsg("OgrDatasource", _globalCallback, _key, ErrorMsg(_lastErrorCode));
}

void  COgrDatasource::get_LastErrorCode(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*pVal = _lastErrorCode;
	_lastErrorCode = tkNO_ERROR;
	return ;
}

void  COgrDatasource::get_ErrorMsg(long ErrorCode, BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	*pVal = A2BSTR(ErrorMsg(ErrorCode));
	return ;
}

//***********************************************************************/
//*		get/put_GlobalCallback()
//***********************************************************************/
void  COgrDatasource::get_GlobalCallback(ICallback **pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*pVal = _globalCallback;
	return ;
}

void  COgrDatasource::put_GlobalCallback(ICallback *newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	_globalCallback = newVal;
	return ;
}

// *************************************************************
//		Open()
// *************************************************************
void  COgrDatasource::Open(BSTR connectionString, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	Close();
	*retVal = VARIANT_FALSE;

	GDALDataset* ds = GdalHelper::OpenOgrDatasetW(OLE2W(connectionString), false);
	if (!ds)
	{
		// clients should extract last GDAL error
		ErrorMessage(tkFAILED_TO_OPEN_OGR_DATASOURCE);
		return ;
	}
	else
	{
		_connectionString = connectionString;
		_dataset = ds;
		*retVal = VARIANT_TRUE;
	}
	return ;
}

// *************************************************************
//		Close()
// *************************************************************
void  COgrDatasource::Close()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if (_dataset)
	{
		GDALClose(_dataset);
		_dataset = NULL;
	}
	return ;
}

// *************************************************************
//		get_LayerCount()
// *************************************************************
void  COgrDatasource::get_LayerCount(int* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = _dataset ? _dataset->GetLayerCount() : 0;
	return ;
}

// *************************************************************
//		GetLayerByName()
// *************************************************************
void  COgrDatasource::GetLayerByName(BSTR name, VARIANT_BOOL forUpdate, COgrLayer** retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = NULL;
	if (!CheckState()) return ;
	
	COgrLayer* layer = new COgrLayer();

	VARIANT_BOOL vb;
	CComBSTR bstrConnection(_connectionString);
	((COgrLayer*)layer)->OpenFromDatabase(bstrConnection, name, forUpdate, &vb);
	if (!vb)
	{
		long errorCode;
		layer->get_LastErrorCode(&errorCode);
		COgrDatasource::ErrorMessage(errorCode);
		delete layer;
	}
	else
	{
		*retVal = layer;
	}
	return ;
}

// *************************************************************
//		get_Layer()
// *************************************************************
void  COgrDatasource::GetLayer(int index, VARIANT_BOOL forUpdate, COgrLayer** retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = NULL;
	if (!COgrDatasource::CheckState()) return ;
	if (index < 0 && index >= _dataset->GetLayerCount())
	{
		COgrDatasource::ErrorMessage(tkINDEX_OUT_OF_BOUNDS);
		return ;
	}
	else
	{
		COgrLayer* layer = new COgrLayer();
		
		
		VARIANT_BOOL vb;
		CComBSTR bstrConnection(_connectionString);
		((COgrLayer*)layer)->OpenDatabaseLayer(bstrConnection, index, forUpdate, &vb);
		if (!vb)
		{
			long errorCode;
			layer->get_LastErrorCode(&errorCode);
			COgrDatasource::ErrorMessage(errorCode);
		    delete layer;
		}
		else
		{
			*retVal = layer;
		}
		return ;
	}
}

// *************************************************************
//		RunQuery()
// *************************************************************
void  COgrDatasource::RunQuery(BSTR sql, COgrLayer** retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = NULL;
	if (COgrDatasource::CheckState())
	{
		COgrLayer* layer = new COgrLayer();
		VARIANT_BOOL vb;
		CComBSTR bstr(_connectionString);
		((COgrLayer*)layer)->OpenFromQuery(bstr, sql, &vb);
		if (!vb)
		{
			long errorCode;
			layer->get_LastErrorCode(&errorCode);
			COgrDatasource::ErrorMessage(errorCode);
		    delete layer;
		}
		else
		{
			*retVal = layer;
		}
	}
	return ;
}

// *************************************************************
//		CheckState()
// *************************************************************
bool COgrDatasource::CheckState()
{
	if (!_dataset)
	{
		ErrorMessage(tkOGR_DATASOURCE_UNINITIALIZED);
		return false;
	}
	return true;
}

// *************************************************************
//		GetDriverName()
// *************************************************************
void  COgrDatasource::get_DriverName(BSTR* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if (!CheckState())
	{
		*retVal = A2BSTR("");
		return ;
	}
	else
	{
		*retVal = A2BSTR(_dataset->GetDriverName());   // no need to convert from UTF-8: it's in ASCII
		return ;
	}
}

// *************************************************************
//		GetLayerName()
// *************************************************************
void  COgrDatasource::GetLayerName(int index, BSTR* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if (CheckState())
	{
		if (index >= 0 || index < _dataset->GetLayerCount())
		{
			OGRLayer* layer = _dataset->GetLayer(index);
			if (layer)
			{
				CStringW name = OgrHelper::OgrString2Unicode(layer->GetName());
				*retVal = W2BSTR(name);
				return ;
			}
		}
		else
		{
			ErrorMessage(tkINDEX_OUT_OF_BOUNDS);
		}
	}
	*retVal = A2BSTR("");
	return ;
}

// *************************************************************
//		DeleteLayer()
// *************************************************************
void  COgrDatasource::DeleteLayer(int layerIndex, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = VARIANT_FALSE;
	if (CheckState())
	{
		if (layerIndex >= 0 || layerIndex < _dataset->GetLayerCount())
		{
			OGRErr result = _dataset->DeleteLayer(layerIndex);
			*retVal = (result == OGRERR_NONE) ? VARIANT_TRUE : VARIANT_FALSE;
		}
		else
		{
			ErrorMessage(tkINDEX_OUT_OF_BOUNDS);
		}
	}
	return ;
}

// *************************************************************
//		TestCapability()
// *************************************************************
void  COgrDatasource::TestCapability(tkOgrDSCapability capability, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = VARIANT_FALSE;
	if (!CheckState()) return ;
	int val = _dataset->TestCapability(OgrHelper::GetDsCapabilityString(capability));
	*retVal = val ? VARIANT_TRUE : VARIANT_FALSE;
	return ;
}

// *************************************************************
//		CreateLayer()
// *************************************************************
void  COgrDatasource::CreateLayer(BSTR layerName, ShpfileType shpType, CGeoProjection* projection, BSTR creationOptions, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = VARIANT_FALSE;
	if (!CheckState()) return ;
	
	if (shpType == SHP_NULLSHAPE)
	{
		ErrorMessage(tkUNSUPPORTED_SHAPEFILE_TYPE);
		return ;
	}

	OGRSpatialReference* ref = projection ? ((CGeoProjection*)projection)->get_SpatialReference() : NULL;

	char** options = ParseLayerCreationOptions(creationOptions);
	bool multiPart = CSLFetchBoolean(options, "MULTI_PART", 1) != 0;

	OGRLayer* layer = _dataset->CreateLayer(OgrHelper::Bstr2OgrString(layerName), ref,
		OgrConverter::ShapeType2GeometryType(shpType, multiPart), options);
	CSLDestroy(options);

	*retVal = layer != NULL ? VARIANT_TRUE : VARIANT_FALSE;
	return ;
}

// *************************************************************
//		LayerIndexByName()
// *************************************************************
void  COgrDatasource::LayerIndexByName(BSTR layerName, int* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = -1;
	if (!CheckState()) return ;

	CStringA name = OgrHelper::Bstr2OgrString(layerName);
	for (int i = 0; i < _dataset->GetLayerCount(); i++)
	{
		OGRLayer* layer = _dataset->GetLayer(i);
		if (layer && _stricmp(layer->GetName(), name) == 0)
		{
			*retVal = i;
			break;
		}
	}
	return ;
}

// *************************************************************
//		ImportShapefile()
// *************************************************************
void  COgrDatasource::ImportShapefile(CShapefile* shapefile, BSTR newLayerName, BSTR creationOptions, 
	tkShapeValidationMode validationMode, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = VARIANT_FALSE;
	if (!CheckState()) return ;

	if (!shapefile)
	{
		ErrorMessage(tkUNEXPECTED_NULL_PARAMETER);
		return ;
	}

	CStringA name = OgrHelper::Bstr2OgrString(newLayerName);
	if (name.GetLength() == 0)
	{
		ErrorMessage(tkINVALID_LAYER_NAME);
		return ;
	}

	if (validationMode != NoValidation)
	{
		CShapeValidationInfo* info = ((CShapefile*)shapefile)->ValidateInputCore(shapefile, "ImportShapefile", "shapefile", VARIANT_FALSE, validationMode, "OgrDatasource");
		if (info)
		{
			tkShapeValidationStatus status;
			info->get_Status(&status);
			delete info;
			if (status == OperationAborted)
			{
				ErrorMessage(tkOGR_ABORTED_INVALID_SHAPES);
				goto cleaning;
			}
		}
	}

	// creating empty layer
	CGeoProjection* gp = NULL;
	ShpfileType shpType;
	shapefile->get_ShapefileType(&shpType);
	shapefile->get_GeoProjection(&gp);
	OGRSpatialReference* ref = gp ? ((CGeoProjection*)gp)->get_SpatialReference() : NULL;
	
	char** options = ParseLayerCreationOptions(creationOptions);
	bool multiPart = CSLFetchBoolean(options, "MW_MULTI_PART", 1) != 0;
	bool vacuum = CSLFetchBoolean(options, "MW_POSTGIS_VACUUM", 1) != 0;
	bool saveStyles = CSLFetchBoolean(options, "MW_SAVE_STYLE", 1) != 0;

	OGRLayer* layer = _dataset->CreateLayer(name, ref, OgrConverter::ShapeType2GeometryType(shpType, multiPart), options);
	
	delete gp;
	CSLDestroy(options);

	if (!layer)
	{
		ErrorMessage(tkFAILED_TO_CREATE_OGR_LAYER);
		goto cleaning;
	}

	// copying fields and shapes
	bool result = Shape2Ogr::Shapefile2OgrLayer(shapefile, layer, m_globalSettings.saveOgrLabels, _globalCallback);
	if (result)
	{
		layer->SyncToDisk();
	}

	*retVal = result ? VARIANT_TRUE : VARIANT_FALSE;

	// saving current style as a default one
	if (saveStyles)
	{
		CStringW layerName = OLE2W(newLayerName);
		if (OgrStyleHelper::SupportsStyles(_dataset, layerName)) 
		{
			OgrStyleHelper::RemoveStyle(_dataset, OgrStyleHelper::GetStyleTableName(layerName), layerName, L"" );
			OgrStyleHelper::SaveStyle(_dataset, shapefile, layerName, L"");
		}
	}

	// run vacuum command for PostGis unless user canceled it explicitly
	if (vacuum)
	{
		if (OgrHelper::IsPostGisDatasource(_dataset))
		{
			// if it fails just ignore it as it's not critical
			name = "VACUUM FULL ANALYZE " + name;
			_dataset->ExecuteSQL(name, NULL, NULL);
		}
	}

cleaning:
	((CShapefile*)shapefile)->ClearValidationList();
	return ;
}

// *************************************************************
//		ExecuteSQL()
// *************************************************************
void  COgrDatasource::ExecuteSQL(BSTR sql, BSTR* errorMessage, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	CPLErrorReset();
	
	OGRLayer* lyr = _dataset->ExecuteSQL(OgrHelper::Bstr2OgrString(sql), NULL, NULL);
	if (lyr)_dataset->ReleaseResultSet(lyr);

	if (CPLGetLastErrorNo() != OGRERR_NONE)
	{
		CStringW s = OgrHelper::OgrString2Unicode(CPLGetLastErrorMsg());
		*errorMessage = W2BSTR(s);
		*retVal = VARIANT_FALSE;
		return ;
	}
	else
	{
		*errorMessage = A2BSTR("");
		*retVal = VARIANT_TRUE;
		return ;
	}
}

// *************************************************************
//		ParseLayerCreationOptions()
// *************************************************************
char** COgrDatasource::ParseLayerCreationOptions(BSTR creationOptions)
{
	USES_CONVERSION;
	CString options = OLE2A(creationOptions);
	char** list = NULL;
	int pos = 0;
	do
	{
		CString s = options.Tokenize(";", pos);
		list = CSLAddString(list, s);
	} while (pos < options.GetLength() - 1);
	return list;
}

// *************************************************************
//		get_DriverMetadata()
// *************************************************************
void  COgrDatasource::get_DriverMetadata(tkGdalDriverMetadata metadata, BSTR* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	
	if (CheckState())
	{
		GDALDriver* driver = _dataset->GetDriver();
		if (driver)
		{
			char* val = const_cast<char*>(driver->GetMetadataItem(GdalHelper::GetMetadataNameString(metadata)));
			*retVal = A2BSTR(val);		// no need to treat it as utf-8: it's in ASCII
			return ;
		}
	}
	*retVal = A2BSTR("");
	return ;
}

// *************************************************************
//		get_DriverMetadataCount()
// *************************************************************
void  COgrDatasource::get_DriverMetadataCount(int* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if (CheckState())
	{
		GDALDriver* driver = _dataset->GetDriver();
		if (driver)
		{
			char** data = driver->GetMetadata();
			*retVal = CSLCount(data);
			return ;
		}
	}
	*retVal = 0;
	return ;
}

// *************************************************************
//		get_DriverMetadata()
// *************************************************************
void  COgrDatasource::get_DriverMetadataItem(int metadataIndex, BSTR* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;

	if (CheckState())
	{
		GDALDriver* driver = _dataset->GetDriver();
		if (driver)
		{
			char** data = driver->GetMetadata();
			if (metadataIndex < 0 || metadataIndex >= CSLCount(data))
			{
				ErrorMessage(tkINDEX_OUT_OF_BOUNDS);
				return ;
			}
			else
			{
				char* item = const_cast<char*>(CSLGetField(data, metadataIndex));
				*retVal = A2BSTR(item);    // no need to treat it as utf-8: it's in ASCII
				return ;
			}
		}
	}
	*retVal = A2BSTR("");
	return ;
}

// *************************************************************
//		get_GdalLastErrorMsg()
// *************************************************************
void  COgrDatasource::get_GdalLastErrorMsg(BSTR* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CStringW s = OgrHelper::OgrString2Unicode(CPLGetLastErrorMsg());
	*pVal = W2BSTR(s);
	return ;
}