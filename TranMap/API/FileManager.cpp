// FileManager.cpp : Implementation of CFileManager
#include "stdafx.h"
#include "FileManager.h"
#include "GridManager.h"
#include "Grid.h"
#include "OgrDatasource.h"
#include "OgrHelper.h"
#include "Image.h"

//***********************************************************************
//*		get/put_Key()
//***********************************************************************
void  CFileManager::get_Key(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	*pVal = OLE2BSTR(_key);
	return ;
}
void  CFileManager::put_Key(BSTR newVal)
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
void CFileManager::ErrorMessage(long ErrorCode)
{
	_lastErrorCode = ErrorCode;
	CallbackHelper::ErrorMsg("FileManager", _globalCallback, _key, ErrorMsg(_lastErrorCode));
}

void  CFileManager::get_LastErrorCode(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*pVal = _lastErrorCode;
	_lastErrorCode = tkNO_ERROR;
	return ;
}

void  CFileManager::get_ErrorMsg(long ErrorCode, BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	*pVal = A2BSTR(ErrorMsg(ErrorCode));
	return ;
}

//***********************************************************************/
//*		get/put_GlobalCallback()
//***********************************************************************/
void  CFileManager::get_GlobalCallback(ICallback **pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*pVal = _globalCallback;
	return ;
}

void  CFileManager::put_GlobalCallback(ICallback *newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	_globalCallback = newVal;
	return ;
}

//****************************************************************
//			get_OpenStrategyCore()
//****************************************************************
tkFileOpenStrategy CFileManager::get_OpenStrategyCore(BSTR Filename)
{
	// shapefile
	CStringW filenameW = OLE2W(Filename);
	if (IsShapefile(Filename))
	{
		return tkFileOpenStrategy::fosVectorLayer;
	}
	
	// GDAL
	GdalSupport support = GdalHelper::TryOpenWithGdal(filenameW);
	if (support != GdalSupport::GdalSupportNone) {
		if (support == GdalSupportRgb)
		{
			return tkFileOpenStrategy::fosRgbImage;
		}
		else
		{
			return GridManager::NeedProxyForGrid(filenameW, m_globalSettings.gridProxyMode) ? tkFileOpenStrategy::fosProxyForGrid : tkFileOpenStrategy::fosDirectGrid;
		}
	}

	// it can be binary grid, handled by our own classes
	USES_CONVERSION;
	GridManager gm;
	DATA_TYPE dType = gm.getGridDataType(W2A(filenameW), USE_EXTENSION);
	if (dType != INVALID_DATA_TYPE)
	{
		return tkFileOpenStrategy::fosProxyForGrid;
	}

	// OGR vector
	tkFileOpenStrategy strategy = fosNotSupported;
	GDALDataset* dt = GdalHelper::OpenOgrDatasetW(filenameW, false);
	if (dt)
	{
		int layerCount = dt->GetLayerCount();
		if (layerCount > 0) {
			strategy = layerCount == 1? fosVectorLayer : fosVectorDatasource;
		}
		dt->Dereference();
		delete dt;
	}
	return strategy;
}

//****************************************************************
//			get_IsSupportedBy()
//****************************************************************
void  CFileManager::get_IsSupportedBy(BSTR Filename, tkSupportType supportType, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	switch(supportType)
	{
		case stGdal:
			*retVal = GdalHelper::CanOpenAsGdalRaster(OLE2W(Filename)) ? VARIANT_TRUE: VARIANT_FALSE;
			return ;
		case stGdalOverviews:
			*retVal = GdalHelper::SupportsOverviews(OLE2W(Filename), _globalCallback) ? VARIANT_TRUE : VARIANT_FALSE;
			return ;
	}
	*retVal = VARIANT_FALSE;
	return ;
}

//****************************************************************
//			get_IsSupported()
//****************************************************************
void  CFileManager::get_IsSupported(BSTR Filename, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	tkFileOpenStrategy strategy = get_OpenStrategyCore(Filename);
	*retVal = strategy != fosNotSupported ? VARIANT_TRUE: VARIANT_FALSE;
	return ;
}

//****************************************************************
//			get_LastOpenStrategy()
//****************************************************************
void  CFileManager::get_LastOpenStrategy(tkFileOpenStrategy* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = _lastOpenStrategy;
	return ;
}

//****************************************************************
//			get_LastOpenFilename()
//****************************************************************
void  CFileManager::get_LastOpenFilename(BSTR* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	*retVal = W2BSTR(_lastOpenFilename);
	return ;
}

//****************************************************************
//			get_LastOpenIsSuccess()
//****************************************************************
void  CFileManager::get_LastOpenIsSuccess(VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = _lastOpenIsSuccess ? VARIANT_TRUE : VARIANT_FALSE;
	return ;
}

//****************************************************************
//			get_IsRgbImage()
//****************************************************************
void  CFileManager::get_IsRgbImage(BSTR Filename, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	*retVal = VARIANT_FALSE;
	GDALDataset* dt = GdalHelper::OpenRasterDatasetW(OLE2W(Filename));
	if (dt)
	{
		*retVal = GdalHelper::IsRgb(dt) ? VARIANT_TRUE: VARIANT_FALSE;
		GdalHelper::CloseDataset(dt);
	}
	return ;
}

//****************************************************************
//			get_IsGrid()
//****************************************************************
void  CFileManager::get_IsGrid(BSTR Filename, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = VARIANT_FALSE;
	
	CStringW name = OLE2W(Filename);
	GdalSupport support = GdalHelper::TryOpenWithGdal(name);
	if (support != GdalSupport::GdalSupportNone) {
		*retVal = support != GdalSupportRgb ? VARIANT_TRUE: VARIANT_FALSE;
	}
	else
	{
		// it can be binary grid, handled by our own driver
		USES_CONVERSION;
		GridManager gm;
		DATA_TYPE dType = gm.getGridDataType( W2A(name), USE_EXTENSION );
		if (dType != INVALID_DATA_TYPE)
		{
			*retVal = VARIANT_TRUE;
		}
	}
	return ;
}

//****************************************************************
//			get_IsVectorLayer()
//****************************************************************
void  CFileManager::get_IsVectorLayer(BSTR Filename, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = VARIANT_FALSE;

	CStringW filenameW = OLE2W(Filename);
	if (IsShapefile(filenameW)) 
	{
		*retVal = VARIANT_TRUE;
		return ;
	}

	if (GdalHelper::CanOpenAsOgrDataset(filenameW)) {
		*retVal = VARIANT_TRUE;
		return ;
	}
	return ;
}

//****************************************************************
//			get_OpenStrategy()
//****************************************************************
void  CFileManager::get_OpenStrategy(BSTR Filename, tkFileOpenStrategy* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = get_OpenStrategyCore(Filename);
	return ;
}

//****************************************************************
//			get_CanOpenAs()
//****************************************************************
void  CFileManager::get_CanOpenAs(BSTR Filename, tkFileOpenStrategy strategy, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if (strategy == fosDirectGrid)
	{
		// direct grids must be supported by GDAL
		USES_CONVERSION;
		*retVal = GdalHelper::CanOpenAsGdalRaster(OLE2W(Filename));
		return ;
	}
		
	tkFileOpenStrategy format = get_OpenStrategyCore(Filename);
	switch(format) {
		case fosNotSupported:
			*retVal = VARIANT_FALSE;
			break;
		case fosRgbImage:
			*retVal = strategy == fosRgbImage || strategy == fosDirectGrid || strategy == fosProxyForGrid;
			break;
		case fosVectorLayer:
			*retVal = strategy == fosVectorLayer ? VARIANT_TRUE : VARIANT_FALSE;
			break;
		case fosProxyForGrid:
		case fosDirectGrid:
			*retVal = strategy == fosProxyForGrid;
			break;
	}
	return ;
}

//****************************************************************
//			Open()
//****************************************************************
void  CFileManager::Open(BSTR Filename, tkFileOpenStrategy openStrategy, ICallback* callback, ILayer** retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = NULL;
	
	if (callback) {
		put_GlobalCallback(callback);
	}

	CStringW filenameW = OLE2W(Filename);

	_lastOpenIsSuccess = false;
	_lastOpenFilename = filenameW;
	_lastOpenStrategy = openStrategy;

	USES_CONVERSION;
	if (!Utility::FileExistsW(filenameW))
	{
		ErrorMessage(tkFILE_NOT_EXISTS);
		return ;
	}

	if (openStrategy == fosAutoDetect) {
		openStrategy = get_OpenStrategyCore(Filename);
	}
	
	switch(openStrategy) 
	{
		case fosNotSupported:
			ErrorMessage(tkUNSUPPORTED_FORMAT);
			break;
		case fosVectorLayer:
			if (IsShapefile(filenameW)) {
				OpenShapefile(Filename, NULL, (CShapefile**)retVal);
			}
			else {
				OpenVectorLayer(Filename, SHP_NULLSHAPE, VARIANT_FALSE, (COgrLayer**)retVal);
			}
			break;
		case fosVectorDatasource:
			OpenVectorDatasource(Filename, (COgrDatasource**)retVal);
			break;
		default:
			OpenRaster(Filename, openStrategy, NULL, (CImageClass**)retVal);
	}
	return ;
}

//****************************************************************
//			OpenVectorLayer()
//****************************************************************
void  CFileManager::OpenVectorLayer(BSTR Filename, ShpfileType preferedShapeType, VARIANT_BOOL forUpdate, COgrLayer** retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = NULL;

	CStringW filenameW = OLE2W(Filename);
	_lastOpenFilename = filenameW;
	_lastOpenStrategy = fosVectorLayer;

	COgrDatasource* ds = new COgrDatasource();
	if (!ds)  return ;
		
	VARIANT_BOOL vb;
	ds->Open(Filename, &vb);		// error will be reported in the class

	if (!vb) return ;

	int layerCount;
	ds->get_LayerCount(&layerCount);
	if (layerCount == 0)
	{
		ds->Close();
		ErrorMessage(tkOGR_DATASOURCE_EMPTY);
		return ;
	}
	
	COgrLayer* layer = NULL;
	int layerIndex = 0;
	if (layerCount > 1 || preferedShapeType != SHP_NULLSHAPE)
	{
		// choose layer with proper type (for KML for example)
		layer = OgrHelper::ChooseLayerByShapeType(ds, preferedShapeType, forUpdate);
	}

	if (!layer) {
		ds->GetLayer(0, forUpdate, &layer);		// simply grab the first one
	}

	if (layer) {
		*retVal = layer;
		_lastOpenIsSuccess = true;
	}
	
	ds->Close();
	return ;
}

//****************************************************************
//			OpenShapefile()
//****************************************************************
void  CFileManager::OpenShapefile(BSTR Filename, ICallback* callback, CShapefile** retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = NULL;

	CStringW filenameW = OLE2W(Filename);
	_lastOpenFilename = filenameW;
	_lastOpenStrategy = fosVectorLayer;

	if (callback) {
		put_GlobalCallback(callback);
	}

	if (!Utility::FileExistsW(filenameW))
	{
		ErrorMessage(tkFILE_NOT_EXISTS);
		return ;
	}

	VARIANT_BOOL vb;
	if (!IsShapefile(filenameW))
		return ;
	
	CShapefile* sf = new CShapefile();
	sf->Open(Filename, _globalCallback, &vb);
	if (!vb)
	{
		sf->get_LastErrorCode(&_lastErrorCode);
		ErrorMessage(_lastErrorCode);
		delete sf;
	}
	else
	{
		_lastOpenIsSuccess = true;
		*retVal = sf;
	}
	return ;
}

//****************************************************************
//			OpenRaster()
//****************************************************************
void  CFileManager::OpenRaster(BSTR Filename, tkFileOpenStrategy openStrategy, ICallback* callback, CImageClass** retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = NULL;

	_lastOpenFilename = Filename;
	_lastOpenStrategy = openStrategy;

	if (callback) {
		put_GlobalCallback(callback);
	}

	if (!Utility::FileExistsW(OLE2W(Filename)))
	{
		ErrorMessage(tkFILE_NOT_EXISTS);
		return ;
	}

	VARIANT_BOOL vb;
	if (openStrategy == fosAutoDetect) {
		openStrategy = get_OpenStrategyCore(Filename);
	}

	_lastOpenStrategy = openStrategy;

	switch(openStrategy) 
	{
		case fosNotSupported:
			ErrorMessage(tkUNSUPPORTED_FORMAT);
			break;
		case fosRgbImage:
			{
				CImageClass* img = new CImageClass();
				
				if (img)
				{
					img->Open( Filename, ImageType::USE_FILE_EXTENSION, VARIANT_FALSE, _globalCallback, &vb );
					if (!vb)
					{
						img->get_LastErrorCode(&_lastErrorCode);
						ErrorMessage(_lastErrorCode);
						delete img;
						img = NULL;
					}
					else
					{
						// check that is is actually RGB image
						img->get_IsRgb(&vb);
						if (!vb) {
							ErrorMessage(tkINVALID_OPEN_STRATEGY);
							img->Close(&vb);
							delete img;
							img = NULL;
						}
						_lastOpenIsSuccess = true;
						*retVal = img;
					}
				}
			}
			break;
		case fosDirectGrid:
		case fosProxyForGrid:
			{
			    CGrid* grid = new CGrid();
				if (grid)
				{
					m_globalSettings.forceReadOnlyModeForGdalRasters = true;
					
					// TODO: choose inRam mode
					grid->Open(Filename, GridDataType::UnknownDataType, VARIANT_FALSE, GridFileType::UseExtension, _globalCallback, &vb);

					m_globalSettings.forceReadOnlyModeForGdalRasters = false;

					if (!vb) {
						grid->get_LastErrorCode(&_lastErrorCode);
						ErrorMessage(_lastErrorCode);
					}
					else
					{
						PredefinedColorScheme coloring = m_globalSettings.GetGridColorScheme();
						CGridColorScheme* scheme = NULL;
						
						grid->RetrieveOrGenerateColorScheme(tkGridSchemeRetrieval::gsrAuto, 
															tkGridSchemeGeneration::gsgGradient, coloring, &scheme);
						
						tkGridProxyMode mode = openStrategy == fosDirectGrid ? gpmNoProxy : gpmUseProxy;
						
						CImageClass* img = NULL;
						grid->OpenAsImage(scheme, mode, _globalCallback, &img);
						
						if (!img) {
							// TODO: perhaps use another mode on failure
							grid->get_LastErrorCode(&_lastErrorCode);
							ErrorMessage(_lastErrorCode);
						}
						else {
							_lastOpenIsSuccess = true;
							*retVal = img;
						}
						grid->Close(&vb);
					}
				}
			}
			break;
	}
	return ;
}

#pragma region Projection
//****************************************************************
//			get_HasProjection()
//****************************************************************
void  CFileManager::get_HasProjection(BSTR Filename, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = VARIANT_FALSE;
	// TODO: implement
	return ;
}

//****************************************************************
//			get_GeoProjection()
//****************************************************************
void  CFileManager::get_GeoProjection(BSTR Filename, CGeoProjection** retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = NULL;
	// TODO: implement
	return ;
}

//****************************************************************
//			get_IsSameProjection()
//****************************************************************
void  CFileManager::get_IsSameProjection(BSTR Filename, CGeoProjection* projection, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = VARIANT_FALSE;
	// TODO: implement
	return ;
}
#pragma endregion

//****************************************************************
//			DeleteDatasource()
//****************************************************************
void  CFileManager::DeleteDatasource(BSTR Filename, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = NULL;
	return ;
}

//****************************************************************
//			HasGdalOverviews()
//****************************************************************
void  CFileManager::get_HasGdalOverviews(BSTR Filename, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = GdalHelper::HasOverviews(OLE2W(Filename)) ? VARIANT_TRUE: VARIANT_FALSE;
	return ;
}

//****************************************************************
//			ClearGdalOverviews()
//****************************************************************
void  CFileManager::ClearGdalOverviews(BSTR Filename, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = GdalHelper::RemoveOverviews(OLE2W(Filename)) ? VARIANT_TRUE: VARIANT_FALSE;
	return ;
}

//****************************************************************
//			CreateGdalOverviews()
//****************************************************************
void  CFileManager::BuildGdalOverviews(BSTR Filename, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = GdalHelper::BuildOverviewsIfNeeded(OLE2W(Filename), true, _globalCallback) ? VARIANT_TRUE : VARIANT_FALSE;
	return ;
}

//****************************************************************
//			NeedsGdalOverviews()
//****************************************************************
void  CFileManager::get_NeedsGdalOverviews(BSTR Filename, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = NULL;
	return ;
}

//****************************************************************
//			RemoveProxyImages()
//****************************************************************
void  CFileManager::RemoveProxyForGrid(BSTR Filename, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	VARIANT_BOOL isGrid;
	this->get_IsGrid(Filename, &isGrid);
	if (isGrid) {
		*retVal = GridManager::RemoveImageProxy(OLE2W(Filename)) ? VARIANT_TRUE: VARIANT_FALSE;
	}
	*retVal = VARIANT_FALSE;
	return ;
}

//****************************************************************
//			HasValidProxyForGrid()
//****************************************************************
void  CFileManager::get_HasValidProxyForGrid(BSTR Filename, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = GridManager::HasValidProxy(OLE2W(Filename));
	return ;
}

//****************************************************************
//			OpenFromDatabase()
//****************************************************************
void  CFileManager::OpenFromDatabase(BSTR connectionString, BSTR layerNameOrQuery, COgrLayer** retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = NULL;
	
	COgrDatasource* source = NULL;
	source = new COgrDatasource();
	//ComHelper::CreateInstance(idOgrDatasource, (IDispatch**)&source);
	VARIANT_BOOL vb;
	source->Open(connectionString, &vb);
	if (!vb)
	{
		long errorCode;
		source->get_LastErrorCode(&errorCode);
		ErrorMessage(errorCode);
		return ;
	}
	
	source->GetLayerByName(layerNameOrQuery, VARIANT_FALSE, retVal);
	if (*retVal == NULL)
	{
		source->RunQuery(layerNameOrQuery, retVal);
	}
	source->Close();
	delete source;
	return ;
}

//****************************************************************
//			IsShapefile()
//****************************************************************
bool CFileManager::IsShapefile(CStringW filename)
{
	return Utility::EndsWith(filename, L"shp");
}

//****************************************************************
//			OpenVectorDatasource()
//****************************************************************
void  CFileManager::OpenVectorDatasource(BSTR Filename, COgrDatasource** retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	*retVal = NULL;

	CStringW filenameW = OLE2W(Filename);
	_lastOpenFilename = filenameW;
	_lastOpenStrategy = fosVectorLayer;	    // TODO: perhaps fosVectorDatasource is more appropriate

	COgrDatasource* ds = new COgrDatasource();
	if (!ds)  return ;

	VARIANT_BOOL vb;
	ds->Open(Filename, &vb);		// error will be reported in the class
	
	if (vb) {
		*retVal = ds;
	}
	return ;
}

//****************************************************************
//			GetFilter()
//****************************************************************
CString CFileManager::GetFilter(OpenFileDialogFilter filter)
{
	switch (filter)
	{
		case FilterImage:
			return "Image Formats|hdr.adf;*.asc;*.bt;*.bil;*.bmp;*.dem;*.ecw;*.img;*.gif;*.map;*.jp2;*.jpg;*.sid;*.pgm;*.pnm;*.png;*.ppm;*.vrt;*.tif;*.ntf|";
		case FilterGrid:
			return "Grid Formats|sta.adf;*.bgd;*.asc;*.tif;????cel0.ddf;*.arc;*.aux;*.pix;*.dem;*.dhm;*.dt0;*.img;*.dt1;*.bil;*.nc|";
		case FilterOgr:
			// TODO: add more formats; this list is just for a start;
			return "Vector formats|*.dgn;*.dxf;*.gml;*.kml;*.mif;*.tab;*.shp|";
		case FilterShapefile:
			return "ESRI Shapefiles (*.shp)|*.shp|";
		case FilterAll:
			return "All files|*.*";
	}
	return "";
}

//****************************************************************
//			get_CdlgFilter()
//****************************************************************
void  CFileManager::get_CdlgFilter(BSTR* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString s = GetFilter(FilterShapefile) + 
				GetFilter(FilterOgr) +
				GetFilter(FilterImage) +
				GetFilter(FilterGrid) +
				GetFilter(FilterAll);
	*pVal = A2BSTR(s);
	return ;
}

//****************************************************************
//			get_CdlgRasterFilter()
//****************************************************************
void  CFileManager::get_CdlgRasterFilter(BSTR* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString s = GetFilter(FilterImage) +
				GetFilter(FilterGrid) +
				GetFilter(FilterAll);
	*pVal = A2BSTR(s);
	return ;
}

//****************************************************************
//			get_CdlgVectorFilter()
//****************************************************************
void  CFileManager::get_CdlgVectorFilter(BSTR* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString s = GetFilter(FilterShapefile) +
				GetFilter(FilterOgr) +
				GetFilter(FilterAll);
	*pVal = A2BSTR(s);
	return ;
}

//****************************************************************
//			get_SupportedGdalFormats()
//****************************************************************
#include "Utils.h"
void  CFileManager::get_SupportedGdalFormats(BSTR* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CComBSTR bstr("");
	CComBSTR bstrOptions("--formats");
	GetUtils()->GDALInfo(bstr, bstrOptions, NULL, pVal);
	return ;
}
