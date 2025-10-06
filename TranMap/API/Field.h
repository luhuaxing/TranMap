#pragma once
#include "GlobalVars.h"
// CField
#include "Table.h"

class  CField
{
public:
	CField()
	{
		_key = SysAllocString(L"");
		_name = SysAllocString(L"");
		_globalCallback = NULL;
		_lastErrorCode = tkNO_ERROR;
		_width = 10;
		_precision = 10;
		_type = STRING_FIELD;
		_isUpdated = false;
		_table = NULL;
	//	gReferenceCounter.AddRef(tkInterface::idField);
	}

	~CField()
	{
		::SysFreeString(_key);
		::SysFreeString(_name);

		//if( _globalCallback )
		//	_globalCallback->Release();
		//_globalCallback = NULL;
		//gReferenceCounter.Release(tkInterface::idField);
	}




// CField
public:
	void get_Key(/*[out, retval]*/ BSTR *pVal);
	void put_Key(/*[in]*/ BSTR newVal);
	void get_GlobalCallback(/*[out, retval]*/ ICallback * *pVal);
	void put_GlobalCallback(/*[in]*/ ICallback * newVal);
	void get_ErrorMsg(/*[in]*/ long ErrorCode, /*[out, retval]*/ BSTR *pVal);
	void get_LastErrorCode(/*[out, retval]*/ long *pVal);
	void get_Type(/*[out, retval]*/ FieldType *pVal);
	void put_Type(/*[in]*/ FieldType newVal);
	void get_Precision(/*[out, retval]*/ long *pVal);
	void put_Precision(/*[in]*/ long newVal);
	void get_Width(/*[out, retval]*/ long *pVal);
	void put_Width(/*[in]*/ long newVal);
	void get_Name(/*[out, retval]*/ BSTR *pVal);
	void put_Name(/*[in]*/ BSTR newVal);
	void Clone(/*[out, retval]*/ CField** retVal);
	
	void ErrorMessage(long ErrorCode);
	CTableClass* GetTable() { return _table; }
	void SetTable(CTableClass* table) { _table = table; }
	bool GetIsUpdated() {return _isUpdated; }
	void SetIsUpdated(bool value) { _isUpdated = value; }

private:
	CTableClass* _table;		// no references should be added here
	ICallback * _globalCallback;
	FieldType _type;
	BSTR _key;
	BSTR _name;
	long _lastErrorCode;
	long _precision;
	long _width;
	bool _isUpdated;

private:
	bool CheckTableEditingState();
};

