#pragma once


struct FieldOperation
{
	int targetIndex;
	int fieldIndex;
	CStringW fieldName;
	bool hasName;
	tkFieldStatOperation operation;
	bool valid;
	FieldType targetFieldType;
	tkFieldOperationValidity isValidReason;

	FieldOperation()
	{
		targetIndex = -1;
		fieldIndex = -1;
		fieldName = L"";
		hasName = false;
		valid = true;
	}
};

// CFieldStatOperations
class  CFieldStatOperations
{
public:
	CFieldStatOperations()
	{
		_lastErrorCode = tkNO_ERROR;
		_key = SysAllocString(L"");
	}

	~CFieldStatOperations()
	{
		Clear();
	}



public:
	void AddFieldIndex(int fieldIndex, tkFieldStatOperation operation);
	void AddFieldName(BSTR fieldIndex, tkFieldStatOperation operation);
	void Remove(int operationIndex, VARIANT_BOOL* retVal);
	void Clear();
	void get_Count(int* retVal);
	void get_Operation(int operationIndex, tkFieldStatOperation* retVal);
	void get_FieldIndex(int operationIndex, int* retVal);
	void get_ErrorMsg(/*[in]*/ long ErrorCode, /*[out, retval]*/ BSTR *pVal);
	void get_LastErrorCode(/*[out, retval]*/ long *pVal);
	void get_Key(/*[out, retval]*/ BSTR *pVal);
	void put_Key(/*[in]*/ BSTR newVal);
	void get_FieldName(int operationIndex, BSTR* retVal);
	void Validate(CShapefile* sf, VARIANT_BOOL* retVal);
	void get_OperationIsValid(int operationIndex, VARIANT_BOOL* retVal);
	void get_OperationIsValidReason(int operationIndex, tkFieldOperationValidity* retVal);

private:
	void ErrorMessage(long ErrorCode);
	long _lastErrorCode;
	BSTR _key;

public:
	std::vector<FieldOperation*> _operations;
};


