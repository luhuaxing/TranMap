#pragma once


// CGlobalSettings
class  CGlobalSettings
{
public:
	CGlobalSettings()
	{
	}


public:
	void get_MinPolygonArea(double* retVal);
	void put_MinPolygonArea(double newVal);
	void get_MinAreaToPerimeterRatio(double* retVal);
	void put_MinAreaToPerimeterRatio(double newVal);
	void get_ClipperGcsMultiplicationFactor(DOUBLE* pVal);
	void put_ClipperGcsMultiplicationFactor(DOUBLE newVal);
	void CreatePoint(CPointClass** retVal);
	void get_ShapefileFastMode(VARIANT_BOOL* pVal);
	void put_ShapefileFastMode(VARIANT_BOOL newVal);
	void get_InvalidShapesBufferDistance(DOUBLE* pVal);
	void put_InvalidShapesBufferDistance(DOUBLE newVal);
	void get_ShapefileFastUnion(VARIANT_BOOL* pVal);
	void put_ShapefileFastUnion(VARIANT_BOOL newVal);
	void get_GdalLastErrorNo(tkGdalError* pVal);
	void get_GdalLastErrorType(tkGdalErrorType* pVal);
	void get_GdalLastErrorMsg(BSTR* pVal);
	void ResetGdalError(void);
	void get_GdalReprojectionErrorMsg(BSTR* pVal);
	void get_LabelsSmoothingMode(tkSmoothingMode* pVal);
	void put_LabelsSmoothingMode(tkSmoothingMode newVal);
	void get_LabelsCompositingQuality(tkCompositingQuality* pVal);
	void put_LabelsCompositingQuality(tkCompositingQuality newVal);
	void put_LocalizedString(tkLocalizedStrings unit, BSTR localizedString);
	void get_LocalizedString(tkLocalizedStrings unit, BSTR* retVal);
	void put_ZoomToFirstLayer(VARIANT_BOOL newVal);
	void get_ZoomToFirstLayer(VARIANT_BOOL* pVal);
	void put_LabelsCollisionMode(tkCollisionMode newVal);
	void get_LabelsCollisionMode(tkCollisionMode* pVal);
	void get_GridProxyFormat(tkGridProxyFormat* pVal);
	void put_GridProxyFormat(tkGridProxyFormat newVal);
	void get_MaxDirectGridSizeMb(double* pVal);
	void put_MaxDirectGridSizeMb(double newVal);
	void put_GridProxyMode(tkGridProxyMode newVal);
	void get_GridProxyMode(tkGridProxyMode* pVal);
	void put_MaxUniqueValuesCountForGridScheme(int newVal);
	void get_MaxUniqueValuesCountForGridScheme(int* pVal);
	void get_RandomColorSchemeForGrids( VARIANT_BOOL *retVal);
	void put_RandomColorSchemeForGrids( VARIANT_BOOL newVal);
	void get_DefaultColorSchemeForGrids( PredefinedColorScheme *retVal);
	void put_DefaultColorSchemeForGrids( PredefinedColorScheme newVal);
	void get_ShapeOutputValidationMode( tkShapeValidationMode *retVal);
	void put_ShapeOutputValidationMode( tkShapeValidationMode newVal);
	void get_ShapeInputValidationMode( tkShapeValidationMode *retVal);
	void put_ShapeInputValidationMode( tkShapeValidationMode newVal);
	void get_GeometryEngine( tkGeometryEngine *retVal);
	void put_GeometryEngine( tkGeometryEngine newVal);
	void get_SaveGridColorSchemeToFile( VARIANT_BOOL *retVal);
	void put_SaveGridColorSchemeToFile( VARIANT_BOOL newVal);
	void get_TiffCompression( tkTiffCompression *retVal);
	void put_TiffCompression( tkTiffCompression newVal);
	void get_RasterOverviewCreation( tkRasterOverviewCreation *retVal);
	void put_RasterOverviewCreation( tkRasterOverviewCreation newVal);
	void get_MinOverviewWidth( int *retVal);
	void put_MinOverviewWidth( int newVal);
	void get_RasterOverviewResampling( tkGDALResamplingMethod *retVal);
	void put_RasterOverviewResampling( tkGDALResamplingMethod newVal);
	void put_TilesThreadPoolSize(int newVal);
	void get_TilesThreadPoolSize(int* pVal);
	void put_LoadSymbologyOnAddLayer(VARIANT_BOOL newVal);
	void get_LoadSymbologyOnAddLayer(VARIANT_BOOL* pVal);
	void put_TilesMinZoomOnProjectionMismatch(int newVal);
	void get_TilesMinZoomOnProjectionMismatch(int* pVal);
	void get_ImageUpsamplingMode(tkInterpolationMode* newVal);
	void put_ImageUpsamplingMode(tkInterpolationMode pVal);
	void get_ImageDownsamplingMode(tkInterpolationMode* newVal);
	void put_ImageDownsamplingMode(tkInterpolationMode pVal);
	void get_GridUpsamplingMode(tkInterpolationMode* newVal);
	void put_GridUpsamplingMode(tkInterpolationMode pVal);
	void get_GridDownsamplingMode(tkInterpolationMode* newVal);
	void put_GridDownsamplingMode(tkInterpolationMode pVal);
	void get_OgrStringEncoding(tkOgrEncoding* newVal);
	void put_OgrStringEncoding(tkOgrEncoding pVal);
	void get_OgrLayerMaxFeatureCount(LONG* pVal);
	void put_OgrLayerMaxFeatureCount(LONG newVal);
	void get_AutoChooseOgrLoadingMode(VARIANT_BOOL* pVal);
	void put_AutoChooseOgrLoadingMode(VARIANT_BOOL newVal);
	void get_AttachMapCallbackToLayers(VARIANT_BOOL* pVal);
	void put_AttachMapCallbackToLayers(VARIANT_BOOL newVal);
	void get_HotTrackingMaxShapeCount(LONG* pVal);
	void put_HotTrackingMaxShapeCount(LONG newVal);
	void get_AllowLayersWithoutProjections(VARIANT_BOOL* pVal);
	void put_AllowLayersWithoutProjections(VARIANT_BOOL newVal);
	void get_AllowProjectionMismatch(VARIANT_BOOL* pVal);
	void put_AllowProjectionMismatch(VARIANT_BOOL newVal);
	void get_ReprojectLayersOnAdding(VARIANT_BOOL* pVal);
	void put_ReprojectLayersOnAdding(VARIANT_BOOL newVal);
	void get_MouseTolerance(DOUBLE* pVal);
	void put_MouseTolerance(DOUBLE newVal);
	void get_ApplicationCallback(ICallback** pVal);
	void put_ApplicationCallback(ICallback* newVal);
	void get_MaxReprojectionShapeCount(LONG* pVal);
	void put_MaxReprojectionShapeCount(LONG newVal);
	void get_PixelOffsetMode(tkPixelOffsetMode* pVal);
	void put_PixelOffsetMode(tkPixelOffsetMode newVal);
	void get_AutoChooseRenderingHintForLabels(VARIANT_BOOL* pVal);
	void put_AutoChooseRenderingHintForLabels(VARIANT_BOOL newVal);
	void get_GdalVersion(BSTR* retVal);
	void get_OgrLayerForceUpdateMode(VARIANT_BOOL* pVal);
	void put_OgrLayerForceUpdateMode(VARIANT_BOOL newVal);
	void get_ForceHideLabels(VARIANT_BOOL* pVal);
	void put_ForceHideLabels(VARIANT_BOOL newVal);
	void get_GdalPluginPath(BSTR* pVal);
	void put_GdalPluginPath(BSTR newVal);
	void get_GdalDataPath(BSTR* pVal);
	void put_GdalDataPath(BSTR newVal);
	void get_BingApiKey(BSTR* pVal);
	void put_BingApiKey(BSTR newVal);
	void TestBingApiKey(BSTR key, VARIANT_BOOL* retVal);
	void SetHereMapsApiKey(BSTR appId, BSTR appCode);
};