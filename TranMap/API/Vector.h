
#pragma once
#include "cppVector.h"

// CVector
class  CVector 
{
public:
	CVector()
	{
		_lastErrorCode = tkNO_ERROR;
		_globalCallback = NULL;
		_key = SysAllocString(L"");
	}
	~CVector()
	{
		::SysFreeString(_key);
	}

// IVectro
public:
	void get_Key(/*[out, retval]*/ BSTR *pVal);
	void put_Key(/*[in]*/ BSTR newVal);
	void get_GlobalCallback(/*[out, retval]*/ ICallback * *pVal);
	void put_GlobalCallback(/*[in]*/ ICallback * newVal);
	void get_ErrorMsg(/*[in]*/ long ErrorCode, /*[out, retval]*/ BSTR *pVal);
	void get_LastErrorCode(/*[out, retval]*/ long *pVal);
	void CrossProduct(/*[in]*/ CVector * V, /*[out,retval]*/ CVector ** result);
	void Dot(/*[in]*/ CVector * V, /*[out,retval]*/ double * result);
	void Normalize();
	void get_k(/*[out, retval]*/ double *pVal);
	void put_k(/*[in]*/ double newVal);
	void get_j(/*[out, retval]*/ double *pVal);
	void put_j(/*[in]*/ double newVal);
	void get_i(/*[out, retval]*/ double *pVal);
	void put_i(/*[in]*/ double newVal);

private:
	cppVector _v;
	long _lastErrorCode;
	ICallback * _globalCallback;
	BSTR _key;
};


