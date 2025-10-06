#pragma once
#include "tkBitmap.h"
#include "tkRaster.h"
#include "ImageGroup.h"
#include "InMemoryBitmap.h"
#include "Grid.h"
#include "Labels.h"
#include "GridColorScheme.h"
#include "ILayer.h"

struct DataPixels
{
public:
	colour value;
	int position; 
};

// CImageClass
class  CImageClass:public ILayer
{
public:
	
	// constructor
	CImageClass();
	~CImageClass();
	
	void SetDefaults();
// CImage
public:
	eLayerType GetLayerType() { return eLayerType::IMAGE_LAYER; }
	void SetImageBitsDC(/*[in]*/ long hDC, /*[out, retval]*/ VARIANT_BOOL * retval);
	void GetImageBitsDC(/*[in]*/ long hDC, /*[out, retval]*/ VARIANT_BOOL * retval);
	void get_Filename(/*[out, retval]*/ BSTR *pVal);
	void get_Picture(/*[out, retval]*/ IPictureDisp * *pVal);
	void putref_Picture(/*[in]*/ IPictureDisp *newVal);
	void get_ImageType(/*[out, retval]*/ ImageType *pVal);
	void get_FileHandle(/*[out, retval]*/ long *pVal);
	void get_Key(/*[out, retval]*/ BSTR *pVal);
	void put_Key(/*[in]*/ BSTR newVal);
	void get_GlobalCallback(/*[out, retval]*/ ICallback* *pVal);
	void put_GlobalCallback(/*[in]*/ ICallback* newVal);
	void get_CdlgFilter(/*[out, retval]*/ BSTR *pVal);
	void get_ErrorMsg(/*[in]*/long ErrorCode, /*[out, retval]*/ BSTR *pVal);
	void get_LastErrorCode(/*[out, retval]*/ long *pVal);
	void get_UseTransparencyColor(/*[out, retval]*/ VARIANT_BOOL *pVal);
	void put_UseTransparencyColor(/*[in]*/ VARIANT_BOOL newVal);
	void get_TransparencyColor(/*[out, retval]*/ OLE_COLOR *pVal);
	void put_TransparencyColor(/*[in]*/ OLE_COLOR newVal);
	void get_IsInRam(/*[out, retval]*/ VARIANT_BOOL *pVal);
	void get_Value(/*[in]*/long row, /*[in]*/long col, /*[out, retval]*/ long *pVal);
	void put_Value(/*[in]*/long row, /*[in]*/long col, /*[in]*/ long newVal);
	void get_dX(/*[out, retval]*/ double *pVal);
	void put_dX(/*[in]*/ double newVal);
	void get_dY(/*[out, retval]*/ double *pVal);
	void put_dY(/*[in]*/ double newVal);
	void get_XllCenter(/*[out, retval]*/ double *pVal);
	void put_XllCenter(/*[in]*/ double newVal);
	void get_YllCenter(/*[out, retval]*/ double *pVal);
	void put_YllCenter(/*[in]*/ double newVal);
	void get_Height(/*[out, retval]*/ long *pVal);
	void get_Width(/*[out, retval]*/ long *pVal);
	void GetRow(/*[in]*/ long Row, /*[in, out]*/ long * Vals, /*[out, retval]*/VARIANT_BOOL * retval);
	void Clear(/*[in, optional, defaultvalue(0xffffff)]*/ OLE_COLOR CanvasColor, /*[in, optional]*/ICallback * CBack, /*[out, retval]*/VARIANT_BOOL * retval);
	void Close(/*[out, retval]*/VARIANT_BOOL * retval);
	void CreateNew(/*[in]*/long NewWidth, /*[in]*/long NewHeight, /*[out, retval]*/VARIANT_BOOL * retval);
	void Save(/*[in]*/BSTR ImageFileName, /*[in, optional, defaultvalue(FALSE)]*/VARIANT_BOOL WriteWorldFile,/*[in, optional, defaultvalue(USE_FILE_EXTENSION)]*/ImageType FileType, /*[in, optional]*/ICallback * cBack, /*[out, retval]*/ VARIANT_BOOL * retval);
	void Open(/*[in]*/BSTR ImageFileName, /*[in, optional, defaultvalue(USE_FILE_EXTENSION)]*/ImageType FileType, /*[in, optional, defaultvalue(TRUE)]*/ VARIANT_BOOL InRam, /*[in, optional]*/ICallback * cBack, /*[out, retval]*/ VARIANT_BOOL * retval);
	//Rob Cairns
	void SetVisibleExtents(/*[in]*/double newMinX,/*[in]*/ double newMinY,/*[in]*/ double newMaxX,/*[in]*/ double newMaxY,/*[in]*/long pixInView, /*[in]*/float transPercent);
	void SetProjection(/*[in]*/BSTR Proj4, /*[out, retval]*/ VARIANT_BOOL * retval);
	void GetProjection(/*[out, retval]*/ BSTR * Proj4);
	void Resource(/*[in]*/ BSTR newSrcPath, /*[out, retval]*/ VARIANT_BOOL * retval);
	void _pushSchemetkRaster(/*[in]*/ CGridColorScheme * cScheme, /*[out, retval]*/ VARIANT_BOOL * retval);
	//Rob Cairns 1-Jun-09

	// deprecated
	void GetOriginalXllCenter(/*[out, retval]*/ double *pVal);
	void GetOriginalYllCenter(/*[out, retval]*/ double *pVal);
	void GetOriginal_dX(/*[out, retval]*/ double *pVal);
	void GetOriginal_dY(/*[out, retval]*/ double *pVal);
	void GetOriginalHeight(/*[out, retval]*/ long *pVal);
	void GetOriginalWidth(/*[out, retval]*/ long *pVal);
	// deprecated

	void get_AllowHillshade(/*[out]*/VARIANT_BOOL *pVal);
	void put_AllowHillshade(/*[in]*/VARIANT_BOOL newValue);
	void get_SetToGrey(/*[out, retval]*/VARIANT_BOOL *pVal);
	void put_SetToGrey(/*[in]*/VARIANT_BOOL newValue);
	void get_UseHistogram(/*[out, retval]*/VARIANT_BOOL *pVal);
	void put_UseHistogram(/*[in]*/VARIANT_BOOL newValue);
	void get_HasColorTable(/*[out, retval]*/VARIANT_BOOL *pVal);
	void get_PaletteInterpretation(/*[out, retval]*/BSTR *pVal);
	void get_BufferSize(/*[out, retval]*/int *pVal);
	void put_BufferSize(/*[in]*/int newValue);
	void get_NoBands(/*[out, retval]*/int *pVal);
	void get_ImageColorScheme(/*[out, retval]*/PredefinedColorScheme *pVal);
	void put_ImageColorScheme(/*[in]*/PredefinedColorScheme newValue);
	void get_ClearGDALCache(VARIANT_BOOL* retVal);	
	void put_ClearGDALCache(VARIANT_BOOL newVal);	
	void get_TransparencyPercent(double* retVal);	
	void put_TransparencyPercent(double newVal);
	void get_TransparencyColor2(OLE_COLOR* retVal);	
	void put_TransparencyColor2(OLE_COLOR newVal);	
	void get_DownsamplingMode(tkInterpolationMode* retVal);	
	void put_DownsamplingMode(tkInterpolationMode newVal);	
	void get_UpsamplingMode(tkInterpolationMode* retVal);	
	void put_UpsamplingMode(tkInterpolationMode newVal);	
	void get_DrawingMethod(int* retVal);	
	void put_DrawingMethod(int newVal);	
	void BuildOverviews(tkGDALResamplingMethod ResamplingMethod, int numOverviews, SAFEARRAY* OverviewList, VARIANT_BOOL * retval);
	void get_Labels(CLabels** pVal);
	void put_Labels(CLabels* newValue);
	void get_OriginalWidth(LONG* OriginalWidth);
	void get_OriginalHeight(LONG* OriginalHeight);
	void get_Extents(CExtents** pVal);
	void ProjectionToImage(double ProjX, double ProjY, long* ImageX, long* ImageY);
	void ImageToProjection(long ImageX, long ImageY, double* ProjX, double* ProjY);
	void ProjectionToBuffer(double ProjX, double ProjY, long* BufferX, long* BufferY);
	void BufferToProjection(long BufferX, long BufferY, double* ProjX, double* ProjY);
	void get_CanUseGrouping(VARIANT_BOOL* retVal);
	void put_CanUseGrouping(VARIANT_BOOL newVal);
	void get_OriginalXllCenter( double *pVal);
	void put_OriginalXllCenter( double newVal);
	void get_OriginalYllCenter( double *pVal);
	void put_OriginalYllCenter( double newVal);
	void get_OriginalDX( double *pVal);
	void put_OriginalDX( double newVal);
	void get_OriginalDY( double *pVal);
	void put_OriginalDY( double newVal);
	void GetUniqueColors(double MaxBufferSizeMB, VARIANT* Colors, VARIANT* Frequencies, LONG* Count);
	void SetNoDataValue(double Value, VARIANT_BOOL* Result);
	void get_NumOverviews(int* retval);
	void LoadBuffer(double maxBufferSize, VARIANT_BOOL* retVal);	// loads the whole buffer
	void get_SourceType(tkImageSourceType* retVal);
	void Serialize(VARIANT_BOOL SerializePixels, BSTR* retVal);
	void Deserialize(BSTR newVal);
	void get_Warped(VARIANT_BOOL* retVal);
	
	void get_SourceGridName(BSTR* retVal);
	void get_IsGridProxy(VARIANT_BOOL* retVal);
	void get_CustomColorScheme( CGridColorScheme** pVal);
	void put_CustomColorScheme( CGridColorScheme* newVal);
	//void get_CanUseExternalColorScheme(VARIANT_BOOL* retVal);
	void get_AllowGridRendering(tkGridRendering* retVal);
	void put_AllowGridRendering(tkGridRendering newVal);
	void get_GridRendering(VARIANT_BOOL* retVal);
	void SetTransparentColor(OLE_COLOR color);
	void get_IsRgb(VARIANT_BOOL* retVal);
	void OpenAsGrid(CGrid** retVal);
	void get_SourceFilename(BSTR* retVal);
	void get_SourceGridBandIndex(int* retVal);
	void put_SourceGridBandIndex(int newVal);
	void get_GridProxyColorScheme(CGridColorScheme** retVal);
	void get_GeoProjection(CGeoProjection** pVal);
	void get_IsEmpty(VARIANT_BOOL* pVal);

private:
	tkImageSourceType _sourceType;
	
	bool _inRam;
	BSTR _key;
	ICallback * _globalCallback;
	long _lastErrorCode;

	//world coordinate related variables
	double _dY;			//change in Y (for size of cell or pixel)
	double _dX;			//change in X (for size of cell or pixel)
	double _yllCenter;	//Y coordinate of lower left corner of image (world coordinate)
	double _xllCenter;	//X coordinate of lower left corner of image (world coordinate)

	bool _gdalImage;
	bool _dataLoaded;
	bool _setRGBToGrey;			//Set a color RGB image or hillshade to gray
	CStringW _fileName;			//For GDALOpen

	//Image Variables
	colour * _imageData;		//array storing generic image data
	long _width;				//number of Columns in image
	long _height;			//number of rows in image
	ImageType _imgType;	    //enumeration stating type of image currently being used

	//Image Objects
	tkBitmap* _bitmapImage;
	tkRaster* _rasterImage;

	//Transparency
	OLE_COLOR _transColor;
	OLE_COLOR _transColor2;	// to specify range for GDI+
	VARIANT_BOOL _useTransColor;

	double _transparencyPercent;
	tkInterpolationMode _downsamplingMode;
	tkInterpolationMode _upsamplingMode;
	short _drawingMethod;
	bool _downSampling;	// the image in the buffer was produced by downsampling

	CLabels* _labels;
	CGeoProjection* _projection;

public:
	int m_groupID;			// in case belong to the group of images which are to be treated as one
	DataPixels* m_pixels;	// a structure to hold values of pixels with the value other than noDataValue
	int m_pixelsCount;
	bool _canUseGrouping;
	bool _pixelsSaved;
	bool _bufferReloadIsNeeded;
	bool _imageChanged;
	ScreenBitmap* _screenBitmap;	// GDI+ bitmap on the screen
	CStringW sourceGridName;
	bool isGridProxy;
	CMemoryBitmap* _iconGdiPlus;	// in-memory state of GDI+ image file (after deserialization)

private:
	bool Resample();
	bool ReadPPM(CString ImageFile, bool InRam = true);
	bool ReadGIF(CString ImageFile);
	bool ReadJPEG(CString ImageFile);
	bool ReadBMP(const CStringW ImageFile, bool InRam);
	bool ReadRaster(const CStringW ImageFile, GDALAccess accessMode);

	bool getFileType(const CStringW ImageFile, ImageType &ft);
	bool WritePPM(CStringW ImageFile, bool WorldFile, ICallback *cBack);
	bool WriteGIF(CString ImageFile, bool WorldFile, ICallback * cBack);
	bool WriteBMP(CStringW FileName, bool WriteWorldFile, ICallback *cBack);
	bool WriteGDIPlus(CStringW ImageFile, bool WorldFile, ImageType type, ICallback *cBack);

	VARIANT_BOOL WriteWorldFile(CStringW WorldFileName);
	bool ReadWorldFile(CStringW WorldFileName);

	bool IsGdalImageAvailable();
	bool BuildColorMap(colour* data, int size, VARIANT* Colors, VARIANT* Frequencies, long* count);
	bool SetImageBitsDCCore(HDC hdc);
	bool CheckForProxy();
	void ReadProjection();
	bool CopyGdalDataset(CStringW imageFilename, ImageType fileType, bool writeWorldFile);

public:
	bool DeserializeCore(CPLXMLNode* node);
	CPLXMLNode* SerializeCore(VARIANT_BOOL SerializePixels, CString ElementName);

	bool SetDCBitsToImage(long hDC,BYTE* bits);
	unsigned char* get_ImageData();
	void put_ImageData(colour* data);
	bool get_BufferIsDownsampled();

	void ClearBuffer();
	bool SaveNotNullPixels(bool forceSaving = false);
	void ClearNotNullPixels();
	void OpenImage(CStringW ImageFileName, ImageType FileType, VARIANT_BOOL InRam, ICallback *cBack, GDALAccess accessMode, bool checkForProxy, VARIANT_BOOL *retval);
	
	int get_originalBufferWidth()
	{
		if (_gdalImage && _rasterImage)	return (_rasterImage->visibleRect.right - _rasterImage->visibleRect.left);
		else							return 0;
	}	
	int get_originalBufferHeight()
	{
		if (_gdalImage && _rasterImage)	return (_rasterImage->visibleRect.right - _rasterImage->visibleRect.left);
		else							return 0;
	}
	void ErrorMessage(long ErrorCode);
	void LoadImageAttributesFromGridColorScheme(CGridColorScheme* scheme);
	
};