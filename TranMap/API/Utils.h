
#pragma once
#include "OgrConverter.h"
#include "GridInterpolate.h"
#include "XRedBlackTree.h"
#include "YRedBlackTree.h"
#include "Tiles.h"
#include "Tin.h"
#include "CallbackHelper.h"
#include "GlobalVars.h"

struct BreakVal
{
	double lowVal;
	double highVal;
	double newVal;
};

#define GEOTRSFRM_TOPLEFT_X            0
#define GEOTRSFRM_WE_RES               1
#define GEOTRSFRM_ROTATION_PARAM1      2
#define GEOTRSFRM_TOPLEFT_Y            3
#define GEOTRSFRM_ROTATION_PARAM2      4
#define GEOTRSFRM_NS_RES               5

// CUtils
class  CUtils
{
public:
	CUtils()
	{
		_pip_left = 0;
		_pip_right = 0;
		_pip_top = 0;
		_pip_bottom = 0;		

		_lastErrorCode = tkNO_ERROR;
		_globalCallback = NULL;
		_key = SysAllocString(L"");
			
		_bufferA_R = NULL;
		_bufferA_G = NULL;
		_bufferA_B = NULL;
		_bufferB_R = NULL;
		_bufferB_G = NULL;
		_bufferB_B = NULL;
		_bufferLastUsed = 'Z';
		_bufferANum = -1;
		_bufferBNum = -1;
		_rasterDataset = NULL;

		_bSubCall = FALSE;
		_tileProjections[0] = NULL;
		_tileProjections[1] = NULL;
	}
	~CUtils()
	{
		for(int i = 0; i < 2; i++)
		{
			if (_tileProjections[i]) {
				delete _tileProjections[i];
				_tileProjections[i] = NULL;
			}
		}
		::SysFreeString(_key);
	}


// IUtils
public:
	void ClipPolygon(/*[in]*/ PolygonOperation op, /*[in]*/ CShape * SubjectPolygon, /*[in]*/ CShape * ClipPolygon, /*[out, retval]*/ CShape ** retval);
	void get_Key(/*[out, retval]*/ BSTR *pVal);
	void put_Key(/*[in]*/ BSTR newVal);
	void get_GlobalCallback(/*[out, retval]*/ ICallback * *pVal);
	void put_GlobalCallback(/*[in]*/ ICallback * newVal);
	void get_ErrorMsg(/*[in]*/ long ErrorCode, /*[out, retval]*/ BSTR *pVal);
	void get_LastErrorCode(/*[out, retval]*/ long *pVal);
	void get_Area(/*[in]*/ CShape * Shape, /*[out, retval]*/ double *pVal);
	void get_Perimeter(/*[in]*/ CShape * Shape, /*[out, retval]*/ double *pVal);
	void get_Length(/*[in]*/ CShape * Shape, /*[out, retval]*/ double *pVal);
	void RemoveColinearPoints(/*[in,out]*/ CShapefile * Shapes, /*[in]*/double LinearTolerance, /*[in, optional]*/ICallback * cBack, /*[out, retval]*/ VARIANT_BOOL * retval);
	void GridInterpolateNoData(/*[in, out]*/ CGrid * Grid, /*[in, optional]*/ICallback * cBack, /*[out, retval]*/ VARIANT_BOOL * retval);
	void GridReplace(/*[in, out]*/ CGrid * Grid, /*[in]*/ VARIANT OldValue, /*[in]*/ VARIANT NewValue, /*[in, optional]*/ICallback * cBack, /*[out, retval]*/ VARIANT_BOOL * retval);
	void PointInPolygon(/*[in]*/ CShape * Shape, /*[in]*/CPointClass * TestPoint, /*[out, retval]*/ VARIANT_BOOL * retval);
	void ShapeMerge(/*[in]*/ CShapefile * Shapes, /*[in]*/long IndexOne, /*[in]*/long IndexTwo, /*[in, optional]*/ICallback * cBack, /*[out, retval]*/ CShape ** retval);
	void GridMerge(/*[in]*/ VARIANT Grids, /*[in]*/ BSTR MergeFilename, /*[in, optional, defaultvalue(TRUE)]*/ VARIANT_BOOL InRam, /*[in, optional, defaultvalue(UseExtension)]*/ GridFileType GrdFileType, /*[in, optional]*/ICallback * cBack, /*[out, retval]*/ CGrid ** retval);
	void hBitmapToPicture(/*[in]*/ long hBitmap, /*[out, retval]*/ IPictureDisp ** retval);
	void ShapefileToGrid(/*[in]*/ CShapefile * Shpfile,/* [in, optional, defaultvalue(TRUE)]*/ VARIANT_BOOL UseShapefileBounds, /*[in, optional, defaultvalue(NULL)]*/CGridHeader * GrdHeader, /*[in,optional, defaultvalue(30.0)]*/double Cellsize, /*[in,optional, defaultvalue(TRUE)]*/VARIANT_BOOL UseShapeNumber, /*[in,optional, defaultvalue(1)]*/short SingleValue,/*[out, retval]*/ CGrid ** retval);
	void TinToShapefile(/*[in]*/ CTin * Tin, /*[in]*/ ShpfileType Type, /*[in, optional]*/ICallback * cBack, /*[out, retval]*/ CShapefile ** retval);
	void ShapeToShapeZ(/*[in]*/ CShapefile * Shapefile, /*[in]*/ CGrid * Grid, /*[in, optional]*/ICallback * cBack, /*[out, retval]*/ CShapefile ** retval);
	void GridToGrid(/*[in]*/ CGrid * Grid, /*[in]*/ GridDataType OutDataType, /*[in, optional]*/ICallback * cBack, /*[out, retval]*/ CGrid ** retval);
	void GridToShapefile(/*[in]*/ CGrid * Grid, /*[in, optional, defaultvalue(NULL)]*/ CGrid * ConnectionGrid, /*[in, optional]*/ICallback * cBack, /*[out, retval]*/ CShapefile ** retval);
	void GridToImage(/*[in]*/ CGrid * Grid, /*[in]*/ CGridColorScheme * ci, /*[in, optional]*/ICallback * cBack, /*[out, retval]*/ CImageClass ** retval);
	void GenerateHillShade(/*[in]*/ BSTR bstrGridFilename, /*[in]*/ BSTR bstrShadeFilename, /*[in, optional, defaultvalue(1)]*/ float z, /*[in, optional, defaultvalue(1)]*/ float scale, /*[in, optional, defaultvalue(315)]*/ float az, /*[in, optional, defaultvalue(45)]*/ float alt, /*[out, retval]*/ VARIANT_BOOL * retval);
	void GenerateContour(/*[in]*/ BSTR pszSrcFilename, /*[in]*/ BSTR pszDstFilename, /*[in]*/ double dfInterval, /*[in, optional, defaultvalue(0)]*/ double dfNoData, /*[in, optional, defaultvalue(FALSE)]*/ VARIANT_BOOL Is3D, /* [in, optional, defaultvalue(NULL)] */ VARIANT dblFLArray, /*[in, optional]*/ ICallback * cBack, /*[out, retval]*/ VARIANT_BOOL * retval);
	void TranslateRaster(/*[in]*/ BSTR bstrSrcFilename, /*[in]*/ BSTR bstrDstFilename, /*[in]*/ BSTR bstrOptions, /*[in, optional]*/ ICallback * cBack, /*[out, retval]*/ VARIANT_BOOL * retval);
	void GDALInfo(/*[in]*/ BSTR bstrSrcFilename, /*[in]*/ BSTR bstrOptions, /*[in, optional]*/ ICallback * cBack, /*[out, retval]*/ BSTR * bstrInfo);

	void GDALWarp(/*[in]*/ BSTR bstrSrcFilename, /*[in]*/ BSTR bstrDstFilename, /*[in]*/ BSTR bstrOptions, /*[in, optional]*/ ICallback * cBack, /*[out, retval]*/ VARIANT_BOOL * retval);
	void GDALBuildVrt(/*[in]*/ BSTR bstrDstFilename, /*[in]*/ BSTR bstrOptions, /*[in, optional]*/ ICallback * cBack, /*[out, retval]*/ VARIANT_BOOL * retval);
	void GDALAddOverviews(/*[in]*/ BSTR bstrSrcFilename, /*[in]*/ BSTR bstrDstFilename, /*[in]*/ BSTR bstrLevels, /*[in, optional]*/ ICallback * cBack, /*[out, retval]*/ VARIANT_BOOL * retval);
	void GDALRasterize(/*[in]*/ BSTR bstrSrcFilename, /*[in]*/ BSTR bstrDstFilename, /*[in]*/ BSTR bstrOptions, /*[in, optional]*/ ICallback * cBack, /*[out, retval]*/ VARIANT_BOOL * retval);
	void OGRInfo(/*[in]*/ BSTR bstrSrcFilename, /*[in]*/ BSTR bstrOptions, /*[in, optional]*/ BSTR bstrLayers, /*[in, optional]*/ ICallback * cBack, /*[out, retval]*/ BSTR * bstrInfo);
	void OGR2OGR(/*[in]*/ BSTR bstrSrcFilename, /*[in]*/ BSTR bstrDstFilename, /*[in]*/ BSTR bstrOptions, /*[in, optional]*/ ICallback * cBack, /*[out, retval]*/ VARIANT_BOOL * retval);

	void OGRLayerToShapefile(/*[in]*/BSTR Filename, /*[in, optional, defaultvalue(SHP_NULLSHAPE)]*/ ShpfileType shpType, /*[in, optional, defaultvalue(NULL)]*/ICallback *cBack, /*[out, retval]*/CShapefile** sf);
	void MergeImages(/*[in]*/SAFEARRAY* InputNames, /*[in]*/BSTR OutputName, VARIANT_BOOL* retVal);
	void ReprojectShapefile(CShapefile* sf, CGeoProjection* source, CGeoProjection* target, CShapefile** result);

	void ClipGridWithPolygon(BSTR inputGridfile, CShape* poly, BSTR resultGridfile, VARIANT_BOOL keepExtents, VARIANT_BOOL* retVal);
	void ClipGridWithPolygon2(CGrid* grid, CShape* poly, BSTR resultGridfile, VARIANT_BOOL keepExtents, VARIANT_BOOL* retVal);
	void GridStatisticsToShapefile(CGrid* grid, CShapefile* sf, VARIANT_BOOL selectedOnly, VARIANT_BOOL overwriteFields, VARIANT_BOOL* retVal) ;
	void Polygonize(/*[in]*/ BSTR pszSrcFilename, /*[in]*/ BSTR pszDstFilename, /*[in, optional, defaultvalue(1)]*/ int iSrcBand, /*[in, optional, defaultvalue(FALSE)]*/ VARIANT_BOOL NoMask, /*[in, optional, defaultvalue(NULL)]*/ BSTR pszMaskFilename, /*[in, optional, defaultvalue("GML")]*/ BSTR pszOGRFormat, /*[in, optional, defaultvalue("out")]*/ BSTR pszDstLayerName, /*[in, optional, defaultvalue("DN")]*/ BSTR pszPixValFieldName, /*[in, optional, defaultvalue(NULL)]*/ ICallback * cBack, /*[out, retval]*/ VARIANT_BOOL * retval);
	void CopyNodataValues(BSTR sourceFilename, BSTR destFilename, VARIANT_BOOL* retVal);
	void ColorByName(tkMapColor name, OLE_COLOR* retVal);
	void ConvertDistance(tkUnitsOfMeasure sourceUnit, tkUnitsOfMeasure targetUnit, DOUBLE* value, VARIANT_BOOL* retval);
	void GeodesicDistance(double lat1, double lng1, double lat2, double lng2, double* retVal);
	void MaskRaster(BSTR filename, BYTE newPerBandValue, VARIANT_BOOL* retVal);
	void GridStatisticsForPolygon(CGrid* grid, CGridHeader* header, CExtents* gridExtents, CShape* shape, 
										  double noDataValue, double* meanValue, double* minValue, double* maxValue, VARIANT_BOOL* retVal);
	void GridToImage2(CGrid * Grid, CGridColorScheme * ci, tkGridProxyFormat imageFormat, VARIANT_BOOL inRam, ICallback* cBack, CImageClass ** retval);
	void ErrorMsgFromObject(IDispatch * comClass, BSTR* retVal);
	void TileProjectionToGeoProjection(tkTileProjection projection, CGeoProjection** retVal);
	void get_ComUsageReport(VARIANT_BOOL unreleasedOnly, BSTR* retVal);
	void CalculateRaster(SAFEARRAY* filenames, BSTR expression, BSTR outputFilename, BSTR gdalOutputFormat, 
		float noDataValue, ICallback* cBack, BSTR* errorMsg, VARIANT_BOOL* retVal);
	void ReclassifyRaster(BSTR Filename, int bandIndex, BSTR outputName, SAFEARRAY* LowerBounds, 
		SAFEARRAY* UpperBounds, SAFEARRAY* NewValues, 
		BSTR gdalOutputFormat, ICallback* cBack, VARIANT_BOOL* retVal);

private:
	struct RasterPoint
	{
		RasterPoint()
		{
			column = 0;
			row = 0;
		}
		RasterPoint(long c, long r)
		{
			column = c;
			row = r;
		}

		long row;
		long column;
	};

private:
	//Polygonal Algorithm
	//Used to minimize stack in recursive call for trace_polygon
	//Cell 4
	long _cell4_x;
	long _cell4_y;
	//Cell 6
	long _cell6_x;
	long cell6_y;
	//Cell 2
	long _cell2_x;
	long _cell2_y;
	//Cell 8			
	long _cell8_x;
	long _cell8_y;
	//Flow Directions
	double _flow2;
	double _flow8;
	double _flow4;
	double _flow6;
	CGrid * _expand_grid;
	CGrid * _connection_grid;

	bool _canScanlineBuffer;
	char _bufferLastUsed;
	int _bufferANum;
	int _bufferBNum;
	_int32 * _bufferA_R;
	_int32 * _bufferA_G;
	_int32 * _bufferA_B;
	_int32 * _bufferB_R;
	_int32 * _bufferB_G;
	_int32 * _bufferB_B;
	GDALRasterBand * _poBandR;
	GDALRasterBand * _poBandG;
	GDALRasterBand * _poBandB;
	GDALDataset * _rasterDataset;
	vector<double> _pip_xs;
	vector<double> _pip_xs_parts;
	std::deque<long> _pip_cache_parts;
	std::deque<double> _pip_cache_pointsX;
	std::deque<double> _pip_cache_pointsY;
	double _pip_left;
	double _pip_right;
	double _pip_top;
	double _pip_bottom;
	long _lastErrorCode;
	ICallback * _globalCallback;
	BSTR _key;
	CGeoProjection* _tileProjections[2];

	/* GDAL/OGR variables */
	int _bSubCall;
	CStringArray _sArr;
	CStringArray _sConfig;

private:
	inline long findBreak(std::deque<BreakVal> & bvals, double val);
	bool PolygonToGrid(CShape * shape, CGrid ** grid, short cellValue);

	void trace_polygon(long x, long y, std::deque<RasterPoint> & polygon);
	inline bool is_joint(double cell2, double cell8, double cell4, double cell6);
	inline double getValue(CGrid * Grid, long column, long row);
	inline void setValue(CGrid * Grid, long column, long row, double val);
	void scan_fill_to_edge(double & nodata, long x, long y);
	void mark_edge(double & polygon_id, long x, long y);
	inline bool is_decision(CGrid * g, int x, int y);

	inline bool does_cross( int SH, int NSH, double corner_oneX, double corner_oneY, double corner_twoX, double corner_twoY );
	inline void set_sign( double val, int & SH );
	bool is_clockwise( double x0, double y0, double x1, double y1, double x2, double y2); //ah 11/8/05
	bool is_clockwise(CShape *Shape); //ah 6/3/05
	bool is_clockwise(Poly *polygon);//ah 6/3/05
	
	bool isColinear( POINT one, POINT two, POINT test, double tolerance );
	inline void PutBitmapValue(long col, long row, _int32 Rvalue, _int32 Gvalue, _int32 Bvalue, long totalWidth);
	void CreateBitmap(CStringW filename, long cols, long rows, tkGridProxyFormat format, VARIANT_BOOL * retval);
	bool MemoryAvailable(double bytes);
	void FinalizeAndCloseBitmap(int totalWidth);

	HRESULT RunGridToImage(CGrid * Grid, CGridColorScheme * ci, tkGridProxyFormat imageFormat, 
								bool inRam, bool checkMemory, ICallback* callback, CImageClass ** retval);
	void GridToImageCore(CGrid *Grid, CGridColorScheme *ci, ICallback *cBack, bool inRam, CImageClass ** retval);
	inline void WritePixel(CImageClass* img, int row, int col, OLE_COLOR color, 
								int nodataColor_R, int nodataColor_G, int nodataColor_B, int ncols, bool inRam);
	void WriteWorldFile(CStringW worldFile, CStringW imageFile, double dx, double dy, double xll, double yll, int nrows);
	void ErrorMessage(long ErrorCode);
	void ErrorMessage(ICallback* callback, long ErrorCode);
	bool ValidateInputNames(SAFEARRAY* InputNames, LONG& lLBound, LONG& lUBound, BSTR **pbstr);
	bool ParseSafeArray(SAFEARRAY* arr, LONG& lLBound, LONG& lUBound, void **pbstr);

	/* GDAL/OGR functions */
	void Parse(CString sOrig, int * opts);
	BOOL ProcessGeneralOptions(int * opts);
	HRESULT ResetConfigOptions(long ErrorCode = 0);
	//ICallback* GetCallback() { return m_globalSettings.callback ? m_globalSettings.callback : _globalCallback; }
	BOOL IsTIFFGrid(LPCTSTR Filename);

public:
	HRESULT TileProjectionToGeoProjectionCore(tkTileProjection projection, VARIANT_BOOL useCache, CGeoProjection** retVal);
	
	void IsTiffGrid(BSTR Filename, VARIANT_BOOL* retVal);
};

double CalcPolyGeodesicArea(std::vector<Point2D>& points);