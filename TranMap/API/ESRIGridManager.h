#pragma once
#include "GridManager.h"

// CESRIGridManager
class CESRIGridManager
{
public:
	CESRIGridManager()
	{	_globalCallback = NULL;
	}

// IESRIGridManager
public:
	void IsESRIGrid(/*[in]*/ BSTR Filename, /*[out, retval]*/ VARIANT_BOOL * retval);
	void DeleteESRIGrids(/*[in]*/ BSTR Filename, /*[out, retval]*/ VARIANT_BOOL * retval);
	void CanUseESRIGrids(/*[out, retval]*/ VARIANT_BOOL * retval);
	void get_GlobalCallback(/*[out, retval]*/ ICallback * *pVal);
	void put_GlobalCallback(/*[in]*/ ICallback * newVal);
	void get_ErrorMsg(/*[in]*/ long ErrorCode, /*[out, retval]*/ BSTR *pVal);
	void get_LastErrorCode(/*[out, retval]*/ long *pVal);

private:
	ICallback * _globalCallback;
	GridManager _gm;
};