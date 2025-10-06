#include "stdafx.h"
#include "GlobalSettings.h"
#include "BingMapProvider.h"

// ****************************************************
//	    get_MinPolygonArea()
// ****************************************************
void  CGlobalSettings::get_MinPolygonArea(double* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = m_globalSettings.minPolygonArea;
	return ;
}

// ****************************************************
//	    put_MinPolygonArea()
// ****************************************************
void  CGlobalSettings::put_MinPolygonArea(double newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	m_globalSettings.minPolygonArea = newVal;
	return ;
}

// ****************************************************
//	    get_MinAreaToPerimeterRatio()
// ****************************************************
void  CGlobalSettings::get_MinAreaToPerimeterRatio(double* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = m_globalSettings.minAreaToPerimeterRatio;
	return ;
}

// ****************************************************
//	    put_MinAreaToPerimeterRatio()
// ****************************************************
void  CGlobalSettings::put_MinAreaToPerimeterRatio(double newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	m_globalSettings.minAreaToPerimeterRatio = newVal;
	return ;
}

// ****************************************************
//	    get_ClipperGcsMultiplicationFactor()
// ****************************************************
void  CGlobalSettings::get_ClipperGcsMultiplicationFactor(DOUBLE* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = m_globalSettings.clipperGcsMultiplicationFactor;
	return ;
}

// ****************************************************
//	    put_ClipperGcsMultiplicationFactor()
// ****************************************************
void  CGlobalSettings::put_ClipperGcsMultiplicationFactor(DOUBLE newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_globalSettings.clipperGcsMultiplicationFactor = newVal;
	return ;
}

// ****************************************************
//	    CreatePoint()
// ****************************************************
void  CGlobalSettings::CreatePoint(CPointClass** retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = new CPointClass();
	return ;
}

// ****************************************************
//	    get_ShapefileFastMode()
// ****************************************************
void  CGlobalSettings::get_ShapefileFastMode(VARIANT_BOOL* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = m_globalSettings.shapefileFastMode ? VARIANT_TRUE : VARIANT_FALSE;
	return ;
}

// ****************************************************
//	    put_ShapefileFastMode()
// ****************************************************
void  CGlobalSettings::put_ShapefileFastMode(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_globalSettings.shapefileFastMode = newVal == VARIANT_FALSE ? false: true;
	return ;
}

// ****************************************************
//	    get_ShapefileFastMode()
// ****************************************************
void  CGlobalSettings::get_ZoomToFirstLayer(VARIANT_BOOL* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = m_globalSettings.zoomToFirstLayer ? VARIANT_TRUE : VARIANT_FALSE;
	return ;
}

void  CGlobalSettings::put_ZoomToFirstLayer(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_globalSettings.zoomToFirstLayer = newVal == VARIANT_FALSE ? false: true;
	return ;
}

// ****************************************************
//	    get_LabelCollisionMode()
// ****************************************************
void  CGlobalSettings::get_LabelsCollisionMode(tkCollisionMode* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = m_globalSettings.labelsCollisionMode;
	return ;
}

void  CGlobalSettings::put_LabelsCollisionMode(tkCollisionMode newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_globalSettings.labelsCollisionMode = newVal;
	return ;
}


// ****************************************************
//	    get_InvalidShapesBufferDistance()
// ****************************************************
void  CGlobalSettings::get_InvalidShapesBufferDistance(DOUBLE* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = m_globalSettings.invalidShapesBufferDistance;
	return ;
}

// ****************************************************
//	    put_InvalidShapesBufferDistance()
// ****************************************************
void  CGlobalSettings::put_InvalidShapesBufferDistance(DOUBLE newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_globalSettings.invalidShapesBufferDistance = newVal;
	return ;
}

// ****************************************************
//	    get_ShapefileFastUnion()
// ****************************************************
void  CGlobalSettings::get_ShapefileFastUnion(VARIANT_BOOL* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = m_globalSettings.shapefileFastUnion ? VARIANT_TRUE : VARIANT_FALSE;
	return ;
}

// ****************************************************
//	    put_ShapefileFastUnion()
// ****************************************************
void  CGlobalSettings::put_ShapefileFastUnion(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_globalSettings.shapefileFastUnion = newVal?true:false;
	return ;
}

#pragma region "Gdal error handling"
// ****************************************************
//	    get_GdalLastErrorNo()
// ****************************************************
void  CGlobalSettings::get_GdalLastErrorNo(tkGdalError* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	int err = CPLGetLastErrorNo();
	if (err < 0 || err >= (int)GdalErrorUndefined)
		err = (int)GdalErrorUndefined;
	*pVal = (tkGdalError)err;
	return ;
}

// ****************************************************
//	    get_GdalLastErrorType()
// ****************************************************
void  CGlobalSettings::get_GdalLastErrorType(tkGdalErrorType* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	int err = CPLGetLastErrorType();
	if (err < 0 || err >= (int)GdalUndefined)
		err = (int)GdalUndefined;
	*pVal = (tkGdalErrorType)err;
	return ;
}

// ****************************************************
//	    get_GdalLastErrorMsg()
// ****************************************************
void  CGlobalSettings::get_GdalLastErrorMsg(BSTR* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString s = CPLGetLastErrorMsg();
	*pVal = A2BSTR(s);
	return ;
}

// ****************************************************
//	    get_GdalReprojectionErrorMsg()
// ****************************************************
void  CGlobalSettings::get_GdalReprojectionErrorMsg(BSTR* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = A2BSTR(m_globalSettings.gdalErrorMessage);
	return ;
}

// ****************************************************
//	    ResetGdalError()
// ****************************************************
void  CGlobalSettings::ResetGdalError(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CPLErrorReset();
	m_globalSettings.gdalErrorMessage = "";
	return ;
}
#pragma endregion

#pragma region Labels

// ****************************************************
//	    get_CompositingQuality()
// ****************************************************
void  CGlobalSettings::get_LabelsCompositingQuality(tkCompositingQuality* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = m_globalSettings.labelsCompositingQuality;
	return ;
}

// ****************************************************
//	    put_CompositingQuality()
// ****************************************************
void  CGlobalSettings::put_LabelsCompositingQuality(tkCompositingQuality newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (newVal >= 0 && newVal <= AssumeLinear)
		m_globalSettings.labelsCompositingQuality = newVal;
	return ;
}

// ****************************************************
//	    get_SmoothingMode()
// ****************************************************
void  CGlobalSettings::get_LabelsSmoothingMode(tkSmoothingMode* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = m_globalSettings.labelsSmoothingMode;
	return ;
}
void  CGlobalSettings::put_LabelsSmoothingMode(tkSmoothingMode newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (newVal >= 0 && newVal <= AntiAlias)
		m_globalSettings.labelsSmoothingMode = newVal;
	return ;
}
#pragma endregion

// ****************************************************
//	    LocalizedString()
// ****************************************************
void  CGlobalSettings::put_LocalizedString(tkLocalizedStrings unit, BSTR localizedString)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	USES_CONVERSION;
	m_globalSettings.shortUnitStrings[unit] = OLE2W(localizedString);
	return ;
}
void  CGlobalSettings::get_LocalizedString(tkLocalizedStrings unit, BSTR* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CStringW s = m_globalSettings.GetLocalizedString(unit);
	USES_CONVERSION;
	*retVal = W2BSTR(s);
	return ;
}

// ****************************************************
//	    GridProxyFormat()
// ****************************************************
void  CGlobalSettings::get_GridProxyFormat(tkGridProxyFormat* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = m_globalSettings.gridProxyFormat;
	return ;
}
void  CGlobalSettings::put_GridProxyFormat(tkGridProxyFormat newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_globalSettings.gridProxyFormat = newVal;
	return ;
}

// ****************************************************
//	    MaxDirectGridSizeMb()
// ****************************************************
void  CGlobalSettings::get_MaxDirectGridSizeMb(double* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = m_globalSettings.MaxDirectGridSizeMb;
	return ;
}
void  CGlobalSettings::put_MaxDirectGridSizeMb(double newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_globalSettings.MaxDirectGridSizeMb = newVal;
	return ;
}

// ****************************************************
//	    get_AlwaysCreateProxyForGrids()
// ****************************************************
void  CGlobalSettings::get_GridProxyMode(tkGridProxyMode* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = m_globalSettings.gridProxyMode;
	return ;
}
void  CGlobalSettings::put_GridProxyMode(tkGridProxyMode newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_globalSettings.gridProxyMode = newVal;
	return ;
}

// ****************************************************
//	    get_MaxUniqueValuesCountForGridScheme()
// ****************************************************
void  CGlobalSettings::get_MaxUniqueValuesCountForGridScheme(int* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = m_globalSettings.maxUniqueValuesCount;
	return ;
}
void  CGlobalSettings::put_MaxUniqueValuesCountForGridScheme(int newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_globalSettings.maxUniqueValuesCount = newVal;
	return ;
}

// ****************************************************
//	    RandomColorSchemeForGrids()
// ****************************************************
void  CGlobalSettings::get_RandomColorSchemeForGrids(VARIANT_BOOL* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = m_globalSettings.randomColorSchemeForGrids ? VARIANT_TRUE: VARIANT_FALSE;
	return ;
}
void  CGlobalSettings::put_RandomColorSchemeForGrids(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_globalSettings.randomColorSchemeForGrids = newVal ? true: false;
	return ;
}
	
// ****************************************************
//	    DefaultColorSchemeForGrids()
// ****************************************************
void  CGlobalSettings::get_DefaultColorSchemeForGrids(PredefinedColorScheme* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = m_globalSettings.defaultColorSchemeForGrids;
	return ;
}
void  CGlobalSettings::put_DefaultColorSchemeForGrids(PredefinedColorScheme newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_globalSettings.defaultColorSchemeForGrids = newVal;
	return ;
}

// ****************************************************
//	    ShapeOutputValidationMode
// ****************************************************
void  CGlobalSettings::get_ShapeOutputValidationMode(tkShapeValidationMode* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = m_globalSettings.outputValidation;
	return ;
}
void  CGlobalSettings::put_ShapeOutputValidationMode(tkShapeValidationMode newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_globalSettings.outputValidation = newVal;
	return ;
}

// ****************************************************
//	    ShapeInputValidationMode
// ****************************************************
void  CGlobalSettings::get_ShapeInputValidationMode(tkShapeValidationMode* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = m_globalSettings.inputValidation;
	return ;
}
void  CGlobalSettings::put_ShapeInputValidationMode(tkShapeValidationMode newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_globalSettings.inputValidation = newVal;
	return ;
}

// ****************************************************
//	    GeometryEngine
// ****************************************************
void  CGlobalSettings::get_GeometryEngine(tkGeometryEngine* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = m_globalSettings.geometryEngine;
	return ;
}
void  CGlobalSettings::put_GeometryEngine(tkGeometryEngine newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_globalSettings.geometryEngine = newVal;
	return ;
}

// ****************************************************
//	    SaveGridColorSchemeToFile
// ****************************************************
void  CGlobalSettings::get_SaveGridColorSchemeToFile( VARIANT_BOOL *retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = m_globalSettings.saveGridColorSchemeToFile ? VARIANT_TRUE: VARIANT_FALSE;
	return ;
}

void  CGlobalSettings::put_SaveGridColorSchemeToFile( VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_globalSettings.saveGridColorSchemeToFile = newVal ? true : false;
	return ;
}

// ****************************************************
//	    TiffCompression
// ****************************************************
void  CGlobalSettings::get_TiffCompression( tkTiffCompression *retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = m_globalSettings.tiffCompression;
	return ;
}
void  CGlobalSettings::put_TiffCompression( tkTiffCompression newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_globalSettings.tiffCompression = newVal;
	return ;
}

// ****************************************************
//	    RasterOverviewCreation
// ****************************************************
void  CGlobalSettings::get_RasterOverviewCreation( tkRasterOverviewCreation *retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = m_globalSettings.rasterOverviewCreation;
	return ;
}
void  CGlobalSettings::put_RasterOverviewCreation( tkRasterOverviewCreation newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_globalSettings.rasterOverviewCreation = newVal;
	return ;
}

// ****************************************************
//	    MinOverviewWidth
// ****************************************************
void  CGlobalSettings::get_MinOverviewWidth( int *retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = m_globalSettings.minOverviewWidth;
	return ;
}
void  CGlobalSettings::put_MinOverviewWidth( int newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_globalSettings.minOverviewWidth = newVal;
	return ;
}

// ****************************************************
//	    RasterOverviewResampling
// ****************************************************
void  CGlobalSettings::get_RasterOverviewResampling( tkGDALResamplingMethod *retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = m_globalSettings.rasterOverviewResampling;
	return ;
}
void  CGlobalSettings::put_RasterOverviewResampling( tkGDALResamplingMethod newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_globalSettings.rasterOverviewResampling = newVal;
	return ;
}

// *********************************************************
//	     TilesThreadPoolSize
// *********************************************************
void  CGlobalSettings::get_TilesThreadPoolSize(int* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = m_globalSettings.tilesThreadPoolSize;
	return ;
}
void  CGlobalSettings::put_TilesThreadPoolSize(int newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_globalSettings.tilesThreadPoolSize = newVal;
	return ;
}

// *********************************************************
//	     LoadSymbologyOnAddLayer
// *********************************************************
void  CGlobalSettings::get_LoadSymbologyOnAddLayer(VARIANT_BOOL* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = m_globalSettings.loadSymbologyOnAddLayer;
	return ;
}
void  CGlobalSettings::put_LoadSymbologyOnAddLayer(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_globalSettings.loadSymbologyOnAddLayer = newVal ? true: false;
	return ;
}

// *********************************************************
//	     TilesMinZoomOnProjectionMismatch
// *********************************************************
void  CGlobalSettings::get_TilesMinZoomOnProjectionMismatch(int* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = m_globalSettings.tilesMaxZoomOnProjectionMismatch;
	return ;
}
void  CGlobalSettings::put_TilesMinZoomOnProjectionMismatch(int newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_globalSettings.tilesMaxZoomOnProjectionMismatch = newVal;
	return ;
}


// *********************************************************
//	     ImageUpsamplingMode
// *********************************************************
void  CGlobalSettings::get_ImageUpsamplingMode(tkInterpolationMode* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = m_globalSettings.imageUpsamplingMode;
	return ;
}
void  CGlobalSettings::put_ImageUpsamplingMode(tkInterpolationMode newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_globalSettings.imageUpsamplingMode = newVal;
	return ;
}

// *********************************************************
//	     ImageDownsamplingMode
// *********************************************************
void  CGlobalSettings::get_ImageDownsamplingMode(tkInterpolationMode* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = m_globalSettings.imageDownsamplingMode;
	return ;
}
void  CGlobalSettings::put_ImageDownsamplingMode(tkInterpolationMode newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_globalSettings.imageDownsamplingMode = newVal;
	return ;
}

// *********************************************************
//	     GridUpsamplingMode
// *********************************************************
void  CGlobalSettings::get_GridUpsamplingMode(tkInterpolationMode* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = m_globalSettings.gridUpsamplingMode;
	return ;
}
void  CGlobalSettings::put_GridUpsamplingMode(tkInterpolationMode newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_globalSettings.gridUpsamplingMode = newVal;
	return ;
}

// *********************************************************
//	     GridDownsamplingMode
// *********************************************************
void  CGlobalSettings::get_GridDownsamplingMode(tkInterpolationMode* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = m_globalSettings.gridDownsamplingMode;
	return ;
}
void  CGlobalSettings::put_GridDownsamplingMode(tkInterpolationMode newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_globalSettings.gridDownsamplingMode = newVal;
	return ;
}

// *********************************************************
//	     OgrStringEncoding
// *********************************************************
void  CGlobalSettings::get_OgrStringEncoding(tkOgrEncoding* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = m_globalSettings.ogrEncoding;
	return ;
}

void  CGlobalSettings::put_OgrStringEncoding(tkOgrEncoding pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_globalSettings.ogrEncoding = pVal;
	return ;
}

// *********************************************************
//	     OgrLayerMaxFeatureCount
// *********************************************************
void  CGlobalSettings::get_OgrLayerMaxFeatureCount(LONG* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = m_globalSettings.ogrLayerMaxFeatureCount;
	return ;
}
void  CGlobalSettings::put_OgrLayerMaxFeatureCount(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_globalSettings.ogrLayerMaxFeatureCount = newVal;
	return ;
}

// *********************************************************
//	     AutoChooseOgrLoadingMode
// *********************************************************
void  CGlobalSettings::get_AutoChooseOgrLoadingMode(VARIANT_BOOL* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = m_globalSettings.autoChooseOgrLoadingMode ? VARIANT_TRUE: VARIANT_FALSE;
	return ;
}
void  CGlobalSettings::put_AutoChooseOgrLoadingMode(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_globalSettings.autoChooseOgrLoadingMode = newVal ? true : false;
	return ;
}

// *********************************************************
//	     AttachMapCallbackToLayers
// *********************************************************
void  CGlobalSettings::get_AttachMapCallbackToLayers(VARIANT_BOOL* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = m_globalSettings.attachMapCallbackToLayers ? VARIANT_TRUE : VARIANT_FALSE;
	return ;
}
void  CGlobalSettings::put_AttachMapCallbackToLayers(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_globalSettings.attachMapCallbackToLayers = newVal ? true : false;
	return ;
}

// *********************************************************
//	     HotTrackingMaxShapeCount
// *********************************************************
void  CGlobalSettings::get_HotTrackingMaxShapeCount(LONG* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = m_globalSettings.hotTrackingMaxShapeCount;
	return ;
}
void  CGlobalSettings::put_HotTrackingMaxShapeCount(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_globalSettings.hotTrackingMaxShapeCount = newVal;
	return ;
}

// *********************************************************
//	     AllowLayersWithoutProjections
// *********************************************************
void  CGlobalSettings::get_AllowLayersWithoutProjections(VARIANT_BOOL* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = m_globalSettings.allowLayersWithoutProjection ? VARIANT_TRUE : VARIANT_FALSE;
	return ;
}
void  CGlobalSettings::put_AllowLayersWithoutProjections(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_globalSettings.allowLayersWithoutProjection = newVal ? true : false;
	return ;
}

// *********************************************************
//	     AllowProjectionMismatch
// *********************************************************
void  CGlobalSettings::get_AllowProjectionMismatch(VARIANT_BOOL* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = m_globalSettings.allowProjectionMismatch ? VARIANT_TRUE : VARIANT_FALSE;
	return ;
}
void  CGlobalSettings::put_AllowProjectionMismatch(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_globalSettings.allowProjectionMismatch = newVal ? true : false;
	return ;
}

// *********************************************************
//	     ReprojectLayersOnAdding
// *********************************************************
void  CGlobalSettings::get_ReprojectLayersOnAdding(VARIANT_BOOL* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = m_globalSettings.reprojectLayersOnAdding ? VARIANT_TRUE : VARIANT_FALSE;
	return ;
}
void  CGlobalSettings::put_ReprojectLayersOnAdding(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_globalSettings.reprojectLayersOnAdding = newVal ? true : false;
	return ;
}

// *********************************************************
//	     MouseTolerance
// *********************************************************
void  CGlobalSettings::get_MouseTolerance(DOUBLE* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = m_globalSettings.mouseTolerance ? VARIANT_TRUE : VARIANT_FALSE;
	return ;
}
void  CGlobalSettings::put_MouseTolerance(DOUBLE newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_globalSettings.mouseTolerance = newVal ? true : false;
	return ;
}

// *********************************************************
//	     ApplicationCallback
// *********************************************************
void  CGlobalSettings::get_ApplicationCallback(ICallback** pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = m_globalSettings.callback;
	return ;
}
void  CGlobalSettings::put_ApplicationCallback(ICallback* newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_globalSettings.callback = newVal;
	return ;
}

// *********************************************************
//	     MaxReprojectionShapeCount
// *********************************************************
void  CGlobalSettings::get_MaxReprojectionShapeCount(LONG* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = m_globalSettings.maxReprojectionShapeCount;
	return ;
}
void  CGlobalSettings::put_MaxReprojectionShapeCount(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_globalSettings.maxReprojectionShapeCount = newVal;
	return ;
}

// *********************************************************
//	     PixelOffsetMode
// *********************************************************
void  CGlobalSettings::get_PixelOffsetMode(tkPixelOffsetMode* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = m_globalSettings.pixelOffsetMode;
	return ;
}
void  CGlobalSettings::put_PixelOffsetMode(tkPixelOffsetMode newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_globalSettings.pixelOffsetMode = newVal;
	return ;
}

// *********************************************************
//	     AutoChooseRenderingHintForLabels
// *********************************************************
void  CGlobalSettings::get_AutoChooseRenderingHintForLabels(VARIANT_BOOL* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = m_globalSettings.autoChooseRenderingHintForLabels ? VARIANT_TRUE : VARIANT_FALSE;
	return ;
}
void  CGlobalSettings::put_AutoChooseRenderingHintForLabels(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_globalSettings.autoChooseRenderingHintForLabels = newVal ? true : false;
	return ;
}

// ***************************************************************
//	get_GdalVersion
// ***************************************************************
#include "Utils.h"
void  CGlobalSettings::get_GdalVersion(BSTR* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CComBSTR bstr("");
	CComBSTR bstrOptions("--version");
	GetUtils()->GDALInfo(bstr, bstrOptions, NULL, retVal);
	return ;
}

// ***************************************************************
//	OgrLayerForceUpdateMode
// ***************************************************************
void  CGlobalSettings::get_OgrLayerForceUpdateMode(VARIANT_BOOL* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = m_globalSettings.ogrLayerForceUpdateMode ? VARIANT_TRUE : VARIANT_FALSE;
	return ;
}
void  CGlobalSettings::put_OgrLayerForceUpdateMode(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_globalSettings.ogrLayerForceUpdateMode = newVal ? true : false;
	return ;
}

// ***************************************************************
//		ForceHideLabels
// ***************************************************************
void  CGlobalSettings::get_ForceHideLabels(VARIANT_BOOL* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = m_globalSettings.forceHideLabels ? VARIANT_TRUE : VARIANT_FALSE;
	return ;
}
void  CGlobalSettings::put_ForceHideLabels(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_globalSettings.forceHideLabels = newVal ? true : false;
	return ;
}

// ***************************************************************
//		GdalPluginPath
// ***************************************************************
void  CGlobalSettings::get_GdalPluginPath(BSTR* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CStringW path = GdalHelper::GetConfigPath(PathGdalPlugins);
	*pVal = W2BSTR(path);
	return ;
}
void  CGlobalSettings::put_GdalPluginPath(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	GdalHelper::SetConfigPath(PathGdalPlugins, OLE2W(newVal));
	return ;
}

// ***************************************************************
//		GdalDataPath
// ***************************************************************
void  CGlobalSettings::get_GdalDataPath(BSTR* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CStringW path = GdalHelper::GetConfigPath(PathGdalData);
	*pVal = W2BSTR(path);
	return ;
}
void  CGlobalSettings::put_GdalDataPath(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	GdalHelper::SetConfigPath(PathGdalData, OLE2W(newVal));
	return ;
}

// ***************************************************************
//		BingApiKey
// ***************************************************************
void  CGlobalSettings::get_BingApiKey(BSTR* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = A2BSTR(m_globalSettings.bingApiKey);
	return ;
}
void  CGlobalSettings::put_BingApiKey(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	USES_CONVERSION;
	m_globalSettings.bingApiKey = OLE2A(newVal);
	return ;
}

// ***************************************************************
//		TestBingApiKey
// ***************************************************************
void  CGlobalSettings::TestBingApiKey(BSTR key, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = VARIANT_FALSE;
	
	CString previosKey = m_globalSettings.bingApiKey;
	m_globalSettings.bingApiKey = key;

	BingMapProvider provider;
	if (provider.Initialize()) {
		m_globalSettings.bingApiKey = key;
		*retVal = VARIANT_TRUE;
	}
	else {
		m_globalSettings.bingApiKey = previosKey;
	}

	return ;
}

// ***************************************************************
//		SetHereMapsApiKey
// ***************************************************************
void  CGlobalSettings::SetHereMapsApiKey(BSTR appId, BSTR appCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	USES_CONVERSION;
	m_globalSettings.hereAppCode = OLE2A(appCode);
	m_globalSettings.hereAppId = OLE2A(appId);
	return ;
}
