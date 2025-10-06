#pragma once
#include <set>
#include "dgrid.h"
#include "fgrid.h"
#include "lgrid.h"
#include "sgrid.h"
#include "GridManager.h"
#include "tkGridRaster.h"
#include "Image.h"
#include "GridColorScheme.h"
#include "GridHeader.h"
#include "Extents.h"
#include "ILayer.h"
// CGrid
class  CGrid:public ILayer
{
public:
	CGrid();
	~CGrid();
	void CallBack(long percent,const char * message);

// CGrid
public:
	eLayerType GetLayerType() { return eLayerType::GRID_LAYER; }
	void get_CdlgFilter(/*[out, retval]*/ BSTR *pVal);
	void CellToProj(/*[in]*/ long Column, /*[in]*/ long Row, /*[out]*/ double * x, /*[out]*/ double * y);
	void ProjToCell(/*[in]*/ double x, /*[in]*/ double y, /*[out]*/ long * Column, /*[out]*/ long * Row);
	void Clear(/*[in]*/ VARIANT ClearValue, /*[out, retval]*/ VARIANT_BOOL * retval);
	void Save(/*[in, optional]*/ BSTR Filename, /*[in, optional, defaultvalue(tkUseExtension)]*/ GridFileType FileType, /*[in]*/ ICallback * cBack, /*[out, retval]*/ VARIANT_BOOL * retval);
	void Close(/*[out, retval]*/ VARIANT_BOOL * retval);
	void CreateNew(/*[in]*/ BSTR Filename, /*[in]*/ CGridHeader * Header, /*[in]*/ GridDataType DataType, /*[in]*/ VARIANT InitialValue, /*[in, optional, defaultvalue(TRUE)]*/ VARIANT_BOOL InRam, /*[in, optional, defaultvalue(tkUseExtension)]*/ GridFileType FileType, /*[in, optional]*/ ICallback * cBack, /*[out, retval]*/ VARIANT_BOOL * retval);
	void Open(/*[in]*/ BSTR Filename, /*[in, optional, defaultvalue(tkUnknown)]*/ GridDataType DataType, /*[in, optional, defaultvalue(TRUE)]*/ VARIANT_BOOL InRam, /*[in, optional, defaultvalue(tkUseExtension)]*/ GridFileType FileType, /*[in, optional]*/ ICallback * cBack, /*[out, retval]*/ VARIANT_BOOL * retval);
	void get_Key(/*[out, retval]*/ BSTR *pVal);
	void put_Key(/*[in]*/ BSTR newVal);
	void get_GlobalCallback(/*[out, retval]*/ ICallback * *pVal);
	void put_GlobalCallback(/*[in]*/ ICallback * newVal);
	void get_ErrorMsg(/*[in]*/ long ErrorCode, /*[out, retval]*/ BSTR *pVal);
	void get_LastErrorCode(/*[out, retval]*/ long *pVal);
	void get_Filename(/*[out, retval]*/ BSTR *pVal);
	void get_DataType(/*[out, retval]*/ GridDataType *pVal);
	void get_Minimum(/*[out, retval]*/ VARIANT *pVal);
	void get_Maximum(/*[out, retval]*/ VARIANT *pVal);
	void get_InRam(/*[out, retval]*/ VARIANT_BOOL *pVal);
	void get_Value(/*[in]*/ long Column, /*[in]*/ long Row, /*[out, retval]*/ VARIANT *pVal);
	void put_Value(/*[in]*/ long Column, /*[in]*/ long Row, /*[in]*/ VARIANT newVal);
	void get_Header(/*[out, retval]*/ CGridHeader * *pVal);
	void AssignNewProjection(/*[in]*/ BSTR projection, /*[out, retval]*/ VARIANT_BOOL * retval);
	void get_RasterColorTableColoringScheme(/*[out, retval]*/ CGridColorScheme * *pVal);
	void GetRow(/*[in]*/ long Row, /*[in, out]*/ float *Vals, /*[out, retval]*/ VARIANT_BOOL * retval);
	void GetRow2(/*[in]*/ long Row, /*[in, out]*/ double *Vals, /*[out, retval]*/ VARIANT_BOOL * retval);
	void PutRow(/*[in]*/ long Row, /*[in]*/ float *Vals, /*[out, retval]*/ VARIANT_BOOL * retval);
	void PutRow2(/*[in]*/ long Row, /*[in]*/ double *Vals, /*[out, retval]*/ VARIANT_BOOL * retval);
	void GetFloatWindow(/*[in]*/ long StartRow, /*[in]*/ long EndRow, /*[in]*/ long StartCol, /*[in]*/ long EndCol, /*[in, out]*/ float * Vals, /*[out, retval]*/ VARIANT_BOOL * retval);
	void PutFloatWindow(/*[in]*/ long StartRow, /*[in]*/ long EndRow, /*[in]*/ long StartCol, /*[in]*/ long EndCol, /*[in, out]*/ float * Vals, /*[out, retval]*/ VARIANT_BOOL * retval);
	void GetFloatWindow2(/*[in]*/ long StartRow, /*[in]*/ long EndRow, /*[in]*/ long StartCol, /*[in]*/ long EndCol, /*[in, out]*/ double * Vals, /*[out, retval]*/ VARIANT_BOOL * retval);
	void PutFloatWindow2(/*[in]*/ long StartRow, /*[in]*/ long EndRow, /*[in]*/ long StartCol, /*[in]*/ long EndCol, /*[in, out]*/ double * Vals, /*[out, retval]*/ VARIANT_BOOL * retval);
	void SetInvalidValuesToNodata(/*[in*/ double MinThresholdValue, /*[in*/ double MaxThresholdValue, /*out, retval*/ VARIANT_BOOL * retval);
	void Resource(/*[in]*/ BSTR newSrcPath, /*[out, retval]*/ VARIANT_BOOL * retval);
	void get_Extents(CExtents** retVal);
	void get_NumBands(int *retVal);
	void get_ActiveBandIndex(int *retVal);
	void OpenBand(int bandIndex, VARIANT_BOOL* retVal);
	void get_SourceType(tkGridSourceType* retVal);
	void RetrieveColorScheme(tkGridSchemeRetrieval method, CGridColorScheme** retVal);
	void GenerateColorScheme(tkGridSchemeGeneration method, PredefinedColorScheme colors, CGridColorScheme** retVal);
	void RetrieveOrGenerateColorScheme(tkGridSchemeRetrieval retrievalMethod, tkGridSchemeGeneration generateMethod, PredefinedColorScheme colors, CGridColorScheme** retVal);
	void OpenAsImage(CGridColorScheme* scheme, tkGridProxyMode proxyMode, ICallback* cBack, CImageClass ** retVal);
	void RemoveImageProxy(VARIANT_BOOL* retVal);
	void CreateImageProxy(CGridColorScheme* colorScheme, CImageClass** retVal);
	void get_PreferedDisplayMode( tkGridProxyMode *retVal);
	void put_PreferedDisplayMode( tkGridProxyMode newVal);
	void get_HasValidImageProxy( VARIANT_BOOL *retVal);

private:
	ICallback * _globalCallback;
	long _lastErrorCode;
	CStringW _filename;
	BSTR _key;

	dGrid * _dgrid;
	fGrid * _fgrid;
	lGrid * _lgrid;
	sGrid * _sgrid;
	tkGridRaster * _trgrid;
	tkGridProxyMode _preferedDisplayMode;

private:
	bool MemoryAvailable(double bytes);
	void set_ProjectionIntoHeader(char * projection);
	void ResolveFileType(GridFileType &newFileType, CString extension);
	void SaveProjectionAsWkt();
	bool OpenCustomGrid(GridDataType DataType, bool inRam, GridFileType FileType);
	void TryOpenAsAsciiGrid(GridDataType DataType, bool& inRam, bool& forcingGDALUse);
	bool OpenAuxHeader(CStringW& filename);
	void OpenAsDirectImage(CGridColorScheme* scheme, ICallback* cBack, CImageClass** retVal);

public:
	// Gets name of the legend file associated with grid
	CStringW GetFilename() { return _filename; }
	CStringW GetLegendName() {	return Utility::GetPathWOExtension(_filename) + L".mwleg"; }
	CStringW GetProxyLegendName() {	return GridManager::GetProxyLegendName(GetFilename()); }
	CStringW GetProxyName() {return GridManager::GetProxyName(GetFilename());}
	CStringW GetProxyWorldFileName() {return GridManager::GetProxyWorldFileName(GetFilename());	}

	bool IsRgb();
	CImageClass* OpenImageProxy();
	void SaveProjection(char* projection);
	bool RemoveColorSchemeFile();
	CGrid* Clone(BSTR newFilename);
	CGrid* Clip(BSTR newFilename, long firstCol, long lastCol, long firstRow, long lastRow);
	void ErrorMessage(long ErrorCode);

	bool BuildUniqueColorScheme(int maxValuesCount, PredefinedColorScheme colors, ColoringType coloringType, CGridColorScheme** newscheme);
	CImageClass* GridToImage(CGridColorScheme* scheme);

	CGridColorScheme* BuildGradientColorSchemeCore(PredefinedColorScheme colors, ColoringType coloringType);
	bool GetUniqueValues(set<CComVariant>& values, int maxCount);
	void GetFloatWindowCore(long StartRow, long EndRow, long StartCol, long EndCol, void *Vals, bool useDouble, VARIANT_BOOL * retval);
	void PutFloatWindowCore(long StartRow, long EndRow, long StartCol, long EndCol, void *Vals, bool useDouble, VARIANT_BOOL * retval);
	bool PutRowDouble(long Row, double *Vals);
	void PutRowCore(long Row, void *Vals, bool useDouble, VARIANT_BOOL * retval);
	void GetRowCore(long Row, void *Vals, bool useDouble, VARIANT_BOOL * retval);
};

