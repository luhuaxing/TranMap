#pragma once
#include "fheader.h"
#include "sheader.h"
#include "dheader.h"
#include "lheader.h"
#include "tkGridRaster.h"
#include "GeoProjection.h"

// CGridHeader
class  CGridHeader
{
public:
	CGridHeader()
	{	USES_CONVERSION;
		_dx = 1.0;
		_dy = 1.0;	
		_key = SysAllocString(L"");
		_notes = SysAllocString(L"");
		_colorTable = SysAllocString(L"");
		_globalCallback = NULL;
		_numbercols = 0;
		_numberrows = 0;
		_nodatavalue = -1;
		_lastErrorCode = tkNO_ERROR;

		_myowner_d = NULL;
		_myowner_f = NULL;
		_myowner_l = NULL;
		_myowner_t = NULL;
		_myowner_s = NULL;

		_projection = new CGeoProjection();
	}
	~CGridHeader()
	{	
		::SysFreeString(_key);
		::SysFreeString(_notes);
		::SysFreeString(_colorTable);

		// Note -- don't delete myowner_*, these are just references
		// to things managed elsewhere
		_myowner_d = NULL;
		_myowner_f = NULL;
		_myowner_l = NULL;
		_myowner_t = NULL;
		_myowner_s = NULL;

		delete _projection;
	}

// CGridHeader
public:
	void get_Key(/*[out, retval]*/ BSTR *pVal);
	void put_Key(/*[in]*/ BSTR newVal);
	void get_GlobalCallback(/*[out, retval]*/ ICallback * *pVal);
	void put_GlobalCallback(/*[in]*/ ICallback * newVal);
	void get_ErrorMsg(/*[in]*/ long ErrorCode, /*[out, retval]*/ BSTR *pVal);
	void get_LastErrorCode(/*[out, retval]*/ long *pVal);
	void get_Notes(/*[out, retval]*/ BSTR *pVal);
	void put_Notes(/*[in]*/ BSTR newVal);
	void get_Projection(/*[out, retval]*/ BSTR *pVal);
	void put_Projection(/*[in]*/ BSTR newVal);
	void get_YllCenter(/*[out, retval]*/ double *pVal);
	void put_YllCenter(/*[in]*/ double newVal);
	void get_XllCenter(/*[out, retval]*/ double *pVal);
	void put_XllCenter(/*[in]*/ double newVal);
	void get_dY(/*[out, retval]*/ double *pVal);
	void put_dY(/*[in]*/ double newVal);
	void get_dX(/*[out, retval]*/ double *pVal);
	void put_dX(/*[in]*/ double newVal);
	void get_NodataValue(/*[out, retval]*/ VARIANT *pVal);
	void put_NodataValue(/*[in]*/ VARIANT newVal);
	void get_NumberRows(/*[out, retval]*/ long *pVal);
	void put_NumberRows(/*[in]*/ long newVal);
	void get_NumberCols(/*[out, retval]*/ long *pVal);
	void put_NumberCols(/*[in]*/ long newVal);
	void put_Owner(/*[in]*/ int * t, /*[in]*/ int * d, /*[in]*/ int * s, /*[in]*/ int * l, /*[in]*/ int * f);
	void CopyFrom(/*[in]*/ CGridHeader * pVal);
	void get_ColorTable(/*[out, retval]*/ BSTR *pVal);
	void put_ColorTable(/*[in]*/ BSTR newVal);
	void get_GeoProjection(CGeoProjection** pVal);
	void put_GeoProjection(CGeoProjection* newVal);

private:
	ICallback * _globalCallback;
	long _lastErrorCode;
	BSTR _key;	
	BSTR _notes;
	BSTR _colorTable;
	double _yllcenter;
	double _xllcenter;
	double _dx;
	double _dy;
	double _nodatavalue;
	long _numberrows;
	long _numbercols;
	CGeoProjection* _projection;

	dHeader * _myowner_d;
	fHeader * _myowner_f;
	sHeader * _myowner_s;
	lHeader * _myowner_l;
	tkGridRaster * _myowner_t;

private:
	void AttemptSave();
	void ErrorMessage(long ErrorCode);
};